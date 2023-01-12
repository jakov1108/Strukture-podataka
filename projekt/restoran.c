#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define FILE_FAIL -1
#define STATUS_FAIL -1
#define SUCCESS 0

struct Order {
    int id;
    char customer[100];
    float amount;
    int time;
    struct Order* left, * right;
};
typedef struct Order Order;
typedef struct Order* OrderPoz;

struct Item
{
    char name[50];
    float price;
    int id;
    struct Item* next;
    struct Item* prev;
};
typedef struct Item Item;
typedef struct Item* ItemPoz;

OrderPoz insertOrder(OrderPoz root, int id, char customer[], float amount, int time);
OrderPoz searchOrder(OrderPoz root, int id);
int printInOrder(OrderPoz root);
int freeTree(OrderPoz root);
int saveOrders(OrderPoz root, char* fileName);
OrderPoz loadOrders(char* fileName);
int createNewOrder();
int printInOrders(OrderPoz);
int printOrders();
int generateRandomNum(int, int);
void ispisRacuna(OrderPoz);
void ispisRek(OrderPoz, FILE*);
int insert(char name[], float price, int id);
void Display();
ItemPoz getnewNode(char name[], float price, int id);
ItemPoz searchItem(int id);
void freeList();
int writeOrders(OrderPoz, FILE*);
void PrintMenu();

ItemPoz head = NULL;
ItemPoz last = NULL;
OrderPoz root = NULL;
static float TECAJ = 7.5345;

int status = 0;

int main() {
    insert("Pizza", 15.99, 1);
    insert("Burger", 12.99, 2);
    insert("Salata", 9.99, 3);
    insert("Tripice", 0.98, 4);
    insert("Coca Cola", 1.99, 5);
    insert("Heineken", 2.49, 6);
    insert("Domaca rakija", 0.99, 7);

    char imeDatoteke[] = { 0 };
    int option;
    int id;
    float price;
    char name[25];
    srand(time(0));

    do {
        PrintMenu();
        printf("Izaberite: ");
        scanf("%d", &option);
        switch (option) {
        case 1:
            Display();
            break;
        case 2:
            printf("Upisite ID stavke koju zelite pronaci: ");
            scanf("%d", &id);
            ItemPoz item = searchItem(id);
            if (item != NULL) {
                printf("Stavka pronadjena: %s, Cjena : %f\n", item->name, item->price);
            }
            else {
                printf("Stavka nije pronadjena!\n");
            }
            break;
        case 3:
            printf("Upisite ime nove stavke: ");
            scanf(" %[^\n]", name);
            printf("upisite cjenu nove stavke: ");
            scanf("%f", &price);
            printf("Upisite ID nove stavke: ");
            scanf("%d", &id);
            insert(name, price, id);
            printf("Uspjesno dodano na meni!\n");
            break;
        case 4:
            printf("\nMolimo upisite ime datoteke za spremanje!:");
            scanf(" %[^\n]", imeDatoteke);
            status = saveOrders(root, imeDatoteke);
            if (status != 0) {
                return STATUS_FAIL;
            }
            break;
        case 5:
            printf("\nMolimo upisite ime datoteke za ucitavanje!:");
            scanf(" %[^\n]", imeDatoteke);
            root = loadOrders(imeDatoteke);
            break;
        case 6:
            status=createNewOrder();
            if (status != 0) {
                return STATUS_FAIL;
            }
            break;
        case 7:
            status=printOrders();
            if (status != 0) {
                return STATUS_FAIL;
            }
            ispisRacuna(root);
            break;
        case 8:
            printf("Ugodan boravak!\n");
            freeList();
            exit(0);
            break;
        default:
            printf("Krivi odabir, pokusajte ponovo!\n");
            break;
        }
    } while (option != 8);
    return 0;
}


// dodavanje nove nar
OrderPoz insertOrder(OrderPoz root, int id, char customer[], float amount, int time) {
    OrderPoz newOrder = (OrderPoz)malloc(sizeof(Order));
    newOrder->id = id;
    strcpy(newOrder->customer, customer);
    newOrder->amount = amount;
    newOrder->time = time;
    newOrder->left = NULL;
    newOrder->right = NULL;

    if (root == NULL) {
        root = newOrder;
    }
    else {
        OrderPoz current = root;
        OrderPoz parent;
        while (1) {
            parent = current;
            if (id < current->id) {
                current = current->left;
                if (current == NULL) {
                    parent->left = newOrder;
                    break;
                }
            }
            else {
                current = current->right;
                if (current == NULL) {
                    parent->right = newOrder;
                    break;
                }
            }
        }
    }
    return root;
}

OrderPoz searchOrder(OrderPoz root, int id) {
    if (root == NULL || root->id == id) {
        return root;
    }

    if (root->id > id) {
        return searchOrder(root->left, id);
    }

    return searchOrder(root->right, id);
}

int printInOrder(OrderPoz root) {
    if (root != NULL) {
        printInOrder(root->left);
        printf("ID narudzbe: %d\nKorisnik: %s\nIznos: %.2fEUR/%.2fHRK\nVrijeme: %d\n\n", root->id, root->customer, root->amount, root->amount * TECAJ, root->time);
        printInOrder(root->right);
    }

    return SUCCESS;
}

int freeTree(OrderPoz root) {
    if (root != NULL) {
        freeTree(root->left);
        freeTree(root->right);
        free(root);
    }

    return SUCCESS;
}

