/*
7. Napisati program koji pomocu vezanih listi (stabala) predstavlja strukturu direktorija.
Omoguciti unos novih direktorija i pod-direktorija, ispis sadrzaja direktorija i
povratak u prethodni direktorij. Tocnije program treba preko menija simulirati
koristenje DOS naredbi: 1- "md", 2 - "cd dir", 3 - "cd..", 4 - "dir" i 5 – izlaz.
*/

#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define FILENAME 100
#define EXIT 0
#define SUCCESS 0
#define ALLOC_FAIL 0

struct dir;
typedef struct dir* Pozicija;
struct dir {
	char naziv[FILENAME];
	Pozicija sibling;
	Pozicija child;
};
typedef struct dir dir;

struct stack;
typedef struct stack* Poz;
struct stack {
	Pozicija prethodna;
	Poz next;
};
typedef struct stack stack;


int stvoriDatoteku(Pozicija);
void ispisMenija();
Pozicija ulazUDatoteku(Pozicija, Poz);
Pozicija vracanjeNazad(Poz);
int dodajNaStog(Pozicija, Poz);
int makniSaStoga(Poz);
int ispisDatoteka(Pozicija);
int brisiStog(Poz);
int delete(Pozicija);


int main() {

	dir Root;
	stack stog;

	Root.child = NULL;
	Root.sibling = NULL;
	strcpy(Root.naziv,"Root");

	stog.next = NULL;
	stog.prethodna = NULL;

	Pozicija current = &Root;
	Pozicija temp = NULL;

	int odabir = 0, status = 0;

	while (1) {
		ispisMenija();
		printf("\nUpisite redni broj radnje: ");
		scanf("%d", &odabir);
		switch (odabir) {
		case 1:
			status = stvoriDatoteku(current);
			if (status != 0) {
				return ALLOC_FAIL;
			}
			break;
		case 2:
			temp = ulazUDatoteku(current, &stog);
			if (temp != NULL) {
				current = temp;
			}
			else {
				printf("\nNije pronaden odgovarajuci file!");
			}
			break;
		case 3:
			if (current == &Root) {
				printf("\nVec ste u Root folderu!");
				break;
			}
			current = vracanjeNazad(&stog);
			if (current == NULL) {
				return ALLOC_FAIL;
			}
			break;
		case 4:
			ispisDatoteka(current);
			break;
		case 5:
			brisiStog(stog.next);
			delete(Root.child);
			printf("\nHVALA NA KORISTENJU!");
			return EXIT;
			break;
		}
	}

	return SUCCESS;
}


int stvoriDatoteku(Pozicija current) {
	Pozicija p = current;
	Pozicija q = NULL;
	char imeDat[FILENAME] = { 0 };
	printf("\nUnesi naziv nove datoteke: ");
	scanf("%s", imeDat);

	if (p->child == NULL) {

		q = (Pozicija)malloc(sizeof(dir));

		if (q == NULL) {
			printf("\nGreska u alociranju memorije!");
			return ALLOC_FAIL;
		}

		p->child = q;
		q->sibling = NULL;
		strcpy(q->naziv, imeDat);
		q->child = NULL;

		printf("\nKrenirana nova datoteka %s!",imeDat);
	}
	else {
		if (strcmp(p->child->naziv, imeDat)>0) {
			q = (Pozicija)malloc(sizeof(dir));

			if (q == NULL) {
				printf("\nGreska u alociranju memorije!");
				return ALLOC_FAIL;
			}

			q->sibling = p->child;
			p->child = q;
			strcpy(q->naziv, imeDat);
			q->child = NULL;

			printf("\nKrenirana nova datoteka %s!", imeDat);

			return SUCCESS;
		}
		p = p->child;
		while (p->sibling != NULL && strcmp(p->sibling->naziv, imeDat) < 0) {
			p = p->sibling;
		}

		q = (Pozicija)malloc(sizeof(dir));

		if (q == NULL) {
			printf("\nGreska u alociranju memorije!");
			return ALLOC_FAIL;
		}

		q->sibling = p->sibling;
		p->sibling = q;
		strcpy(q->naziv, imeDat);
		q->child = NULL;

		printf("\nKrenirana nova datoteka %s!", imeDat);
	}

	return SUCCESS;
}

void ispisMenija() {
	printf("\n\n==============UPRAVITELJ DATOTEKA==============");
	printf("\nOpcije:");
	printf("\n1) Stvaranje direktorija (md)");
	printf("\n2) Promjena direktorija (cd dir)");
	printf("\n3) Vracanje u prethodni direktorij (cd ..)");
	printf("\n4) Ispisivanje sadrzaja direktorija (dir)");
	printf("\n5) Izlaz iz programa (exit)");
	printf("\n=================================================\n\n");
}

Pozicija ulazUDatoteku(Pozicija current, Poz stogHead) {
	char imeDat[FILENAME] = { 0 };
	printf("\nUnesi ime odabrane datoteke: ");
	scanf("%s", imeDat);
	Pozicija p = current->child;
	int status = 0;

	while (p != NULL && strcmp(p->naziv, imeDat) != 0) {
		p = p->sibling;
	}

	if (p == NULL) {
		return NULL;
	}
	
	status=dodajNaStog(current, stogHead);
	if (status != 0) {
		return NULL;
	}

	current = p;

	printf("\nOdabrana datoteka %s!", current->naziv);

	return current;
}

int dodajNaStog(Pozicija current, Poz p) {
	Poz q = NULL;
	q = (Poz)malloc(sizeof(stack));
	if (q == NULL) {
		return ALLOC_FAIL;
	}

	q->next = p->next;
	p->next = q;

	q->prethodna = current;

	return SUCCESS;
}

Pozicija vracanjeNazad(Poz p) {
	Pozicija current = NULL;
	current = p->next->prethodna;
	makniSaStoga(p);

	printf("\nVratili ste se u direktorij %s", current->naziv);

	return current;
}

int makniSaStoga(Poz p) {
	Poz temp = NULL;
	temp = p->next;
	p->next = temp->next;
	free(temp);

	return SUCCESS;
}

int ispisDatoteka(Pozicija current) {
	Pozicija p = current->child;
	printf("\nDatoteke u direktoriju %s su: ", current->naziv);
	while (p != NULL) {
		printf("\n> %s", p->naziv);
		p = p->sibling;
	}
	printf("\n=======================");
	return SUCCESS;
}

int brisiStog(Poz p) {
	if (p == NULL) {
		return SUCCESS;
	}
	if (p->next != NULL) {
		brisiStog(p->next);
	}
	free(p);

	return SUCCESS;
}

int delete(Pozicija p) {
	if (p == NULL) {
		return SUCCESS;
	}
	if (p->child != NULL) {
		delete(p->child);
	}
	if (p->sibling != NULL) {
		delete(p->sibling);
	}
	free(p);

	return SUCCESS;
}
