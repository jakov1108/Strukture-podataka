#include <QApplication>
#include <QLabel>
#include <QMainWindow>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QMessageBox>
#include <QMouseEvent>
#include <QLineEdit>
#include <QDialogButtonBox>
#include <QAction>
#include <QMenu>
#include <QMenuBar>
#include <QPainter>
#include <QFileDialog>

int minimalno(int a, int b, int c, int d){
    return std::min( std::min(a,b), std::min(c,d) );
}

class MyMainWindow : public QMainWindow {
    public:
    MyMainWindow();
    QLabel* MyLabel;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    QMenu* EditMenu;
    QAction* EditNoviTekst;
    void EditNoviTekstMenu();
    void paintEvent(QPaintEvent *event) override;
    void FileSaveAsMenu();
    void FileOpenMenu();
    QMenu* FileMenu;
    QAction* FileSaveAs;
    QAction* FileOpen;
    bool mousePress;
    QPoint start;
    QPoint end;
    QRect rect;
};

void MyMainWindow::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    if(mousePress)
        painter.setPen(QPen(Qt::black, 2, Qt::DashLine));
    else
        painter.setPen(QPen(Qt::black, 2));
    painter.drawRect(rect);
}

void MyMainWindow::keyPressEvent(QKeyEvent *event) {
    switch( event->key() ){
        case Qt::Key_Left:
            MyLabel->move(MyLabel->x()-1,MyLabel->y());
            rect.moveTo(rect.x()-1,rect.y());
            break;
        case Qt::Key_Right:
            MyLabel->move(MyLabel->x()+1,MyLabel->y());
            rect.moveTo(rect.x()+1,rect.y());
            break;
        case Qt::Key_Up:
            MyLabel->move(MyLabel->x(),MyLabel->y()-1);
            rect.moveTo(rect.x(),rect.y()-1);
            break;
        case Qt::Key_Down:
            MyLabel->move(MyLabel->x(),MyLabel->y()+1);
            rect.moveTo(rect.x(),rect.y()+1);
            break;
    }
    update();
}

class MyDialog : public QDialog {
    public:
        MyDialog();
        QVBoxLayout* mainLayout;
        QWidget* editWidget;
        QFormLayout* editLayout;
        QLineEdit* lineEdit;
        QDialogButtonBox* buttonBox;
};

MyDialog::MyDialog() {
    lineEdit = new QLineEdit;
    editLayout = new QFormLayout;
    editLayout->addRow(new QLabel(tr("Novi tekst:")), lineEdit);
    editWidget = new QWidget;
    editWidget->setLayout(editLayout);
    buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok |
    QDialogButtonBox::Cancel);
    connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
    mainLayout = new QVBoxLayout;
    mainLayout->addWidget(editWidget);
    mainLayout->addWidget(buttonBox);
    setLayout(mainLayout);
    setWindowTitle("Promjena teksta");
}

void MyMainWindow::FileSaveAsMenu(){
    QString fileName = QFileDialog::getSaveFileName(this,"Save As...", "", "FESB File (*.fsb)");
    if (!fileName.isEmpty()) {
        QFile file(fileName);
        if (!file.open(QIODevice::WriteOnly)) {
            QMessageBox::critical(this, "Error", "Could not open file for writing");
            return;
        }
        QTextStream out(&file);
        out << MyLabel->text() << Qt::endl;
        out << MyLabel->geometry().x() << Qt::endl;
        out << MyLabel->geometry().y() << Qt::endl;
        out << MyLabel->geometry().width() << Qt::endl;
        out << MyLabel->geometry().height() << Qt::endl;
        out << rect.x() << Qt::endl;
        out << rect.y() << Qt::endl;
        out << rect.width() << Qt::endl;
        out << rect.height() << Qt::endl;
        file.close();
    }
}

void MyMainWindow::FileOpenMenu(){
    QString fileName = QFileDialog::getOpenFileName(this,"Open...", "", "FESB File (*.fsb)");
    if (!fileName.isEmpty()) {
        QFile file(fileName);
        if (!file.open(QIODevice::ReadOnly)) {
            QMessageBox::critical(this, "Error", "Could not open file for reading");
            return;
        }
        QTextStream in(&file);
        QString text = in.readLine();
        int x = in.readLine().toInt();
        int y = in.readLine().toInt();
        int labelWidth = in.readLine().toInt();
        int labelHeight = in.readLine().toInt();
        int rx = in.readLine().toInt();
        int ry = in.readLine().toInt();
        int width = in.readLine().toInt();
        int height = in.readLine().toInt();
        MyLabel->setText(text);
        MyLabel->setGeometry(x+3, y, labelWidth, labelHeight);
        rect.setX(rx);
        rect.setY(ry);
        rect.setWidth(width);
        rect.setHeight(height);
        update();
        file.close();
    }
}

MyMainWindow::MyMainWindow(){
    mousePress = false;
    MyLabel = new QLabel("Hello World!", this);
    MyLabel->move(30,30);
    EditMenu = menuBar()->addMenu("Edit");
    EditNoviTekst = EditMenu->addAction("Edit Text");
    connect(EditNoviTekst, &QAction::triggered, this, &MyMainWindow::EditNoviTekstMenu);
    FileSaveAs = new QAction("Save As...", this);
    connect(FileSaveAs, &QAction::triggered, this, &MyMainWindow::FileSaveAsMenu);
    FileOpen = new QAction("Open...", this);
    connect(FileOpen, &QAction::triggered, this, &MyMainWindow::FileOpenMenu);
    FileMenu = menuBar()->addMenu("File");
    FileMenu->addAction(FileSaveAs);
    FileMenu->addAction(FileOpen);
}

void MyMainWindow::EditNoviTekstMenu(){
    MyDialog dialog;
    if(dialog.exec() == QDialog::Accepted) {
        MyLabel->setText(dialog.lineEdit->text());
    }
}

void MyMainWindow::mouseMoveEvent(QMouseEvent *event){
    if(mousePress){
        end = event->pos();
        rect = QRect(start.x(), start.y(), end.x()-start.x(), end.y()-start.y());
        update();
    }
}

void MyMainWindow::mouseReleaseEvent(QMouseEvent *event){
    if(event->button() == Qt::LeftButton){
        mousePress = false;
        MyLabel->setGeometry(minimalno(rect.topLeft().x(),rect.bottomLeft().x(),rect.topRight().x(),rect.bottomRight().x())+3,std::min(rect.topLeft().y(),rect.bottomLeft().y()), MyLabel->size().width(), MyLabel->size().height());
        update();
    }
}

void MyMainWindow::mousePressEvent(QMouseEvent *event){
    if(event->button() == Qt::LeftButton){
        mousePress = true;
        start = event->pos();
    }
}

int main(int argc, char **argv) {
    QApplication app (argc, argv);
    MyMainWindow mainWindow;
    mainWindow.resize(600,300);
    mainWindow.show();
    return app.exec();
}
