#ifndef USER_SOURCE
#define USER_SOURCE

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

int takeTestAll(position root, int numQuestions, char* name, double* time) {
    char userAnswer[MAX_LEN];
    int numCorrectAnswers = 0;
    clock_t start = clock();

    int *visited = (int *)calloc(numQuestions, sizeof(int));
    position *questions = (position *)malloc(numQuestions * sizeof(position));
    int question_count = 0;
    position current = root;
    
    getQuestions(current, questions, &question_count);
    int question_index;
    for (int i = 0; i < numQuestions; i++) {
        while (1) {
            question_index = rand() % numQuestions;
            if (visited[question_index] == 0) {
                visited[question_index] = 1;
                break;
            }
        }
        printf("Pitanje: %s\n", questions[question_index]->question);
        printf("Upisi odgovor: ");
        scanf(" %[^\n]", userAnswer);
        if (checkAnswer(questions[question_index], userAnswer)) {
            numCorrectAnswers++;
        }
        else {
            printf("Pogresan odgovor, tocan odgovor je: %s.\n", questions[question_index]->answer);
        }
    }
    clock_t end = clock();
    *time = (double)(end - start) / CLOCKS_PER_SEC;
    printf("\nIgrac %s je odgovorio na %d pitanja od %d. Ukupno vrijeme: %lf sekundi\n", name, numCorrectAnswers, countnodes(root), *time);
    return numCorrectAnswers;
}

void getQuestions(position current, position *questions, int *question_count) {
    if (current == NULL) {
        return;
    }
    questions[(*question_count)++] = current;
    getQuestions(current->left, questions, question_count);
    getQuestions(current->right, questions, question_count);
}

int checkAnswer(position current, char* userAnswer) {
    if (strcmp(userAnswer, current->answer) == 0) {
        return 1;
    }
    return 0;
}

position loadQuestionsFromFile(position root) {
    FILE* fp = NULL;
	char buffer[FILENAME_MAX];
	
	printf("\nMolimo upisite ime predmeta pitanja: ");
	scanf(" %[^\n]", buffer);
	
	strcat(buffer, ".txt");
	
    fp = fopen(buffer, "r");
    if (fp == NULL) {
        printf("Greska u otvaranju datoteke!\n");
        return NULL;
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

player_position createPlayer(player_position p) {
    p = (player_position)malloc(sizeof(player));
    if (p == NULL) {
        printf("Greska u alokaciji memorije!\n");
        return ALLOC_FAIL_POZ;
    }
    p->next = NULL;
    return p;
}

player_position insertPlayer(player_position leaderboard, player_position newPlayer) {
    if (leaderboard == NULL) {
        return newPlayer;
    }

    if (newPlayer->points > leaderboard->points) {
        newPlayer->next = leaderboard;
        return newPlayer;
    }

    leaderboard->next = insertPlayer(leaderboard->next, newPlayer);
    return leaderboard;
}

void saveLeaderboardToFile(player_position leaderboard, int numQuestions) {
    char buffer[TEXT]={0};
    printf("\nUpisite ime datoteke za spremanje rezultata: ");
    scanf(" %[^\n]", buffer);
    strcat(buffer,".csv");
    
    FILE* fp = fopen(buffer, "w");
    if (fp == NULL) {
        printf("Greska u otvaranju datoteke!\n");
        return;
    }

    fprintf(fp, "Ime,Vrijeme,Broj tocnih odgovora,Broj pitanja,Prosjecno vrijeme odgovora,Postotak,Bodovi\n");

    player_position current = leaderboard;
    while (current != NULL) {
        current->percentage=(double)current->numCorrectAnswers*100/numQuestions;
        current->points=(double)current->percentage * (10*numQuestions / current->time);
        fprintf(fp, "%s,%.2lfs,%d,%d,%.2lfs,%.2lf%,%.2lf\n"
                    , current->name, current->time, current->numCorrectAnswers, numQuestions, current->averageAnswer, current->percentage,current->points);
        current = current->next;
    }

    fclose(fp);
}

int countnodes(position root) {
    if (root == NULL) {
        return 0;
    }
    return 1 + countnodes(root->left) + countnodes(root->right);
}

void printMenu() {
	printf("\n1.) Odabir predmeta");
    printf("\n2.) Novo pokretanje kviza");
	printf("\n3.) Izlaz");
}

#endif