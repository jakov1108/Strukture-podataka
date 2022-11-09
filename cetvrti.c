#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SUCCESS (0);
#define ALLOC_FAIL (-1)
#define NAME_MAX (50)
#define READ_FAIL (-1)
#define FOPEN_FAIL (-1)
#define MAX_LINE (150)
#define UNOS_FAIL (-2)
#define ADDITION_FAIL (-3)
#define MULTIP_FAIL (-4)

struct polinom;
typedef struct polinom* Pozicija;
struct polinom {
	int koef;
	int eks;
	Pozicija next;
};
typedef struct polinom Polinom;

Pozicija DodajHead();
int CitanjeIzDatoteke(Pozicija, Pozicija, char[]);
int SortiraniUnos(Pozicija, int, int);
int IspisPolinoma(Pozicija);
Pozicija ZbrojiPolinome(Pozicija, Pozicija);
int DodajNaKraj(Pozicija, int, int);
Pozicija PomnoziPolinome(Pozicija, Pozicija);
int IspisMenija();

int main() {

	Pozicija head1 = NULL;
	Pozicija head2 = NULL;
	Pozicija zbroj = NULL;
	Pozicija umnozak = NULL;
	int status = 0;
	char imeDatoteke[NAME_MAX] = { 0 };
	int odabir = 0;

	head1 = DodajHead();
	if (head1 == NULL) {
		printf("\nGreska u alociranju memorije!");
		return ALLOC_FAIL;
	}

	head2 = DodajHead();
	if (head2 == NULL) {
		printf("\nGreska u alociranju memorije!");
		return ALLOC_FAIL;
	}

	while (1) {
		IspisMenija();
		printf("\nUpisite broj akcije: ");
		scanf(" %d", &odabir);

		switch (odabir) {
		case 0:
			printf("\nHvala na koristenju, ugodan dan! :)");
			return SUCCESS;
			break;
		case 1:
			printf("\nUnesite ime datoteke: ");
			scanf(" %s", imeDatoteke);
			status = CitanjeIzDatoteke(head1, head2, imeDatoteke);
			if (status != 0) {
				printf("\nGreska u citanju iz datoteke!");
				return READ_FAIL;
			}
			printf("\nUspjesno ucitani polinomi!");
			break;
		case 2:
			IspisPolinoma(head1);
			printf("\nIspisan polinom br. 1");
			break;
		case 3:
			IspisPolinoma(head2);
			printf("\nIspisan polinom br. 2");
			break;
		case 4:
			zbroj = ZbrojiPolinome(head1, head2);
			if (zbroj == NULL) {
				printf("\nGreska u zbrajanju!");
				return ADDITION_FAIL;
			}
			IspisPolinoma(zbroj);
			printf("\nIspisan zbroj!");
			break;
		case 5:
			umnozak = PomnoziPolinome(head1, head2);
			if (umnozak == NULL) {
				printf("\nGreska u mnozenju!");
				return MULTIP_FAIL;
			}
			IspisPolinoma(umnozak);
			printf("\nIspisan umnozak!");
			break;
		}
	}
	
	return SUCCESS;
}

Pozicija DodajHead(){
	Pozicija head = NULL;
	head = (Pozicija)malloc(sizeof(Polinom));
	head->next = NULL;
	head->eks = 0;
	head->koef = 0;
	return head;
}

int CitanjeIzDatoteke(Pozicija head1, Pozicija head2, char imeDatoteke[NAME_MAX]) {

	FILE* fp = NULL;
	char buffer[MAX_LINE] = { 0 };
	char* buf;
	int koef = 0, eks = 0, br = 0;
	int status = 0;

	fp = fopen(imeDatoteke, "r");
	if (fp == NULL) {
		printf("\nGreska u otvaranju datoteke!");
		return FOPEN_FAIL;
	}

	fgets(buffer, MAX_LINE, fp);
	buf = buffer;

	while (strlen(buf) != 0) {
		sscanf(buf, "%d %d %n", &koef, &eks, &br);
		status = SortiraniUnos(head1,koef,eks);
		if (status != 0) return UNOS_FAIL;
		buf += br;
	}

	fgets(buffer, MAX_LINE, fp);
	buf = buffer;

	while (strlen(buf) != 0) {
		sscanf(buf, "%d %d %n", &koef, &eks, &br);
		status = SortiraniUnos(head2, koef, eks);
		if (status != 0) return UNOS_FAIL;
		buf += br;
	}

	printf("\nUspjesno citanje iz datoteke!");

	return SUCCESS;
}

