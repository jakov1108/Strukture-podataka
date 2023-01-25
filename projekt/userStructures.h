#ifndef USER_STRUCTURES
#define USER_STRUCTURES

struct node;
typedef struct node* position;
struct node {
    char question[MAX_LEN];
    char a[TEXT];
    char b[TEXT];
    char c[TEXT];
    char d[TEXT];
    char answer;
    position left;
    position right;
};
typedef struct node node;

struct player;
typedef struct player* player_position;
struct player {
    char name[MAX_LEN];
    double time;
    int numCorrectAnswers;
    double percentage;
    double points;
    double averageAnswer;
    player_position next;
};
typedef struct player player;

#endif