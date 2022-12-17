/*Napisati program koji omogućava rad s binarnim stablom pretraživanja. Treba
omogućiti unošenje novog elementa u stablo, ispis elemenata (inorder, preorder, postorder i
level order), brisanje i pronalaženje nekog elementa.
*/

#include <stdio.h>
#include <stdlib.h>

#define SUCCESS (0)
#define PROGRAM_EXIT (-1)
#define ALLOC_FAIL_POZ (NULL)

typedef struct stablo* pozicija;

struct stablo
{
    int broj;
    pozicija desno;
    pozicija lijevo;
};

typedef struct stablo stablo;

pozicija createStruct(pozicija);
pozicija unosElementa(pozicija,pozicija);
int dubinaStabla(pozicija);
int ispisNivoa(pozicija, int);
int ispisStablaLevelOrder(pozicija);
pozicija izbrisi(pozicija, int);
pozicija nadjiMax(pozicija);
pozicija nadjiMin(pozicija);

int main()
{
    pozicija q=NULL;
    int operacija=0;
    int brEl=0;
    pozicija root=NULL;
    root=createStruct(root);
    if(root==NULL)
    {
        return PROGRAM_EXIT;
    }

    printf("\nUnesite root element: ");
    scanf("%d", &root->broj);

    while(1)
    {
        printf("\n---------MENI--------");
        printf("\n1) Unos novog elementa");
        printf("\n2) Ispis stabla (Level order)");
        printf("\n3) Brisanje elementa");
        printf("\n4) Izlaz iz programa");
        printf("\nOdaberite operaciju: ");

        scanf("%d",&operacija);

        switch(operacija)
        {
            case 1:
                q=createStruct(q);
                if(q==NULL)
                {
                    return PROGRAM_EXIT;
                }

                printf("\nUnesite element: ");
                scanf("%d", &q->broj);

                root=unosElementa(root,q);

                break;
            case 2:
                ispisStablaLevelOrder(root);
                break;
            case 3:
                printf("\nKoji element zelite izbrisati: ");
                scanf("%d",&brEl);

                root=izbrisi(root,brEl);
                break;
            case 4:
                printf("\nHvala na koristenju!");
                return SUCCESS;
            default:
                printf("\nNepostojeca operacija!");
                break;
        }
    }

    return SUCCESS;
}

pozicija createStruct(pozicija p)
{
    p=(pozicija)malloc(sizeof(stablo));
    if(p==NULL)
    {
        printf("\nGreska pri alociranju memorije!");
        return ALLOC_FAIL_POZ;
    }

    p->lijevo=NULL;
    p->desno=NULL;

    return p;
}

pozicija unosElementa(pozicija p, pozicija q)
{
    if(p==NULL)
    {
        return q;
    }

    if(p->broj < q->broj)
    {
        p->desno=unosElementa(p->desno,q);
    }
    else if(p->broj > q->broj)
    {
        p->lijevo=unosElementa(p->lijevo,q);
    }
    else
    {
        printf("\nElement vec postoji!");
        free(q);
    }

    return p;
}

int dubinaStabla(pozicija root)
{
    if (root==NULL)
        return 0;
    else {
        int visinaLijevo = dubinaStabla(root->lijevo);
        int visinaDesno = dubinaStabla(root->desno);

        if (visinaLijevo >= visinaDesno)
            return visinaLijevo + 1;
        else
            return visinaDesno + 1;
    }
}

int ispisNivoa(pozicija root, int nivo)
{
    if (root==NULL)
        return SUCCESS;
    if (nivo == 0) {
        printf("%d -> ", root->broj);
    }
    else {
        ispisNivoa(root->lijevo, nivo - 1);
        ispisNivoa(root->desno, nivo - 1);
    }

    return SUCCESS;
}

int ispisStablaLevelOrder(pozicija root)
{
    if (root==NULL)
        return SUCCESS;

    int dubina = dubinaStabla(root);

    for (int i=0; i<dubina; i++) {
        printf("Level %d: ", i+1);
        ispisNivoa(root, i);
        printf("\n");
    }

    printf("\n");

    return SUCCESS;
}

pozicija izbrisi(pozicija p, int br)
{
    if(p==NULL)
    {
        return NULL;
    }

    if(p->broj < br)
    {
        p->desno=izbrisi(p->desno,br);
    }
    else if(p->broj > br)
    {
        p->lijevo=izbrisi(p->lijevo,br);
    }
    else
    {
        if(p->lijevo)
        {
            pozicija tmp=nadjiMax(p->lijevo);
            p->broj=tmp->broj;
            p->lijevo=izbrisi(p->lijevo,tmp->broj);
        }
        else if(p->desno)
        {
            pozicija tmp=nadjiMin(p->desno);
            p->broj=tmp->broj;
            p->desno=izbrisi(p->lijevo,tmp->broj);
        }
        else
        {
            free(p);
            return NULL;
        }
    }

    return p;
}

pozicija nadjiMax(pozicija p)
{
    pozicija tmp=p;

    while (tmp->desno != NULL)
        tmp = tmp->desno;

    return tmp;
}

pozicija nadjiMin(pozicija p)
{
    pozicija tmp=p;

    while (tmp->lijevo != NULL)
        tmp = tmp->lijevo;

    return tmp;
}
