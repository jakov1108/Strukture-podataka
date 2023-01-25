#ifndef ADMIN_FUNCTIONS
#define ADMIN_FUNCTIONS

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

#endif
