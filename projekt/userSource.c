#ifndef USER_SOURCE
#define USER_SOURCE

position createNode(position p) {
    p = (position)malloc(sizeof(node));
    if (p == NULL) {
        printf("\nGreska u alokaciji memorije!\n");
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
    char userAnswer;
    int numCorrectAnswers = 0;
    clock_t start = clock();

    int *visited = (int *)calloc(numQuestions, sizeof(int));
    position *questions = (position *)malloc(numQuestions * sizeof(position));
    int question_count = 0;
    position current = root;
    
    status = getQuestions(current, questions, &question_count);
    if(status != 0){
        printf("\nGreska u izvodjenju testa!");
        return ALLOC_FAIL;
    }
    int question_index;
    for (int i = 0; i < numQuestions; i++) {
        while (1) {
            question_index = rand() % numQuestions;
            if (visited[question_index] == 0) {
                visited[question_index] = 1;
                break;
            }
        }
        printf("\nPitanje: %s\n", questions[question_index]->question);
        printf("a): %s\n", questions[question_index]->a);
        printf("b): %s\n", questions[question_index]->b);
        printf("c): %s\n", questions[question_index]->c);
        printf("d): %s\n", questions[question_index]->d);
        printf("Upisi slovo ispred tocnog odgovora: ");
        scanf(" %c", &userAnswer);
        if (checkAnswer(questions[question_index], userAnswer)) {
            numCorrectAnswers++;
            printf("Tocno!\n");
        }
        else {
            printf("Pogresan odgovor, tocan odgovor je pod %c).\n", questions[question_index]->answer);
        }
    }
    clock_t end = clock();
    *time = (double)(end - start) / CLOCKS_PER_SEC;
    printf("\nIgrac %s je odgovorio na %d pitanja od %d. Ukupno vrijeme: %lf sekundi\n", name, numCorrectAnswers, countnodes(root), *time);
    free(questions);
    free(visited);
    return numCorrectAnswers;
}

int getQuestions(position current, position *questions, int *question_count) {
    if (current == NULL) {
        return SUCCESS;
    }
    questions[(*question_count)++] = current;
    getQuestions(current->left, questions, question_count);
    getQuestions(current->right, questions, question_count);

    return SUCCESS;
}

int checkAnswer(position current, char userAnswer) {
    if (tolower(current->answer) == tolower(userAnswer)) {
        return 1;
    }
    return 0;
}

int printBest(player_position p){
    
    player_position top = p;

    while(p != NULL){
        if(p->points > top->points){
            top = p;
        }
        p = p->next;
    }

    printf("\nNajbolji rezultat ostvario je %s sa postignutih %lf bodova!", top->name, top->points);

    return SUCCESS;
}

int printFastest(player_position p){
    
    player_position min = p;

    while(p != NULL){
        if(p->time < min->time){
            min = p;
        }
        p = p->next;
    }

    printf("\nKviz je najbrze rjesio %s u %.2lf sekundi.", min->name, min->time);

    return SUCCESS;
}

position loadQuestionsFromFile(position root, char fileName[FILENAME_MAX]) {
    FILE* fp = NULL;
	char buffer[FILENAME_MAX];

    strcat(buffer, fileName);
	
	strcat(buffer, ".que");
	
    fp = fopen(buffer, "r");
    if (fp == NULL) {
        printf("Greska u otvaranju datoteke!\n");
        return NULL;
    }

    position newNode = NULL;
    char question[MAX_LEN];
    char a[TEXT];
    char b[TEXT];
    char c[TEXT];
    char d[TEXT];
    char answer;
    while (fscanf(fp, " %[^\n] %[^\n] %[^\n] %[^\n] %[^\n] %c", question, a, b, c, d, &answer) != EOF) {
        newNode = NULL;
        newNode = createNode(newNode);
        if (newNode == NULL) {
            printf("Greska u alociranju memorije!\n");
            return ALLOC_FAIL_POZ;
        }
        strcpy(newNode->question, question);
        strcpy(newNode->a,a);
        strcpy(newNode->b,b);
        strcpy(newNode->c,c);
        strcpy(newNode->d,d);
        newNode->answer = answer;
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

int saveLeaderboardToFile(player_position leaderboard, int numQuestions, char fileName[FILENAME_MAX]) {
    char buffer[TEXT]={0};
    FILE* fp = NULL;
    strcat(buffer,fileName);
    strcat(buffer," rezultati");
    strcat(buffer,".csv");

    fp = fopen(buffer, "a+");
    fseek(fp, 0, SEEK_END);
    if (fp != NULL) {
        player_position current = leaderboard;
        
        if (ftell(fp)==0){
            fprintf(fp, "Ime,Vrijeme,Broj tocnih odgovora,Broj pitanja,Prosjecno vrijeme odgovora,Postotak,Bodovi\n");
        }

        while (current != NULL) {
            current->percentage=(double)current->numCorrectAnswers*100/numQuestions;
            current->points=(double)(current->percentage / 100) * (numQuestions / (current->time / 60)) * 10;
            fprintf(fp, "%s,%.2lfs,%d,%d,%.2lfs,%.2lf%,%.2lf\n"
                        , current->name, current->time, current->numCorrectAnswers, numQuestions, current->averageAnswer, current->percentage,current->points);
            current = current->next;
        }
    }

    fclose(fp);

    return SUCCESS;
}

int deleteAllPlayers(player_position head) {
    player_position current = head;
    player_position next;

    while (current != NULL) {
        next = current->next;
        free(current);
        current = next;
    }
    head = NULL;

    return SUCCESS;
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

void deleteTree(position root) {
    if (root == NULL) {
        return;
    }
    deleteTree(root->left);
    deleteTree(root->right);
    free(root);
}

#endif
