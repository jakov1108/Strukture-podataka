#ifndef ADMIN_HEAD
#define ADMIN_HEAD

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define SUCCESS (0)
#define PROGRAM_EXIT (-1)
#define ALLOC_FAIL_POZ (NULL)
#define MAX_LEN (150)
#define TEXT (100)
#define EXIT (0)
#define EMPTY (0)
#define ERROR (-1)
#define FILE_FAIL (-1)

typedef struct node* position;
struct node {
    char question[MAX_LEN];
    char answer[MAX_LEN];
    position left;
    position right;
};
typedef struct node node;

typedef struct player* player_position;
struct player {
    char name[MAX_LEN];
    double time;
    int numCorrectAnswers;
    //int numQuestions;
    double averageAnswer;
    player_position next;
};
typedef struct player player;

position createNode(position);
position insertNode(position, position);
int saveQuestionsToFile(position);
int saveQuestionsHelper(position, FILE*);
position loadQuestionsFromFile(position);
void printMenu();
int countnodes(position);
int printQuestions(position);
void printInstructions();
position deleteQuestion(position, char*);
position findMinNode(position);
void deleteTree(position);

int count = 0;
int status = 0;

#endif
