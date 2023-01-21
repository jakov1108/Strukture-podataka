#ifndef KVIZ_HEADER
#define KVIZ_HEADER

#define SUCCESS (0)
#define PROGRAM_EXIT (-1)
#define ALLOC_FAIL_POZ (NULL)
#define MAX_LEN (150)
#define TEXT (100)
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

position createNode(position);
position insertNode(position, position);
int takeTestAll(position, int, char*, double*);
int checkAnswer(position, char*);
int saveQuestionsToFile(position);
int saveQuestionsHelper(position, FILE*);
position loadQuestionsFromFile(position);
void printMenu();
//int takeTestAll(position,int);
int takeTestHelperAll(position, int*);
int countnodes(position);

typedef struct player* player_position;

struct player {
    char name[MAX_LEN];
    double time;
    int numCorrectAnswers;
    int numQuestions;
    double fastestAnswer;
    player_position next;
};

typedef struct player player;

player_position createPlayer(player_position);
player_position insertPlayer(player_position, player_position);
void saveLeaderboardToFile(player_position);

int count = 0;
int status = 0;

#endif