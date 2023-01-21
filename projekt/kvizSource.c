#ifndef KVIZ_SOURCE
#define KVIZ_SOURCE

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
    void getQuestions(position current) {
        if (current == NULL) {
            return;
        }
        questions[question_count++] = current;
        getQuestions(current->left);
        getQuestions(current->right);
    }
    getQuestions(current);
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
    printf("\nIgrač %s je odgovorio na %d pitanja od %d. Ukupno vrijeme: %lf sekundi\n", name, numCorrectAnswers, numQuestions, *time);
    return numCorrectAnswers;
}


int checkAnswer(position current, char* userAnswer) {
    if (strcmp(userAnswer, current->answer) == 0) {
        return 1;
    }
    return 0;
}

int saveQuestionsToFile(position root) {
    FILE* fp = NULL;
    fp = fopen("pitanja.txt", "w");
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
}

position loadQuestionsFromFile(position root) {
    FILE* fp = NULL;
    fp = fopen("pitanja.txt", "r");
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

    if (newPlayer->time < leaderboard->time) {
        newPlayer->next = leaderboard;
        return newPlayer;
    }

    leaderboard->next = insertPlayer(leaderboard->next, newPlayer);
    return leaderboard;
}

void saveLeaderboardToFile(player_position leaderboard) {
    FILE* fp = NULL;
    fp = fopen("leaderboard.txt", "w");
    if (fp == NULL) {
        printf("Greska u otvaranju datoteke!\n");
        return;
    }

    player_position current = leaderboard;
    while (current != NULL) {
        fprintf(fp, "Igrač: %s, Broj točnih odgovora: %d, Ukupno vrijeme: %lf, Najbrži odgovor: %lf\n", current->name, current->numCorrectAnswers, current->time, current->fastestAnswer);
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
    printf("\n\nIzaberi opciju:");
    printf("\n1 - Unos pitanja");
    printf("\n2 - Izvodjenje testa");
    printf("\n3 - Spremanje pitanja u datoteku");
    printf("\n4 - Ucitavanje pitanja iz datoteke");
    printf("\n5 - Izlaz iz programa\n");
}

#endif