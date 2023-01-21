#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "kvizHeader.h"
#include "kvizSource.c"

int main() {
    position root = NULL;
    player_position leaderboard = NULL;
    int numQuestions;
    position newNode = NULL;
    int choice = 0;
    int numCorrectAnswers = 0;
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
                newNode = NULL;
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
            player_position newPlayer = NULL;
            newPlayer = createPlayer(newPlayer);
            if (newPlayer == NULL) {
                printf("Greska u alociranju memorije!\n");
                return ERROR;
            }
            printf("Upisi svoje ime: ");
            scanf(" %s", newPlayer->name);
            numCorrectAnswers = takeTestAll(root, numQuestions, newPlayer->name, &newPlayer->time);
            newPlayer->numCorrectAnswers = numCorrectAnswers;
            newPlayer->fastestAnswer = newPlayer->time / numQuestions;
            leaderboard = insertPlayer(leaderboard, newPlayer);
            break;
        case 3:
            status = saveQuestionsToFile(root);
            if(status != SUCCESS){
                printf("\nGreska u izvodjenju programa!");
                return ERROR;
            }
            break;
        case 4:
            root=loadQuestionsFromFile(root);
            break;
        case 5:
            saveLeaderboardToFile(leaderboard);
            printf("\nHvala na koristenju, vidimo se dogodine!");
            return SUCCESS;
        default:
            printf("\nKrivi odabir, birajte ponovo!");
            break;
        }
    }
    return SUCCESS;
}
