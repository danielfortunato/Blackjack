#ifndef FUNCOES_H_INCLUDED
#define FUNCOES_H_INCLUDED
#define MAX_NOME    9
#define DIM_L 21
#define DIM_C 10


typedef struct carta{
    int codigo;
    int naipe;
    int graf_val;
}cartas;


typedef struct node_int{

    cartas carta;
    struct node_int *next;
}node;

typedef struct player{
    char name[MAX_NOME];
    int tipo;//0-HU,1-EA;
    int aposta;
    int money;
    int pontos;
    int pos_hand;
    int jogos_ganhos;
    int jogos_perdidos;
    int jogos_empatados;
    int num_ases;
    int status;//1-Blackjack,2-Bust,3-Surrender
    bool active;
    bool hit;
    int num;
    node *top;
    struct player*next;
}player;

typedef struct
{
    int money;
    int pos_hand;
    int pontos;
    int num_ases;
    int status;//0-nada,1-blackjack,2-bust;
    node*top;
}house;

// declaration of the functions related to graphical issues

int conta_pontos(player*,int);
int conta_pontos_casa(house*,int);
void calcula_dinheiro(player**,house** );
player* ler_ficheiro_parametros(int*,int*,char[]);
player* cria_lista_jogadores(player*,int);
node* preenche_lista_cartas(node*,int);
node* push(node*,cartas);
node* remove_carta(node*,node**,int*);
void clean_list(node**);
void clean_list_player(player**);
player* repor_variaveis(player*);
player* hit(player*,node**,int*);
house*repor_casa(house*);
house*hit_casa(house*,node**,int*,int);
void novo_jogo(player**,house**,int);
void ficheiro_txt(player*,house*);
player*repor_jogos(player*);
player* bet(player*);
player* surrender(player*);
player* _double(player*,node**,int*);
player* H(house**,player**,player*,node**,int*,bool*,int);
player* S(house**,player**,player*,node**,int*,bool*,int);
player* D(house**,player**,player*,node**,int*,bool*,int);
player* R(house**,player**,player*,node**,int*,bool*,int);
player* estrategia_EA(player**,player*,house**,node**,int*,bool*,int);
#endif

