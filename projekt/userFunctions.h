#ifndef USER_FUNCTIONS
#define USER_FUNCTIONS

position createNode(position);
position insertNode(position, position);
int takeTestAll(position, int, char*, double*);
int checkAnswer(position, char);
position loadQuestionsFromFile(position);
void printMenu();
int countnodes(position);
player_position createPlayer(player_position);
player_position insertPlayer(player_position, player_position);
int saveLeaderboardToFile(player_position, int);
int getQuestions(position, position*, int*);
int deleteAllPlayers(player_position);
void deleteTree(position);

#endif
