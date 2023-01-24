#ifndef ADMIN_SOURCE
#define ADMIN_SOURCE

position createNode(position p) {
    p = (position)malloc(sizeof(node));
    if (p == NULL) {
        printf("Greska u alokaciji memorije!\n");
        return ALLOC_FAIL_POZ;
    }
    p->left = NULL;
    p->right = NULL;
    return p;
}

position insertNode(position root, position newNode) {
    if (root == NULL) {
        return newNode;
    }

    if (strcmp(newNode->question, root->question) < 0) {
        root->left = insertNode(root->left, newNode);
    }
    else {
        root->right = insertNode(root->right, newNode);
    }

    return root;
}

int saveQuestionsToFile(position root) {
    FILE* fp = NULL;
	char buffer[FILENAME_MAX];
	
	printf("\nMolimo upisite ime predmeta pitanja: ");
	scanf(" %[^\n]", buffer);
	
	strcat(buffer, ".txt");
	
    fp = fopen(buffer, "w");
    if (fp == NULL) {
        printf("Greska u otvaranju datoteke!\n");
        return FILE_FAIL;
    }

    status = saveQuestionsHelper(root, fp);
    fclose(fp);
    return status;
}

int saveQuestionsHelper(position root, FILE* fp) {
    if (root == NULL) {
        return SUCCESS;
    }

    fprintf(fp, "%s\n%s\n", root->question, root->answer);
    saveQuestionsHelper(root->left, fp);
    saveQuestionsHelper(root->right, fp);

    return SUCCESS;
}

int countnodes(position root) {
    if (root == NULL) {
        return 0;
    }
    return 1 + countnodes(root->left) + countnodes(root->right);
}

position loadQuestionsFromFile(position root) {
    FILE* fp = NULL;
    char buffer[FILENAME_MAX];
	
	printf("\nMolimo upisite ime predmeta za ucitavanje pitanja: ");
	scanf(" %[^\n]", buffer);
	
	strcat(buffer, ".txt");

    fp = fopen(buffer, "r");
    if (fp == NULL) {
        printf("Greska u otvaranju datoteke!\n");
        return ALLOC_FAIL_POZ;
    }

    position newNode = NULL;
    char question[MAX_LEN];
    char answer[MAX_LEN];
    while (fscanf(fp, " %[^\n] %[^\n]", question, answer) != EOF) {
        newNode = NULL;
        newNode = createNode(newNode);
        if (newNode == NULL) {
            printf("Greska u alociranju memorije!\n");
            return ALLOC_FAIL_POZ;
        }
        strcpy(newNode->question, question);
        strcpy(newNode->answer, answer);
        root = insertNode(root, newNode);
    }
    fclose(fp);
    return root;
}

void printMenu(){
    printf("\n\n1.) Upute i informacije");
	printf("\n2.) Unos novih pitanja");
    printf("\n3.) Brisanje pitanja");
    printf("\n4.) Ispis trenutnih pitanja");
	printf("\n5.) Spremi pitanja u datoteku");
	printf("\n6.) Ucitaj pitanja iz datoteke");
    printf("\n7.) Izlaz");
}

int printQuestions(position p) {

    if(p == NULL)
        return SUCCESS;

    printQuestions(p->left);
    printf("\n=========================");
    printf("\nPitanje: %s", p->question);
    printf("\nOdgovor: %s", p->answer);
    printf("\n=========================");
    printQuestions(p->right);

    return SUCCESS;
}

void printInstructions(){
    printf("\nHvala vam na odabiru ovog programa.");
    printf("\nAdmin mod programa je namjenjen profesorima za provjere znanja, ali i u zabavne svrhe.");
    printf("\nIspisom menija dobivate opcije te upisom broja opcije birate jednu od navedenih opcija.");
    printf("\nProfesor u admin modu ima opciju dodavanja pitanja, postavljanja pitanja, brisanja pitanja, te spremanja istih u datoteku.");
    printf("\nProgram je osmisljen tako da ucenici otvaraju user mod,\nodabiru predmet, odnosno temu ispita i krece provjera znanja.");
    printf("\nNa kraju kviza sprema se tablica ucenika i njihovih rezultata, a datoteka se otvara u programu MS Excell.");
}

position deleteQuestion(position root, char* question) {
    if (root == NULL) {
        return root;
    }

    if (strcmp(root->question, question) > 0) {
        root->left = deleteQuestion(root->left, question);
    } else if (strcmp(root->question, question) < 0) {
        root->right = deleteQuestion(root->right, question);
    } else {
        if (root->left == NULL) {
            position temp = root->right;
            free(root);
            return temp;
        } else if (root->right == NULL) {
            position temp = root->left;
            free(root);
            return temp;
        } else {
            position temp = findMinNode(root->right);
            strcpy(root->question, temp->question);
            strcpy(root->answer, temp->answer);
            root->right = deleteQuestion(root->right, temp->question);
        }
    }
    return root;
}


position findMinNode(position root) {
    position current = root;
    while (current->left != NULL) {
        current = current->left;
    }
    return current;
}

void deleteTree(position root) {
    if (root == NULL) {
        return;
    }
    deleteTree(root->left);
    deleteTree(root->right);
    free(root);
}

#endif
