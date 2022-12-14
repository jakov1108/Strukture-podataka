#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SUCCESS (0)
#define PROGRAM_EXIT (-1)
#define ALLOC_FAIL_POZ (NULL)
#define MAX_LEN (150)
#define TEXT 100

typedef struct node* position;

struct node {
    char question[MAX_LEN];
    char answer[MAX_LEN];
    position left;
    position right;
};

typedef struct node node;

position createNode(position);
position insertNode(position, position);
void takeTest(position, int);
int checkAnswer(position, char*);
void saveQuestionsToFile(position);
void saveQuestionsHelper(position, FILE*);
position loadQuestionsFromFile(position);
void printMenu();
void takeTestAll(position,int);
void takeTestHelperAll(position, int*);
int countnodes(position);

int count=0;

int main() {
    position root = NULL;
    int numQuestions;

    int choice = 0;
    printMenu();
    
    while (1)
    {
        printf("\nUnesi odabir: ");
        scanf(" %d", &choice);

        switch (choice) {
        case 1:
            printf("Upisi broj pitanja: ");
            scanf(" %d", &numQuestions);
            for (int i = 0; i < numQuestions; i++) {
                position newNode = NULL;
                newNode = createNode(newNode);
                if (newNode == NULL) {
                    printf("Greska u alociranju memorije!\n");
                    return PROGRAM_EXIT;
                }

                printf("Upisi pitanje br. %d: ", i + 1);
                scanf(" %[^\n]", newNode->question);

                printf("Upisi tocan odgovor za br. %d: ", i + 1);
                scanf(" %[^\n]", newNode->answer);

                root = insertNode(root, newNode);
            }
            break;
        case 2:
            numQuestions=countnodes(root);
            takeTestAll(root,numQuestions);
            break;
        case 3:
            saveQuestionsToFile(root);
            break;
        case 4:
            root=loadQuestionsFromFile(root);
            break;
        case 5:
            printf("\nHvala na koristenju, vidimo se dogodine!");
            return(0);
        default:
            printf("\nKrivi odabir, birajte ponovo!");
            break;
        }
    }



    return SUCCESS;
}

position createNode(position p) {
    p = (position)malloc(sizeof(node));
    if (p == NULL) {
        printf("Greska u alokaciji memorije!\n");
        return ALLOC_FAIL_POZ;
    }
    p->left = NULL;
    p->right = NULL;
    return p;
}


position insertNode(position root, position newNode) {
    if (root == NULL) {
        return newNode;
    }

    if (strcmp(newNode->question, root->question) < 0) {
        root->left = insertNode(root->left, newNode);
    }
    else {
        root->right = insertNode(root->right, newNode);
    }

    return root;
}

void takeTest(position root, int numQuestions) {
    char userAnswer[MAX_LEN];
    int score = 0;

    printf("\nKviz krece, sretno!\n");
    while (root != NULL) {
        printf("%s\n", root->question);
        printf("Odgovor: ");
        scanf(" %[^\n]", userAnswer);

        if (checkAnswer(root, userAnswer)) {
            printf("Tocno!\n");
            score++;
        }
        else {
            printf("Krivo :( Tocan odgovor je %s\n", root->answer);
        }

        root = root->left;
    }

    printf("\nGotovo. Ostvarili ste %d d %d pitanja\n", score, numQuestions);
    printf("Postotak: %.2f%%\n", (score / (float)numQuestions) * 100);
}

int checkAnswer(position root, char* userAnswer) {
    return strcmp(root->answer, userAnswer) == 0;
}

void takeTestAll(position root, int numQuestions) {
    char userAnswer[MAX_LEN];
    int score = 0;

    printf("\nKviz krece!\n");
    if (root == NULL) {
        printf("Greska! Stablo je prazno!");
        return;
    }
    takeTestHelperAll(root, &score);
    printf("\nTest je gotov, ostvarili ste %d od %d pitanja.\n", score, numQuestions);
    printf("Postotak: %.2f%%\n", (score / (float)numQuestions) * 100);
}

void takeTestHelperAll(position root, int* score) {
    if (root == NULL) {
        return;
    }

    char userAnswer[TEXT];

    takeTestHelperAll(root->left, score);
    printf("%s\n", root->question);
    printf("Vas odgovor: ");
    scanf(" %[^\n]", userAnswer);

    if (checkAnswer(root, userAnswer)) {
        printf("Tocno!\n");
        (*score)++;
    }
    else {
        printf("Krivo, tocan odgovor je: %s\n", root->answer);
    }
    takeTestHelperAll(root->right, score);
}


void saveQuestionsToFile(position root) {

    printf("\nUnesi ime datoteke za spremanje pitanja: ");
    char fileName[FILENAME_MAX];
    scanf(" %[^\n]", fileName);
    
    FILE* fp = fopen(fileName, "w");
    if (fp == NULL) {
        printf("Greska pri otvaranju datoteke!\n");
        return;
    }

    saveQuestionsHelper(root, fp);
    fclose(fp);
    printf("Pitanja su spremljena u: %s\n", fileName);
}

void saveQuestionsHelper(position root, FILE* fp) {
    if (root == NULL) {
        return;
    }

    saveQuestionsHelper(root->left, fp);
    fprintf(fp, "%s\n", root->question);
    fprintf(fp, "%s\n", root->answer);
    saveQuestionsHelper(root->right, fp);
}

position loadQuestionsFromFile(position root) {
    printf("\nUnesi ime datoteke za ucitavanje pitanja: ");
    char fileName[FILENAME_MAX];
    scanf(" %[^\n]", fileName);

    FILE* file = fopen(fileName, "r");
    if (file == NULL) {
        printf("Greska u otvaranju datoteke!\n");
        return root;
    }

    char question[MAX_LEN];
    char answer[MAX_LEN];
    while (fscanf(file, "%s %s", question, answer) != EOF) {
        position newNode = NULL;
        newNode= createNode(newNode);
        if (newNode == NULL) {
            printf("Greska u alociranju memorije!\n");
            return root;
        }

        strcpy(newNode->question, question);
        strcpy(newNode->answer, answer);

        root = insertNode(root, newNode);
    }

    fclose(file);
    return root;
}

void printMenu() {
    printf("\nOdaberi opciju:");
    printf("\n1. Unos pitanja");
    printf("\n2. Rjesavanje kviza");
    printf("\n3. Spremanje u datoteku"); 
    printf("\n4. Ucitavanje iz datoteke");
    printf("\n5. Izlaz");
}

int countnodes(position root)
{
    if(root != NULL)
    {
        countnodes(root->left);
        count++;
        countnodes(root->right);
    }
    return count;
}
