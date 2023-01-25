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

class MyMainWindow : public QMainWindow {
    public:
    MyMainWindow();
    QLabel* MyLabel;
    void mousePressEvent(QMouseEvent *event) override;
    QMenu* EditMenu;
    QAction* EditNoviTekst;
    void EditNoviTekstMenu();
    void keyPressEvent(QKeyEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    void FileSaveAsMenu();
    void FileOpenMenu();
    QMenu* FileMenu;
    QAction* FileSaveAs;
    QAction* FileOpen;
};

void MyMainWindow::paintEvent(QPaintEvent*){
    QSize size = this->size();
    QPainter painter(this);
    painter.drawLine(0, size.height(), size.width(), 5);
}


void MyMainWindow::keyPressEvent(QKeyEvent *event) {
    switch( event->key() ){
        case Qt::Key_Left: MyLabel->move(MyLabel->pos().x()-1,
            MyLabel->pos().y());
            break;
        case Qt::Key_Right: MyLabel->move(MyLabel->pos().x()+1,
            MyLabel->pos().y());
            break;
        case Qt::Key_Up: MyLabel->move(MyLabel->pos().x(),
            MyLabel->pos().y()-1);
            break;
        case Qt::Key_Down: MyLabel->move(MyLabel->pos().x(),
            MyLabel->pos().y()+1);
            break;
    }
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
            QMessageBox::information(this, "Unable to Open File", file.errorString());
    return;
    }
    QTextStream out(&file);
    out << "fesb file" << Qt::endl;
    out << MyLabel->text() << Qt::endl;
    out << MyLabel->pos().x() << Qt::endl;
    out << MyLabel->pos().y() << Qt::endl;
    out << pos().x() << Qt::endl;
    out << pos().y() << Qt::endl;
    out << size().width() << Qt::endl;
    out << size().height() << Qt::endl;
    }
}

void MyMainWindow::FileOpenMenu(){
    QString fileName = QFileDialog::getOpenFileName(this, "Open Geometry...", "", "FESB File (*.fsb)");
    if (!fileName.isEmpty()) {
        QFile file(fileName);
        if (!file.open(QIODevice::ReadOnly)) {
            QMessageBox::information(this, "Unable to Open File", file.errorString());
            return;
        }
        QTextStream in(&file);
        QString str; str = in.readLine();
        if(str=="fesb file") {
            str = in.readLine();
            MyLabel->setText(str);
            int x, y, w, h;
            in >> x >> y;
            MyLabel->move(x, y);
            in >> x >> y >> w >> h;
            this->setGeometry(x, y, w, h);
        }
    }
}

MyMainWindow::MyMainWindow() {
    MyLabel = new QLabel(this);
    MyLabel->setText("Hello World!");
    MyLabel->move(10, 20);
    EditNoviTekst = new QAction(tr("&Novi tekst..."), this);
    EditNoviTekst->setShortcut(tr("CTRL+N"));
    connect(EditNoviTekst, &QAction::triggered, this,
    &MyMainWindow::EditNoviTekstMenu);
    EditMenu = menuBar()->addMenu(tr("&Edit"));
    EditMenu->addAction(EditNoviTekst);
    FileSaveAs = new QAction(tr("&Save As..."), this);
    FileSaveAs->setShortcut(tr("CTRL+S"));
    connect(FileSaveAs, &QAction::triggered, this,
    &MyMainWindow::FileSaveAsMenu);
    FileOpen = new QAction(tr("&Open..."), this);
    FileOpen->setShortcut(tr("CTRL+O"));
    connect(FileOpen, &QAction::triggered, this,
    &MyMainWindow::FileOpenMenu);
    FileMenu = menuBar()->addMenu(tr("&File"));
    FileMenu->addAction(FileSaveAs);
    FileMenu->addAction(FileOpen);
}

void MyMainWindow::EditNoviTekstMenu() {
    MyDialog dialog;
    int ret = dialog.exec();
    if( ret == QDialog::Accepted ) {
        MyLabel->setText(dialog.lineEdit->text());
    }
}

void MyMainWindow::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        MyLabel->move(int(event->position().x()),
        int(event->position().y()));
    }
}


int main(int argc, char **argv) {
    QApplication app (argc, argv);
    MyMainWindow mainWindow;
    mainWindow.resize(300,150);
    mainWindow.show();
    return app.exec();
}
