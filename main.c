#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define espaco 5
#define tam 30

typedef struct registro{
    char nome[tam];
    char pai[tam];
    int genero;
    int idade;
}registro;

typedef struct no{
    struct registro dados;
    struct no *esq;
    struct no *dir;
}no;

void desenha_raiz_horiz(no *raiz, int profundidade, char *mapa, int dir)
{
    int i,j;

    if (raiz== NULL)
        return;

    profundidade++;

    desenha_raiz_horiz(raiz->dir, profundidade, mapa, 1);

    mapa[profundidade-2] = 0;

    if(dir)
        mapa[profundidade-2] = 1;

    if(raiz->esq)
        mapa[profundidade-1] = 1;

    printf("\n");

    for(i=0; i<profundidade-1; i++)
    {
        if(i == profundidade-2)
            printf("+");
        else if(mapa[i])
            printf("|");
        else
            printf(" ");

        for(j=1; j<espaco; j++)
            if(i < profundidade-2)
                printf(" ");
            else
                printf("-");
    }

    printf("%s\n", raiz->dados.nome);

    for(i=0; i<profundidade; i++)
    {
        if(mapa[i])
            printf("|");
        else
            printf(" ");
        for(j=1; j<espaco; j++)
            printf(" ");
    }
    desenha_raiz_horiz(raiz->esq, profundidade, mapa, 0);
}

void desenhar(no *raiz)
{
    char mapa[255] = {};
    desenha_raiz_horiz(raiz, 0, mapa, 0);

}

no *adicionar(no *raiz, registro pessoa){
    if(raiz == NULL){
        no *novo = (no*)malloc(sizeof(no));
        novo->dados = pessoa;
        novo->esq = NULL;
        novo->dir = NULL;

        raiz = novo;
    } else {
        if(strcmp(raiz->dados.nome, pessoa.pai) == 0){
            if(raiz->esq == NULL)
                raiz->esq = adicionar(raiz->esq, pessoa);
            else if(raiz->dir == NULL)
                raiz->dir = adicionar(raiz->dir, pessoa);
            else {
                printf("\nEssa pessoa ja tem dois filhos !\n");
                system("pause");
            }
        } else {
            if(raiz->esq != NULL)
                raiz->esq = adicionar(raiz->esq, pessoa);
            if(raiz->dir != NULL)
                raiz->dir = adicionar(raiz->dir, pessoa);
        }
    }
    return raiz;
};

void filhos(no *raiz, char pai[tam]){
    if(raiz != NULL){
        if(strcmp(raiz->dados.nome, pai) == 0)
            printf("\n %s e %s \n", raiz->esq->dados.nome, raiz->dir->dados.nome);
        else {
            filhos(raiz->esq, pai);
            filhos(raiz->dir, pai);
        }
    }
};

int netos(no *raiz, char pai[tam], int cont){
    if(raiz != NULL){
        if(strcmp(raiz->dados.pai, pai) == 0){
            if(raiz->esq != NULL)
                cont = netos(raiz->esq, pai, ++cont);
            if(raiz->dir != NULL)
                cont = netos(raiz->dir, pai, ++cont);
        } else {
            cont = netos(raiz->esq, pai, cont);
            cont = netos(raiz->dir, pai, cont);
        }
    }
    return cont;
};

int *homemEmulher(no *raiz, int contG[2]){
    if(raiz != NULL){
        contG = homemEmulher(raiz->esq, contG);
        contG = homemEmulher(raiz->dir, contG);

        if(raiz->dados.genero == 1)
            contG[0]++;
        if(raiz->dados.genero == 2)
            contG[1]++;
    }
    return contG;
};

int quintaGeracao(no *raiz, int cont){
    if(raiz != NULL){
        if(cont == 5)
            printf(" %s.\n", raiz->dados.nome);

        cont = quintaGeracao(raiz->esq, ++cont);
        cont = quintaGeracao(raiz->dir, ++cont);
    }
    return --cont;
};

int homemIdade(no *raiz, int x, int cont){
    if(raiz != NULL){
        if(raiz->dados.genero == 1 && raiz->dados.idade > x)
            cont++;
        cont = homemIdade(raiz->esq, x, cont);
        cont = homemIdade(raiz->dir, x, cont);
    }
    return cont;
};

int bisneto(no *raiz, int cont){
    if(raiz != NULL){
        if(cont == 4)
            printf(" %s.\n", raiz->dados.nome);

        cont = bisneto(raiz->esq, ++cont);
        cont = bisneto(raiz->dir, ++cont);
    }
    return --cont;
};

void menu(){
    no *raiz = (no*)malloc(sizeof(no));
    raiz = NULL;

    int op, cont;

    do{
        system("cls");
        desenhar(raiz);

        printf("\n0 - Sair\n1 - Adicionar\n2 - Ver filhos\n3 - Ver Netos\n4 - Quantos homens e mulheres\n5 - Nomes da quinta geracao\n6 - Homens que possuem mais de x anos\n7 - Filhos dos Bisnetos\n\nSua escolha: ");
        scanf("%d", &op);

        switch(op){
            case 1:{
                char nome[tam];
                printf("\nNome: ");
                scanf("%s", nome);

                int genero;
                printf("\nGenero[1 - H/ 2 - M]: ");
                scanf("%d", &genero);

                int idade;
                printf("\nIdade: ");
                scanf("%d", &idade);

                char pai[tam];
                printf("\nPai: ");
                scanf("%s", pai);

                registro novo;
                strcpy(novo.nome, nome);
                novo.idade = idade;
                novo.genero = genero;
                strcpy(novo.pai, pai);

                raiz = adicionar(raiz, novo);
            break;}

            case 2:{
                char pai[tam];
                printf("\nFilhos de qual pai ? ");
                fflush(stdin);
                gets(pai);

                filhos(raiz, pai);
                system("pause");
            break;}

            case 3:{
                cont = 0;
                char pai[tam];
                printf("\nNetos de quem ? ");
                fflush(stdin);
                gets(pai);

                cont = netos(raiz, pai, cont);
                printf("\n%s eh vo de seus %d netos\n", pai, cont);
                system("pause");
            break;}

            case 4:{
                int *contG = (int*)malloc(sizeof(int)*2);
                contG[0] = 0;
                contG[1] = 0;

                contG = homemEmulher(raiz, contG);
                printf("\nSao %d homens e %d mulheres\n", contG[0], contG[1]);
                system("pause");
                free(contG);
            break;}

            case 5:{
                cont =0;

                quintaGeracao(raiz, cont);
                system("pause");
            break;}

            case 6:{
                cont =0;
                int x;
                printf("\nMaior do que quantos anos ? ");
                scanf("%d", &x);

                cont = homemIdade(raiz, x, cont);
                printf("\nSao %d homens mais velhos que %d anos\n", cont, x);
                system("pause");
            break;}

            case 7:{
                cont =0;

                bisneto(raiz, cont);
                system("pause");
            break;}
        }
    }while(op);
};

int main()
{
    menu();

    return 0;
}
