#include <stdio.h>

#define SUCCESS (-1)
#define NAME_MAX (100)
#define PROGRAM_EXIT (-1)
#define ALOCC_POZ_FAIL (NULL)
#define FILE_FAIL (-1)
#define BUFFER_MAX (200)

typedef struct racun* Pozicija;

struct racun {
	float br;
	Pozicija next;
	Pozicija kraj;
};

typedef struct racun racun;

int citanjeDatoteke(char[NAME_MAX],Pozicija);
Pozicija createHead();
Pozicija dodajNaStog(int,Pozicija);

int main() 
{
	int status = 0;
	Pozicija head = NULL;
	head = createHead();

	if (head == NULL)
	{
		return PROGRAM_EXIT;
	}

	char imeDat[NAME_MAX] = { 0 };

	printf("\nUnesite ime datoteke: ");
	scanf(" %s", imeDat);

	status=citanjeDatoteke(imeDat, head);
	if (status != 0)
	{
		return PROGRAM_EXIT;
	}

	return SUCCESS;
}

int citanjeDatoteke(char imeDat[NAME_MAX],Pozicija p)
{
	Pozicija status = NULL;
	int tmp=0;
	char buffer[BUFFER_MAX] = { 0 };
	FILE* fp = NULL;
	fp = fopen(imeDat, "r");

	if (fp == NULL)
	{
		printf("\nGreska pri otvaranju datoteke!");
		return FILE_FAIL;
	}
	
	fgets(fp,"%s",buffer);

	while(strlen(buffer)!=0)
	{
		if (sscanf(buffer, "%d", tmp)==1)
		{
			status=dodajNaStog(tmp,p);
			if (status == NULL)
			{
				return FILE_FAIL;
			}
		}
		else
		{

		}
	}
	

}

Pozicija createHead()
{
	Pozicija head = NULL;
	head = (Pozicija)malloc(sizeof(racun));

	if (head == NULL)
	{
		printf("\nGreska pri alokaciji!");
		return ALOCC_POZ_FAIL;
	}

	head->next = NULL;

	return head;
}

Pozicija dodajNaStog(int br, Pozicija p)
{
	Pozicija q = NULL;
	q = (Pozicija)malloc(sizeof(racun));
	if (q == NULL)
	{
		printf("\nGreska pri alokaciji!");
		return ALOCC_POZ_FAIL;
	}

	q->next = p->next;
	p->next = q;

	q->br = (float) br;

}