int saveOrders(OrderPoz root, char* fileName) {
    FILE* file = NULL;
    file = fopen(fileName, "w");
    if (file == NULL) {
        printf("Greska u otvaranju datoteke\n");
        return FILE_FAIL;
    }

    writeOrders(root, file);

    fclose(file);
    printf("Uspjesno spremanje narudzbi u %s\n", fileName);

    return SUCCESS;
}

int writeOrders(OrderPoz root, FILE* file) {
    if (root != NULL) {
        writeOrders(root->left, file);
        fprintf(file, "%d,%s,%.2f,%d\n", root->id, root->customer, root->amount, root->time);
        writeOrders(root->right, file);
    }

    return SUCCESS;
}

OrderPoz loadOrders(char* fileName) {
    FILE* file = fopen(fileName, "r");
    if (file == NULL) {
        printf("Greska u otvaranju datoteke\n");
        return NULL;
    }

    OrderPoz root = NULL;
    char line[100];
    while (fgets(line, sizeof(line), file)) {
        char* id = strtok(line, ",");
        char* customer = strtok(NULL, ",");
        char* amount = strtok(NULL, ",");
        char* time = strtok(NULL, ",");

        root = insertOrder(root, atoi(id), customer, atof(amount), atoi(time));
    }

    fclose(file);
    printf("Uspjesno citanje iz %s\n", fileName);
    return root;
}

int insert(char name[], float price, int id) {
    ItemPoz newItem = (ItemPoz)malloc(sizeof(struct Item));
    strcpy(newItem->name, name);
    newItem->price = price;
    newItem->id = id;
    newItem->next = NULL;

    if (head == NULL) {
        head = newItem;
    }
    else {
        ItemPoz current = head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newItem;
    }

    return SUCCESS;
}


void Display() {
    printf("Meni:\n\n");
    ItemPoz current = head;
    while (current != NULL) {
        printf("Stavka: %s\nCjena: %.2fEUR/%.2fHRK\nID: %d\n\n", current->name, current->price, current->price * TECAJ, current->id);
        current = current->next;
    }
}

void freeList() {
    ItemPoz current = head;
    while (current != NULL) {
        ItemPoz next = current->next;
        free(current);
        current = next;
    }
    head = NULL;
}

ItemPoz getnewNode(char name[], float price, int id) {
    ItemPoz newItem = (ItemPoz)malloc(sizeof(Item));
    strcpy(newItem->name, name);
    newItem->price = price;
    newItem->id = id;
    newItem->next = NULL;
    return newItem;
}

ItemPoz searchItem(int id) {
    ItemPoz current = head;
    while (current != NULL) {
        if (current->id == id) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

int createNewOrder() {
    int id;
    char customer[100];
    float amount = 0;
    int time;
    int itemId;
    int qty;

    id = generateRandomNum(1, 999);
    printf("Unesite ime korisnika za novu narudzbu: ");
    scanf(" %[^\n]", customer);
    printf("Uneiste vrijeme narudzbe: ");
    scanf("%d", &time);

    while (1) {
        Display();
        printf("Unesite ID stavke za dodavanje: ");
        scanf("%d", &itemId);

        ItemPoz item = searchItem(itemId);
        if (item == NULL) {
            printf("Stavka nije pronadjena, pokusajte ponovo\n");
            continue;
        }

        printf("Unesite kolicinu: ");
        scanf("%d", &qty);
        amount += item->price * qty;

        printf("Stavka dodana u narudzbu: %s, Price: %f, Qty: %d\n", item->name, item->price, qty);
        printf("Zelite li dodati jos stavki (1-da, 0-ne): ");
        int choice;
        scanf("%d", &choice);
        if (choice == 0) {
            break;
        }
    }
    root = insertOrder(root, id, customer, amount, time);
    printf("Uspjesno naruceno! Ukupni iznos: %f\n", amount);

    return SUCCESS;
}

int printOrders() {
    if (root == NULL) {
        printf("Nisu pronadene narudzbe.\n");
        return;
    }
    printf("Narudzbe:\n");
    printInOrder(root);

    return SUCCESS;
}

int generateRandomNum(int min, int max) {
    return (rand() % (max - min + 1)) + min;
}

void ispisRacuna(OrderPoz root) {
    char imeDatoteke[100];
    FILE* fp = NULL;

    printf("\nUnesite ime datoteke u koju ce se ispisat racun: ");
    scanf(" %[^\n]", imeDatoteke);

    fp = fopen(imeDatoteke, "w");
    if (fp == NULL) {
        printf("\nGreska u otvaranju datoteke!");
        return FILE_FAIL;
    }

    ispisRek(root, fp);
    fclose(fp);
}

void ispisRek(OrderPoz root, FILE* fp) {
    if (root != NULL) {
        printInOrder(root->left);
        fprintf(fp, "ID narudzbe: %d\nKorisnik: %s\nIznos: %.2fEUR/%.2fHRK\nVrijeme: %d\n\n", root->id, root->customer, root->amount, root->amount * TECAJ, root->time);
        printInOrder(root->right);
    }
}

void PrintMenu() {
    printf("Dobrodosli u restoran! Odaberite:\n\n");
    printf("1. Prikaz menija\n");
    printf("2. Trazenje stavke po ID-u\n");
    printf("3. Dodavanje nove stavke na meni\n");
    printf("4. Spremanje narudzbi u datoteku\n");
    printf("5. Ucitavanje narudzbi iz datoteke\n");
    printf("6. Unosenje narudzbi\n");
    printf("7. Ispis narudzbi\n");
    printf("8. Izlaz\n\n");
}