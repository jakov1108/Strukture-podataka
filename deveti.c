/*
Zadan je niz brojeva 2, 5, 7, 8, 11, 1, 4, 2, 3, 7 koji su spremljeni u čvorove binarnog stabla.
a) Napisati funkciju insert koja dodaje element u stablo tako da se pozivima te funkcije za
sve element zadanog niza brojeva stvori stablo kao na slici Slika 1. Funkcije vraća
pokazivač na korijen stabla.
b) Napisati funkciju replace koja će svaki element stabla zamijeniti sumom elemenata u
njegovom lijevom i desnom podstablu (tj. sumom svih potomaka prije zamjene
vrijednosti u tim potomcima). Npr. stablo sa slike Slika 1 transformirat će se u stablo na
slici Slika 2.
c) Prepraviti program na način da umjesto predefiniranog cjelobrojnog polja korištenjem
funkcije rand() generira slučajne brojeve u rasponu <10, 90>. Također, potrebno je
upisati u datoteku sve brojeve u inorder prolasku nakon korištenja funkcije iz a), zatim b)
dijela zadatka.
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <malloc.h>
#include <time.h>

#define minimum 10
#define maximum 90

struct _lista;
typedef struct _lista* pozicija;
typedef struct _lista {

    int broj;
    pozicija Next;
}lista;

struct _stablo;
typedef struct _stablo* poz;
typedef struct _stablo {

    int broj;
    poz Left;
    poz Right;
}stablo;

int Meni(poz S, pozicija P, pozicija P1, int Niz[], int X);     
poz Insert1(poz S, int X);                                    
int GiveRandNumbers(int X, pozicija P, poz S);                  
pozicija CreateListElement(int X);                              
int InsertIntoList(pozicija P, pozicija S);                     
int PrintList(pozicija P);                                      
poz AddElementIntoTree(int X, poz S);                           
int PrintInorder(poz S);                                        
int Addition(poz S);                                            
int Replace(poz S, pozicija P);                                 
int PutIntoFile(pozicija P, pozicija P1);                      

int main()
{
    lista list = {.broj = 0, .Next = NULL };
    lista list1 = {.broj = 0, .Next = NULL };
    stablo stablo1 = {.broj = 2, .Left = NULL, .Right = NULL};
    int i = 0;
    poz NewEl = (poz)malloc(sizeof(stablo));
    int Niz[9] = {2, 5, 7, 8, 11, 1, 4, 3, 7};

    srand(time (NULL));

    Meni(&stablo1, &list, &list1, Niz, 9);

    return 0;
}

poz Insert1(poz S, int X) {

    if(S == NULL) {

        S = (poz)malloc(sizeof(stablo));
        S->broj = X;
        S->Left = NULL;
        S->Right = NULL;
    }

    else if(S->broj >= X)
        S->Left = AddElementIntoTree(X, S->Left);

    else
        S->Right = AddElementIntoTree(X, S->Right);

    return S;
}

int GiveRandNumbers(int X, pozicija P, poz S) {

    int* Pomocni = NULL;
    int cnt = 0;
    pozicija newEl = NULL;
    poz newEl1 = NULL;
    int i = 0;

    newEl = (pozicija)malloc(sizeof(lista));
    newEl1 = (poz)malloc(sizeof(stablo));

    if(!newEl) {

        printf("Neuspjesno alociranje memorije!\n");
        return -1;
    }

    Pomocni = (int*)malloc(X * sizeof(int));

    printf("%d nasumicnih brojeva:\n",X);

    for (i = 0; i < X ; i++) {

        Pomocni[i] = minimum + (rand() % (maximum - minimum + 1));

        printf("%d \n",Pomocni[i]);
    }

    for(i = 0 ; i < X ; i++) {

        newEl = CreateListElement(Pomocni[i]);

        if(newEl == NULL)
            printf("Greska!\n");

        InsertIntoList(P,newEl);

        if(cnt != 0) {

            printf("Greska!\n");
            return -1;
        }
    }

    for(i = 0 ; i < X ; i++) {

        newEl1 = AddElementIntoTree(Pomocni[i], S);
    }

    return 0;
}

pozicija CreateListElement(int X) {

    pozicija q = NULL;

	q = (pozicija)malloc(sizeof(lista));

	if (q == NULL) {

		printf("Neuspjesno alociranje memorije!\n");
		return NULL;
	}

	q->broj = X;
	q->Next = NULL;

    return q;
}

int PrintList(pozicija P) {

    if(P->Next == NULL) {

        printf("Lista je prazna!\n");
        return -1;
    }

    while(P->Next != NULL) {

        P = P->Next;
        printf("%d ",P->broj);
    }

    return 0;
}

int InsertIntoList(pozicija P, pozicija S) {

    if(P->Next == NULL) {

        P->Next = S;
        S->Next = NULL;
    }

    else {

        while(P->Next != NULL && (P->Next->broj <= S->broj))
            P = P->Next;

        S->Next = P->Next;
        P->Next = S;
    }

    return 0;
}

poz AddElementIntoTree(int X, poz S) {

    if(S == NULL) {

        S = (poz)malloc(sizeof(stablo));
        S->broj = X;
        S->Left = NULL;
        S->Right = NULL;
    }

    else if(S->broj > X)
        S->Left = AddElementIntoTree(X, S->Left);


    else
        S->Right = AddElementIntoTree(X, S->Right);

    return S;
}

int PrintInorder(poz S) {

    if(S == NULL)
        return 0;

    PrintInorder(S->Left);
    printf("%d ", S->broj);
    PrintInorder(S->Right);

    return 0;
}

int Addition(poz S) {

    if(!S)
        return 0;

    else
        return (S->broj + Addition(S->Left) + Addition(S->Right));
}

int Replace(poz S, pozicija P) {

    pozicija newEl = NULL;

    newEl = (pozicija)malloc(sizeof(lista));

    if(!S)
        return 0;

    S->broj = Addition(S->Right) + Addition(S->Left);

    newEl = CreateListElement(S->broj);

    InsertIntoList(P,newEl);

    Replace(S->Left, P);
    Replace(S->Right, P);

    return 0;
}

int Meni(poz S, pozicija P, pozicija P1, int Niz[], int X) {

    int key = 0, i = 0;
    poz NewEl = NULL;
    pozicija NewEl1 = NULL;
    pozicija NewEl2 = NULL;

    NewEl = (poz)malloc(sizeof(stablo));
    NewEl1 = (pozicija)malloc(sizeof(lista));
    NewEl2 = (pozicija)malloc(sizeof(lista));

    printf("Odaberite genericki niz ili nasumicne brojeve:\n");
    printf("1-genericki niz\n2-nasumicni brojeve ");
    scanf("%d", &key);

    if(key == 1) {

        for (i = 0 ; i < X ; i++)
            NewEl = Insert1(S,Niz[i]);

        printf("Ispis stabla: \n");
        PrintInorder(S);
        puts("");

        for (i = 0 ; i < X ; i++) {

            NewEl1 = CreateListElement(Niz[i]);
            InsertIntoList(P,NewEl1);
        }

        Addition(S);
        Replace(S,P1);

        puts("Nakon zamjene:\n");
        PrintInorder(S);

        for(i = 0 ; i < X ; i++) {

            NewEl2 = CreateListElement(Niz[i]);
            InsertIntoList(P1,NewEl2);
        }

        PutIntoFile(P,P1);
    }

    else if(key == 2) {

        printf("Koliko nasumicnih brojeva zelite?\n");
        scanf("%d", &X);

        GiveRandNumbers(X,P,S);

        printf("Ispis stabla: \n");
        PrintInorder(S);

        Addition(S);
        Replace(S,P1);

        puts("Nakon zamjene:\n");
        PrintInorder(S);

        PutIntoFile(P,P1);
    }

    return 0;
}

int PutIntoFile(pozicija P, pozicija P1) {

    FILE* fp = NULL;

    fp = fopen("dat.txt", "w");

    if(!fp) {

        printf("Greska pri otvaranju datoteke!\n");
        return -1;
    }

    fprintf(fp, "Prije funkcije (zad a)\n");

    while (P->Next != NULL) {

        fprintf(fp, "%d ", P->broj);
        P = P->Next;
    }

    fprintf(fp, "\nNakon funkcije (zad b)\n");

    while (P1->Next != NULL) {

        fprintf(fp, "%d ", P1->broj);
        P1 = P1->Next;
    }

    fclose(fp);

    return 0;
}