int SortiraniUnos(Pozicija head, int koef, int eks) {

	Pozicija p = head;
	Pozicija q = NULL;

	if (p->next != NULL) {
		while (p->next != NULL && p->next->eks >= eks) {
			p = p->next;
		}

		if (p->eks == eks) {
			p->koef += koef;
			return SUCCESS;
		}
	}

	q = (Pozicija)malloc(sizeof(Polinom));

	if (q == NULL) {
		printf("\nGreska u alociranju memorije!");
		return ALLOC_FAIL;
	}

	q->koef = koef;
	q->eks = eks;

	q->next = p->next;
	p->next = q;

	return SUCCESS;
}

int IspisPolinoma(Pozicija head) {
	Pozicija p = head->next;

	printf("\n");
	while (p != NULL) {

		if (p->eks == 0) {
			if (p->koef > 0) {
				printf("+");
				printf("%d ", p->koef);
				p = p->next;
			}
			else if (p->koef == 0) {
				p = p->next;
			}
			else if (p->koef < 0) {
				printf("%d ", p->koef);
				p = p->next;
			}
			continue;
		}

		if (p->koef > 0 && p->eks!=0) {
			printf("+");
			printf("%dX^%d ", p->koef, p->eks);
			p = p->next;
		}
		else if (p->koef == 0 && p->eks != 0) {
			p = p->next;
		}
		else if (p->koef < 0 && p->eks != 0) {
			printf("%dX^%d ", p->koef, p->eks);
			p = p->next;
		}
	}

	return SUCCESS;
}

int DodajNaKraj(Pozicija zbroj, int koef, int eks) {

	Pozicija p = zbroj;
	Pozicija q = NULL;

	while (p->next != NULL) {
		p = p->next;
	}

	q = (Pozicija)malloc(sizeof(Polinom));

	if (q == NULL) {
		printf("\nGreska u alociranju memorije!");
		return ALLOC_FAIL;
	}

	q->koef = koef;
	q->eks = eks;

	q->next = p->next;
	p->next = q;

	return SUCCESS;
}

Pozicija ZbrojiPolinome(Pozicija head1, Pozicija head2) {

	Pozicija zbroj = NULL;

	Pozicija p1 = head1->next;
	Pozicija p2 = head2->next;

	zbroj = DodajHead();
	if (zbroj == NULL) {
		printf("\nGreska u alociranju memorije!");
		return ALLOC_FAIL;
	}

	while (p1 != NULL && p2 != NULL) {

		if (p1->eks == p2->eks) {
			DodajNaKraj(zbroj, p1->koef + p2->koef, p1->eks);
			p1 = p1->next;
			p2 = p2->next;
		}

		else if (p1->eks > p2->eks) {
			DodajNaKraj(zbroj, p1->koef, p1->eks);
			p1=p1->next;
		}

		else if (p1->eks < p2->eks) {
			DodajNaKraj(zbroj, p2->koef, p2->eks);
			p2 = p2->next;
		}
	}

	while (p1 != NULL) {
		DodajNaKraj(zbroj, p1->koef, p1->eks);
		p1 = p1->next;
	}

	while (p2 != NULL) {
		DodajNaKraj(zbroj, p2->koef, p2->eks);
		p2 = p2->next;
	}

	return zbroj;
}

Pozicija PomnoziPolinome(Pozicija head1, Pozicija head2) {

	Pozicija umnozak = NULL;

	Pozicija p1 = head1->next;
	Pozicija p2 = head2->next;

	umnozak = DodajHead();
	if (umnozak == NULL) {
		printf("\nGreska u alociranju memorije!");
		return ALLOC_FAIL;
	}

	while (p1 != NULL) {
		while (p2 != NULL) {
			SortiraniUnos(umnozak, p1->koef * p2->koef, p1->eks + p2->eks);
			p2 = p2->next;
		}
		p1 = p1->next;
		p2 = head2->next;
	}

	return umnozak;
}

int IspisMenija() {
	printf("\n\n-PROGRAM ZA POLINOME-");
	printf("\nOdaberite opciju:");
	printf("\n1) Unos polinoma iz datoteke");
	printf("\n2) Ispis prvog polinoma");
	printf("\n3) Ispis drugog polinoma");
	printf("\n4) Zbrajanje dvaju polinoma");
	printf("\n5) Mnozenje dvaju polinoma");
	printf("\n0) Zatvaranje programa");

	return SUCCESS;
}