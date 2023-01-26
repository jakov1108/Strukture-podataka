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

int count = 0;
int status = 0;

#include "adminStructures.h"
#include "adminFunctions.h"
#include "adminSource.c"

int main(){
	
	position root = NULL;
    int numQuestions;
    position newNode = NULL;
    int choice = 0;
    int numCorrectAnswers = 0;

    char password[TEXT]="admin";
    char temp[TEXT]={0};

    while (1) {
        printf("\nUpisite lozinku za pristup admin postavkama, za izlaz unesite 'izlaz': ");
        scanf(" %s", temp);

        if(strcmp(password,temp)==0){
            printf("\nUspjesno ste prijavljeni!");
            break;
        }
        else if((strcmp("izlaz",temp)==0)){
            printf("\nHvala na koristenju!\n");
            return EXIT;
        }
        else{
            printf("\nLozinka pogresna, pokusajte ponovo!");
        }
    }
	
	while (1)
    {
		printMenu();
        printf("\nUnesi odabir: ");
        scanf(" %d", &choice);
			    
        switch (choice) {
        case 1:
        {
            printInstructions();
            break;
        }
        case 2:
        {
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

                printf("Upisi opciju a): ", i + 1);
                scanf(" %[^\n]", newNode->a);

                printf("Upisi opciju b): ", i + 1);
                scanf(" %[^\n]", newNode->b);

                printf("Upisi opciju c): ", i + 1);
                scanf(" %[^\n]", newNode->c);

                printf("Upisi opciju d): ", i + 1);
                scanf(" %[^\n]", newNode->d);

                printf("Upisi slovo ispred tocnog odgovora za br. %d: ", i + 1);
                scanf(" %c", &(newNode->answer));

                root = insertNode(root, newNode);
            }
            break;
        }
        case 3:
        {
            char questionTemp[MAX_LEN+1]={0};
            status=printQuestions(root);
            if(status != SUCCESS){
                printf("\nGreska u izvodjenju programa!");
                return ERROR;
            }
            printf("\nUnesite tekst pitanja koje zelite ukloniti: ");
            scanf(" %[^\n]", questionTemp);
            root = deleteQuestion(root, questionTemp);
            printf("\nPitanje uspjesno uklonjeno!");
            break;
        }
        case 4:
        {
            printf("\nIspis trenutnih pitanja: ");
            status=printQuestions(root);
            if(status != SUCCESS){
                printf("\nGreska u izvodjenju programa!");
                return ERROR;
            }
            break;
        }
        case 5:
        {
            status = saveQuestionsToFile(root);
            if(status != SUCCESS){
                printf("\nGreska u izvodjenju programa!");
                return ERROR;
            }
            break;
        }
        case 6:
        {
            if(countnodes(root) != 0){
                printf("\nVec su ucitana pitanja!");
                break;
            }
            root=loadQuestionsFromFile(root);
            if(countnodes(root) != 0){
                printf("\nPitanja uspjesno ucitana!");
            }
            break;
        }
        case 7:
        {
            printf("\nHvala na koristenju, ugodan dan!\n");
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
