#include "kvizUserHeader.h"
#include "kvizUserSource.c"

int main() {
    position root = NULL;
    player_position leaderboard = NULL;
    int numQuestions;
    position newNode = NULL;
    int choice = 0;
    int numCorrectAnswers = 0;

    while (1)
    {
        printMenu();
        printf("\nUnesi odabir: ");
        scanf(" %d", &choice);
		
        switch (choice) {
        case 1:
        {
            root=loadQuestionsFromFile(root);
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
                printf("Greska u alociranju memorije!\n");
                return ERROR;
            }
            printf("Upisi svoje ime: ");
            scanf(" %[^\n]", newPlayer->name);
            numCorrectAnswers = takeTestAll(root, numQuestions, newPlayer->name, &newPlayer->time);
            newPlayer->numCorrectAnswers = numCorrectAnswers;
            newPlayer->averageAnswer = newPlayer->time / numQuestions;
            leaderboard = insertPlayer(leaderboard, newPlayer);
            break;
        }
        case 3:
        {
            saveLeaderboardToFile(leaderboard, countnodes(root));
            printf("\nHvala na koristenju, vidimo se dogodine!\n");
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
