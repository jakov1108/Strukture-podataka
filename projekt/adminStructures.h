#ifndef ADMIN_STRUCTURES
#define ADMIN_STRUCTURES

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

#endif