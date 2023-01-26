#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define SUCCESS (0)
#define PROGRAM_EXIT (-1)
#define ALLOC_FAIL_POZ (NULL)
#define ALLOC_FAIL (-1)
#define MAX_LEN (150)
#define TEXT (100)
#define EMPTY (0)
#define ERROR (-1)
#define FILE_FAIL (-1)

int count = 0;
int status = 0;

#include "userStructures.h"
#include "userFunctions.h"
#include "userSource.c"

int main() {
    position root = NULL;
    player_position leaderboard = NULL;
    int numQuestions;
    position newNode = NULL;
    int choice = 0;
    int numCorrectAnswers = 0;
    char fileName[FILENAME_MAX] = {0};

    while (1)
    {
        printMenu();
        printf("\nUnesi odabir: ");
        scanf(" %d", &choice);
		
        switch (choice) {
        case 1:
        {
            printf("\nUnesite ime datoteke za ucitavanje pitanja: ");
            scanf(" %[^\n]", fileName);
            root=loadQuestionsFromFile(root,fileName);
			if(countnodes(root)==0){
                printf("\nNema unesenih pitanja, molimo ponovo odaberite predmet!");
                break;
            }
            else if(countnodes(root)>0){
                printf("\nPitanja uspjesno unesena!");
            }
            break;
        }
        case 2:
        {
            numQuestions=countnodes(root);
            if(numQuestions==0){
                printf("\nNema unesenih pitanja, molimo ponovo odaberite datoteku s pitanjima, odnosno predmet!");
                break;
            }

            player_position newPlayer = NULL;
            newPlayer = createPlayer(newPlayer);
            if (newPlayer == NULL) {
                printf("\nGreska u alociranju memorije!\n");
                return ERROR;
            }

            printf("\nUpisi svoje ime: ");
            scanf(" %[^\n]", newPlayer->name);

            numCorrectAnswers = takeTestAll(root, numQuestions, newPlayer->name, &newPlayer->time);
            newPlayer->numCorrectAnswers = numCorrectAnswers;
            newPlayer->averageAnswer = newPlayer->time / numQuestions;
            leaderboard = insertPlayer(leaderboard, newPlayer);

            break;
        }
        case 3:
        {
            if(leaderboard != NULL){ 
                status = saveLeaderboardToFile(leaderboard, countnodes(root), fileName);
            }
            if(status != 0){
                printf("\nGreska u spremanju tablice rezultata!");
                return ALLOC_FAIL;
            }

            printf("\nHvala na koristenju, vidimo se!\n");

            status = deleteAllPlayers(leaderboard);
            if(status != 0){
                printf("\nGreska u oslobadjanju memorije!");
                return ALLOC_FAIL;
            }

            deleteTree(root);

            return SUCCESS;
        }
        default:
        {
            printf("\nKrivi odabir, birajte ponovo!");
            break;
        }
        }
    }
    return SUCCESS;
}
