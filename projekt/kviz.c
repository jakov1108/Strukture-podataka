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
            printf("Enter the number of questions: ");
            scanf(" %d", &numQuestions);
            for (int i = 0; i < numQuestions; i++) {
                position newNode = NULL;
                newNode = createNode(newNode);
                if (newNode == NULL) {
                    printf("Error allocating memory!\n");
                    return PROGRAM_EXIT;
                }

                printf("Enter question %d: ", i + 1);
                scanf(" %[^\n]", newNode->question);

                printf("Enter the correct answer for question %d: ", i + 1);
                scanf(" %[^\n]", newNode->answer);

                root = insertNode(root, newNode);
            }
            break;
        case 2:
            takeTestAll(root,numQuestions);
            break;
        case 3:
            saveQuestionsToFile(root);
            break;
        case 4:
            loadQuestionsFromFile(root);
            break;
        case 5:
            exit(0);
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
        printf("Error allocating memory!\n");
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

    printf("\nStart taking the test:\n");
    while (root != NULL) {
        printf("%s\n", root->question);
        printf("Your answer: ");
        scanf(" %[^\n]", userAnswer);

        if (checkAnswer(root, userAnswer)) {
            printf("Correct!\n");
            score++;
        }
        else {
            printf("Incorrect. The correct answer is %s\n", root->answer);
        }

        root = root->left;
    }

    printf("\nTest completed. You scored %d out of %d questions.\n", score, numQuestions);
    printf("Your percentage is: %.2f%%\n", (score / (float)numQuestions) * 100);
}

int checkAnswer(position root, char* userAnswer) {
    return strcmp(root->answer, userAnswer) == 0;
}

void takeTestAll(position root, int numQuestions) {
    char userAnswer[MAX_LEN];
    int score = 0;

    printf("\nStart taking the test:\n");
    if (root == NULL) {
        printf("Error: tree is empty!");
        return;
    }
    takeTestHelperAll(root, &score);
    printf("\nTest completed. You scored %d out of %d questions.\n", score, numQuestions);
    printf("Your percentage is: %.2f%%\n", (score / (float)numQuestions) * 100);
}

void takeTestHelperAll(position root, int* score) {
    if (root == NULL) {
        return;
    }

    char userAnswer[TEXT];

    takeTestHelperAll(root->left, score);
    printf("%s\n", root->question);
    printf("Your answer: ");
    scanf(" %[^\n]", userAnswer);

    if (checkAnswer(root, userAnswer)) {
        printf("Correct!\n");
        (*score)++;
    }
    else {
        printf("Incorrect. The correct answer is %s\n", root->answer);
    }
    takeTestHelperAll(root->right, score);
}


void saveQuestionsToFile(position root) {

    printf("\nUnesi ime datoteke za spremanje pitanja:");
    char fileName[FILENAME_MAX];
    scanf(" %[^\n]", fileName);
    
    FILE* fp = fopen(fileName, "w");
    if (fp == NULL) {
        printf("Error opening file!\n");
        return;
    }

    saveQuestionsHelper(root, fp);
    fclose(fp);
    printf("Questions saved to %s\n", fileName);
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
    printf("\nUnesi ime datoteke za ucitavanje pitanja:");
    char fileName[FILENAME_MAX];
    scanf(" %[^\n]", fileName);

    FILE* file = fopen(fileName, "r");
    if (file == NULL) {
        printf("Error opening file!\n");
        return root;
    }

    char question[MAX_LEN];
    char answer[MAX_LEN];
    while (fscanf(file, "%s %s", question, answer) != EOF) {
        position newNode = NULL;
        newNode= createNode(newNode);
        if (newNode == NULL) {
            printf("Error allocating memory!\n");
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
