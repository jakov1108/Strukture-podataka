#include "kvizAdminHeader.h"
#include "kvizAdminSource.c"

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

                printf("Upisi tocan odgovor za br. %d: ", i + 1);
                scanf(" %[^\n]", newNode->answer);

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
            root=loadQuestionsFromFile(root);
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
