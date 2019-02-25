#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>               //Biblioteca utilizada para colocar a função time.
#include <stdbool.h>
#include <string.h>
#include "funcoes.h"

#define STRING_SIZE 100       // max size for some strings
#define WIDTH_WINDOW 900      // window width
#define HEIGHT_WINDOW 525     // window height
#define MAX_DECK_SIZE 52      // number of max cards in the deck
#define MAX_CARD_HAND 11      // 11 cards max. that each player can hold
#define CARD_WIDTH 67         // card width
#define CARD_HEIGHT 97        // card height
#define WINDOW_POSX 500       // initial position of the window: x
#define WINDOW_POSY 250       // initial position of the window: y
#define EXTRASPACE 150
#define MARGIN 5
#define MAX_PLAYERS 4         // number of maximum players
#define MAX_CARTAS 312        //número máximo de cartas no vetor de inteiros.



//Função para contar os pontos dos jogadores, recebe como parametro um ponteiro para um jogador e o codigo da carta retirada.

int conta_pontos(player*player,int carta_ret)
{

    int pontosaux=0;
    int carta= carta_ret;
    int pontos_atuais=0;
    pontos_atuais=player->pontos;


                        if(carta<=9)
                            pontosaux=carta+1;
                        else
                            if(carta==13)
                            {
                               pontosaux=11;
                               player->num_ases++;
                            }
                            else
                                pontosaux=10;

                        pontosaux=pontosaux+pontos_atuais;
                        
                        while((pontosaux>21)&&player->num_ases>0)
                        {
                            pontosaux=pontosaux-10;
                            player->num_ases--;
                        }
                            
                        return pontosaux;

}

//Função para contar os pontos da casa, recebe um ponteiro para a estrutura casa e o codigo da carta retirada.

int conta_pontos_casa(house *casa, int carta_retirada)
{
    int pontosaux=0;
    int carta= carta_retirada;
    int pontos_atuais=0;
    pontos_atuais=casa->pontos;

                        if(carta<=9)
                            pontosaux=carta+1;
                        else
                            if(carta==13)
                            {
                               pontosaux=11;
                               casa->num_ases++;
                            }
                            else
                                pontosaux=10;

                        pontosaux=pontosaux+pontos_atuais;

                        while((pontosaux>21)&&casa->num_ases>0)
                        {
                            pontosaux=pontosaux-10;
                            casa->num_ases--;
                        }

                        return pontosaux;


}

//Função que calcula o dinheiro de cada jogador no final do jogo e recolhe os dados necessários para a escrita do ficheiro das estatisticas sobre os jogos realizados.
void calcula_dinheiro(player**head_player,house**casa)
{
    player*_player=NULL;
    house*_casa=NULL;
    _player=*head_player;
    _casa=*casa;
    while(_player!=NULL)
    {
        if(_player->active==false||_player->status==3)
        {
            _player=_player->next;
            continue;
        }        
        
        if((_player->status==1)&&(_casa->status!=1))
        {
            _player->money=_player->money+1.5*_player->aposta;
           _casa->money = _casa->money -1.5*_player->aposta;
            _player->jogos_ganhos++;
        }
        else
            if((_player->status==1)&&(_casa->status==1))
            {
               _player->jogos_empatados++;
            }
            else
                if(_player->status!=1&&_casa->status==1)
                    {
                        _player->money=_player->money-_player->aposta;
                        _casa->money=_casa->money+(_player->aposta);
                        _player->jogos_perdidos++;
                    }
                    else
                        if(_player->status==2)
                        {
                            _player->money=_player->money-_player->aposta;
                            _casa->money=_casa->money+(_player->aposta);
                            _player->jogos_perdidos++;
                        }
                        else
                            if((_player->status!=2||_player->status!=1)&&(_casa->status==2))
                            {
                               _player->money=_player->money+_player->aposta;
                               _casa->money=_casa->money-_player->aposta;
                               _player->jogos_ganhos++;
                            }
                            else
                                if(_casa->status==0&&_player->status==0&&((_player->pontos)>(_casa->pontos)))
                                {
                                   _player->money=_player->money+_player->aposta;
                                   _casa->money=_casa->money-_player->aposta;
                                   _player->jogos_ganhos++;

                                }
                                else
                                 if(_casa->status==0&&_player->status==0&&((_player->pontos)<(_casa->pontos)))
                                       {
                                           _player->money=_player->money-_player->aposta;
                                           _casa->money=_casa->money+_player->aposta;
                                           _player->jogos_perdidos++;

                                        }
                                    else
                                        {
                                            _player->jogos_empatados++;
                                        }

            if(_player->money<=0)
                    _player->active=false;
            
            _player=_player->next;
    }


}
//Função que escreve o ficheiro .txt segundo os dados recolhidos durante o jogo, recebe como parametros a head da lista de jogadores e a casa.
void ficheiro_txt(player*_head,house* _casa)
{
int i=0;
FILE *statsfp;
player*aux_player=NULL;
statsfp=fopen("stats.txt","w");
if (statsfp==NULL)
{
    printf("Can't open stats file!\n");
    exit(EXIT_FAILURE) ;
}
aux_player=_head;
while(aux_player!=NULL)
{   
    if(aux_player->active==false&&aux_player->num==0)
    {
        aux_player=aux_player->next;
        continue;
    }
    fprintf(statsfp,"Jogos empatados/ganhos/perdidos:%d/%d/%d\nDinheiro:%d$\n",aux_player->jogos_empatados,aux_player->jogos_ganhos,aux_player->jogos_perdidos,aux_player->money);
    aux_player=aux_player->next;
    i++;
}
fprintf(statsfp,"Casa:\nBalanço monetário:%d$\n",_casa->money);
fclose(statsfp);
}

//Função que cria o mega baralho de cartas segundo o número de baralhos especificado no ficheiro dos parametros.
node* preenche_lista_cartas(node*head,int num_baralhos)
{
    int i=0;
    int v=1;
    int n=0;
    int g=0;
    node *new_node=NULL;
    node *aux=NULL;

    while(i<52*num_baralhos)
    {
        if(g==52)
            g=0;

        if(v==14)
        {
            n++;
            v=1;
        }

        if(n==4)
            n=0;

        new_node = (node*)malloc(sizeof(node));

        if(new_node==NULL)
        {
            printf("Falhou a criação de memória!\n");
            exit(EXIT_FAILURE);
        }

        new_node->carta.codigo=v;
        new_node->carta.naipe=n;
        new_node->carta.graf_val=g;

    if (head==NULL)
        {
            head=new_node;
            head->next=NULL;
            continue;
        }

        aux=head;

        while(aux->next!=NULL)
            aux=aux->next;

        aux->next=new_node;
        new_node->next=NULL;

       i++;
       v++;
       g++;

    }
return head;
}

//Função que limpa uma lista, recebe como parametro de entrada a head dessa lista.
void clean_list(node **_head)
{
    node *aux = *_head;
    node *tmp = NULL;

    while(aux != NULL)
    {
        tmp=aux;
        aux=aux->next;
        free(tmp);
    }
*_head=NULL;
}
//Função que coloca as cartas retiradas do mega-baralho na mão(lista) de cada um dos jogadores e casa.
node* push(node* top,cartas _carta)
{
    node*aux=NULL;
    node *new_node= (node*)malloc(sizeof(node));
    aux=top;

    if(new_node==NULL)
    {
        printf("Falhou a alocação de memória!\n");
        exit(EXIT_FAILURE);

    }

    new_node->carta=_carta;

    if(top==NULL)
    {
        top=new_node;
        top->next=NULL;

    }
    else
        {
            while(aux->next!=NULL)
                    aux=aux->next;

            aux->next=new_node;
            new_node->next=NULL;
        }
    return top;
}

//Função que remove um carta(nó) do mega-baralho(lista) das cartas, receb como parametro a head dos jogadores e do mega-baralho e o número de cartas
//restantes existentes no baralho.
node* remove_carta(node *_head,node**carta,int *cartas_restantes)
{
    node*anterior=NULL;
    node*atual=_head;

    int pos=0;
    pos=rand()%*cartas_restantes;

    while((atual!=NULL)&&pos!=0)
    {
        anterior=atual;
        atual=atual->next;
        pos--;
    }
    if(anterior!=NULL)
            anterior->next=atual->next;
        else
            _head=_head->next;

    *carta=atual;

    return _head;
}

player* cria_lista_jogadores(player*_head,int num_jogadores)
{
    player* new_node=NULL;
    player* aux=NULL;
    int i=0;

    while(i<4)
    {

       new_node=(player*)malloc(sizeof(player));

       if(new_node==NULL)
       {
           printf("Falhou a alocação de memória!!\n");
           exit(EXIT_FAILURE);
       }



       if (_head==NULL)
       {
           _head=new_node;
           _head->active=false;
           _head->next=NULL;
           _head->num=0;
           i++;
           continue;
       }

       aux=_head;

       while(aux->next!=NULL)
            aux=aux->next;

        aux->next=new_node;
        new_node->next=NULL;
        new_node->active=false;
        new_node->num=0;
        i++;
    }

    aux=_head;
    i=0;

    while(i<num_jogadores)
    {
       aux->active=true;
       aux->num=1;
       aux=aux->next;
       i++;
    }

    return _head;

}
//Função que repões as variaveis de cada um dos jogadores de modo a inicalizar um novo jogo.
player* repor_variaveis(player*_head)
{
    player*aux=NULL;
    aux=_head;
    while(aux!=NULL)
    {   
        if(aux->status==4)
            aux->aposta=(aux->aposta)/2;
        aux->pontos=0;
        aux->status=0;
        aux->pos_hand=2;
        aux->num_ases=0;
        aux->hit=false;
        aux=aux->next;
    }
     return _head;
}
//Função que repões as variaveis da casa de modo a inicalizar um novo jogo.
house *repor_casa(house*_casa)
{
   house*aux=NULL;
   aux=_casa;
   aux->pontos=0;
   aux->pos_hand=1;
   aux->status=0;
   return aux;
}
//FUnção que lê o ficheiro dos parametros.
player* ler_ficheiro_parametros (int *num_baralhos,int *num_jogadores,char str1[])
{

    const char s[2] = "-";
    char *token;
    char linha[90];
    char *result;
    char str_EA[3]="EA";
    char str_HU[3]="HU";
    int j=0;
    char param[20];
    player *aux=NULL;
    player *head=NULL;
    aux=head;
    
   
    sscanf(str1,"%s",param);

    FILE *fp1;
    fp1 = fopen(param, "r");
    if(fp1 == NULL)
    {
       printf("Erro ao abrir o arquivo.\n");
    }


    while (!feof(fp1)) //para garantir que não passa do tamanho da lista.
    {
      result = fgets(linha, 90, fp1);

      if (result)
          token = strtok(result, s);

        if (j==6)
            j=2;

      while( token != NULL )
      {

        switch(j)
        {
            case 0:
                sscanf (token, "%d", num_baralhos);
                printf( " %d\n", *num_baralhos);
                if(*num_baralhos<4||*num_baralhos>8)
                {
                    printf("Número de baralhos não permitido!!\n");
                    exit(EXIT_FAILURE);
                }
                j++;
                break;

            case 1:
                sscanf (token, "%d", num_jogadores);
                printf( " %d\n", *num_jogadores );
                if(*num_jogadores<0||*num_jogadores>4)
                {
                    printf("Número de jogadores não permitido!!\n");
                    exit(EXIT_FAILURE);
                }

                head=cria_lista_jogadores(head,*num_jogadores);
                aux=head;
                j++;
                break;

            case 2:
                if(strcmp(str_EA,token)==0)
                {
                   aux->tipo= 1;
                }
                else
                    if(strcmp(str_HU,token)==0)
                    {
                        aux->tipo=0;
                    }
                    else
                    {
                        printf("Tipo de jogador não esperado!!\n");
                        exit(EXIT_FAILURE);
                    }

                printf("tipo %d\n",aux->tipo);
                j++;
                break;


            case 3:

                strcpy (aux->name,token);
                printf("nome %s\n",aux->name);
                j++;
                break;

            case 4:
                sscanf (token, "%d", &aux->money);
                printf("money %d\n",aux->money);
                if(aux->money<10||aux->money>500)
                {
                    printf("Apenas são aceites valores de dinheiro entre 10 e 500 euros.\n");
                    exit(EXIT_FAILURE);
                }
                j++;
                break;

            case 5:
                sscanf (token, "%d", &aux->aposta);
                printf("aposta %d\n\n",aux->aposta);
                if(aux->aposta<2||aux->aposta>0.25*aux->money)
                {
                    printf("A aposta inicial do jogador não pode ser inferior a 2 ou superior a 25%% do valor do dinheiro!\n");
                    exit(EXIT_FAILURE);
                }
                aux=aux->next;
                j++;

                break;


        }

        token = strtok(NULL, s);
      }

    }
    fclose(fp1);

return head;
}

//Função que realiza a retirada da carta do mega-baralho, a colocação desta na mão do jogador e a contagem dos pontos deste, recebe o jogador, a cabeça do mega baralho e um ponteiro
//para o número de carats restantes.
player* hit(player*_player,node**head_bar,int*cartas_restantes)
{
    node*carta_a_colocar;
    player*aux_player=_player;
    aux_player->hit=true;
    int cartas_rest=*cartas_restantes;
    *head_bar=remove_carta(*head_bar,&carta_a_colocar,&cartas_rest);
    aux_player->top=push(aux_player->top,carta_a_colocar->carta);
    aux_player->pontos=conta_pontos(aux_player,carta_a_colocar->carta.codigo);
    if(aux_player->pontos>21)
        aux_player->status=2;
    cartas_rest--;
    *cartas_restantes=cartas_rest;
    aux_player->pos_hand++;
    return _player;
}
//Função semelhante à anterior mas que é dedicada para a casa.
house *hit_casa(house*_casa,node**head_bar,int *cartas_restantes,int num_baralhos)
{
    node*carta_a_colocar;
    while(_casa->pontos<17)
    {
        int cartas_rest=*cartas_restantes;
        *head_bar=remove_carta(*head_bar,&carta_a_colocar,&cartas_rest);
        _casa->top=push(_casa->top,carta_a_colocar->carta);
        _casa->pontos=conta_pontos_casa(_casa,carta_a_colocar->carta.codigo);
        cartas_rest--;
         if(cartas_rest==0)
        {
            *head_bar=NULL;
            *head_bar=preenche_lista_cartas(*head_bar,num_baralhos);
            *cartas_restantes=52*num_baralhos;
        }      
        *cartas_restantes=cartas_rest;
        if(_casa->pontos>21)
            _casa->status=2;
        _casa->pos_hand++;
    }

    return _casa;

}

//Função que repõe as variaveis necessária a inicialização de um jogo.
player* repor_jogos(player*_head)
{
    player*aux=_head;
    
    while(aux!=NULL)
    {
        aux->jogos_ganhos=0;
        aux->jogos_perdidos=0;
        aux->jogos_empatados=0;
        aux=aux->next;
    }
    return _head;
}
//Função que altera a aposta de cada jogador, recebe como para metro a head da lista de jogadores.
player* bet(player*_head)
{
    player*aux=NULL;
    char nome[10];
     char nome2[10];
    char _aposta[4];
    int aposta;
    int ret=0;
 
    while(ret==0)
    {
    printf("Qual o nome do jogador a mudar a aposta?\n");
    fgets(nome,10,stdin);
    ret=sscanf(nome,"%s",nome2);
    }
    ret=0;
    while(ret==0)
    {
    printf("Qual a nova aposta?\n");
    fgets(_aposta,4,stdin);
    ret=sscanf(_aposta,"%d",&aposta);
    }
       
    aux=_head;
    //Vai comparar o nome colocado com os nomes dos jogadores de modo a encontrar o jogador indicado.  
    while(strcmp(nome2,aux->name)!=0)
    {
        aux=aux->next;
       
    }
    
    aux->aposta=aposta;
    
    return _head;
}
//Função que realiza o surrender.
player* surrender(player*aux_player)
{
    
    aux_player->money=aux_player->money-((aux_player->aposta)/2);
    aux_player->jogos_perdidos++;
    aux_player->status=3;
    return aux_player;
}
//Função que realiza o double.
player* _double(player*aux_player,node**head,int*cartas_restantes)
{
    aux_player->status=4;
    aux_player->aposta=2*aux_player->aposta;
    hit(aux_player,head,cartas_restantes);
    return aux_player;
}
//Função que realiza a leitura do ficheiro com a matriz da estrategia EA e realiza as jogadas de acordo com o indicado nesta.
player* estrategia_EA(player**head_players,player*aux_player,house**casa,node**head,int*cartas_restantes,bool*jogo_terminou,int num_baralhos)
{   
    int i,j;
    char **matriz;
    int x;
    char decisao;
    int valor=0;        
    matriz=(char**)malloc(sizeof(char*)*DIM_L);  // aloca as linhas da matriz
   if((*casa)->top->carta.codigo<=9)
    valor=(*casa)->top->carta.codigo+1;
    if((*casa)->top->carta.codigo==13)
    valor=11;
    else
    valor=10;
       
    
    
    if (matriz == NULL)
    {
        printf ("*** Erro: Memoria Insuficiente!!!***\n");
        exit (EXIT_FAILURE);
    }

    // aloca as colunas da matriz 

    for(x=0; x<DIM_L; x++)
    {
        matriz[x]=(char*)malloc(sizeof(char)*DIM_C);
        if(matriz[x]==NULL)
            {
                  printf ("** Erro: Memoria Insuficiente!!!\n");
                  exit (EXIT_FAILURE);
            }
    }

    FILE *fp ;
    fp = fopen("EA.txt","r");

    if(fp==NULL)
    {
        printf("Erro ao abrir o arquivo EA.\n");
        exit (EXIT_FAILURE);
    }
    else
    {
        
        for(i=0;i<DIM_L;i++)
        {
            for(j=0;j<DIM_C;j++)
            {
                fscanf(fp,"%c%*c", &matriz[i][j]);
            }
        }
    }
  
fclose(fp);

// SE NÃO TIVER NENHUM ÁS
if (aux_player->top->carta.codigo!=13)
{
     for (i=4;i<=17;i++)
    {
        for (j=2; j<=11; j++)
        {  
            while (aux_player->pontos==i && valor==j)
            {
                decisao=matriz[i-4][j-2];
                printf("%c",decisao);
                fflush(stdout);

                switch (decisao)
                {
                    case 'H':
                         aux_player= H(casa,head_players,aux_player,head,cartas_restantes,jogo_terminou,num_baralhos);  
                         
                         break;
                    case 'S':
                       aux_player= S(casa,head_players,aux_player,head,cartas_restantes,jogo_terminou,num_baralhos);   
                       ;    
                        break;
                    case 'D':
                        if (aux_player->pos_hand==2)
                        {
                            aux_player= D(casa,head_players,aux_player,head,cartas_restantes,jogo_terminou,num_baralhos); 
                           
                        }
                        else
                        {
                             aux_player= H(casa,head_players,aux_player,head,cartas_restantes,jogo_terminou,num_baralhos); 
                             
                        }
                        break;
                    case 'R':
                        if (aux_player->pos_hand==2)
                        {
                          aux_player= R(casa,head_players,aux_player,head,cartas_restantes,jogo_terminou,num_baralhos);  
                        }

                        else
                        {
                           aux_player= H(casa,head_players,aux_player,head,cartas_restantes,jogo_terminou,num_baralhos); 
                           
                        }
                    default:
                        break;

                }
            }

            while (aux_player->pontos>17 &&aux_player->pontos<=21)
            {
              aux_player= S(casa,head_players,aux_player,head,cartas_restantes,jogo_terminou,num_baralhos);   
              
            }

        }
    }
}


// SE TIVER UM ÁS
if (aux_player->top->carta.codigo==13)
{
    for (i=13;i<=19;i++)
    {
        for (j=2; j<=11; j++)
        {
            while (aux_player->pontos==i &&aux_player->pontos==j)
            {

                decisao=matriz[i+1][j-2];

                switch (decisao)
                {
                    case 'H':
                               aux_player= H(casa,head_players,aux_player,head,cartas_restantes,jogo_terminou,num_baralhos); 
                           break;
                    case 'S':
                           aux_player= S(casa,head_players,aux_player,head,cartas_restantes,jogo_terminou,num_baralhos);  
                                
                       break;
                    case 'D':
                        if (aux_player->pos_hand==2)
                        {
                          aux_player= D(casa,head_players,aux_player,head,cartas_restantes,jogo_terminou,num_baralhos); 
                          
                        }
                        else
                        {
                            if ((*casa)->pontos>=3 && (*casa)->pontos<=6)
                            {
                               aux_player= S(casa,head_players,aux_player,head,cartas_restantes,jogo_terminou,num_baralhos);  
                             
                            }
                            else
                            {
                               aux_player= H(casa,head_players,aux_player,head,cartas_restantes,jogo_terminou,num_baralhos);   
                              
                            }
                            }
                        
                        break;

                    default:
                      
                        break;
                }
            }

            while (aux_player->pontos>19 && aux_player->pontos<=21)
            {
               aux_player= S(casa,head_players,aux_player,head,cartas_restantes,jogo_terminou,num_baralhos); 
              
            }

            if (aux_player->pontos==12)
            {
                 aux_player= H(casa,head_players,aux_player,head,cartas_restantes,jogo_terminou,num_baralhos);  
                 
            }
        }
       
}


//Liberta a matriz EA

for (x=0; x<DIM_L; x++)
{
    free (matriz[i]);   // liberta as linhas da matriz
}

free (matriz);      // liberta a matriz

}
 return aux_player;
}
//Função para realizar o hit dentro da função main, contém todas as hipoteses possiveis.
player* H(house**casa,player**head_players,player*aux_player,node**head,int*cartas_restantes,bool*jogo_terminou,int num_baralhos)
{

    if(aux_player->status==0)
        aux_player=hit(aux_player,head,cartas_restantes);

    if((aux_player->next==NULL)&&(aux_player->status==2||aux_player->pontos==21))
     {
        (*casa)->pos_hand=2;
        *casa=hit_casa(*casa,head,cartas_restantes,num_baralhos);
        calcula_dinheiro(head_players,casa);
        *jogo_terminou=true;
     }
     else
        if(aux_player->status==2||aux_player->pontos==21)
            aux_player=aux_player->next; 
       
   if(*cartas_restantes==0)
    {
        *head=NULL;
        *head=preenche_lista_cartas(*head,num_baralhos);
        *cartas_restantes=52*num_baralhos;
    }      

    return aux_player;
}
//Função para realizar o stand dentro da função main, contém todas as hipoteses possiveis.
player* S(house**casa,player**head_players,player*aux_player,node**head,int*cartas_restantes,bool*jogo_terminou,int num_baralhos)
{
    if(aux_player->next!=NULL)
    {
        aux_player=aux_player->next;
    }
    else
        if(aux_player->next==NULL)
        {
            (*casa)->pos_hand=2;
            *casa=hit_casa(*casa,head,cartas_restantes,num_baralhos);
            calcula_dinheiro(head_players,casa);
            *jogo_terminou=true;
        }
    
        return aux_player;
}
//Função para realizar o double dentro da função main, contém todas as hipoteses possiveis.
player* D(house**casa,player**head_players,player*aux_player,node**head,int*cartas_restantes,bool*jogo_terminou,int num_baralhos)
{
    if(*jogo_terminou==false&&aux_player->hit==false)
    {
         aux_player=_double(aux_player,head,cartas_restantes);

    if((aux_player->next==NULL))
    {
        (*casa)->pos_hand=2;
        *casa=hit_casa(*casa,head,cartas_restantes,num_baralhos);
        calcula_dinheiro(head_players,casa);
        *jogo_terminou=true;
    }
    if(aux_player->next!=NULL)
         aux_player=aux_player->next;
    }
    
    if(*cartas_restantes==0)
    {
         *head=NULL;
        *head=preenche_lista_cartas(*head,num_baralhos);
        *cartas_restantes=52*num_baralhos;
    }
    return aux_player;
}
//Função para realizar o surrender dentro da função main, contém todas as hipoteses possiveis.
player* R(house**casa,player**head_players,player*aux_player,node**head,int*cartas_restantes,bool*jogo_terminou,int num_baralhos)
{
    
    if(*jogo_terminou==false&&aux_player->hit==false)
    {
         aux_player=surrender(aux_player);

    if((aux_player->next==NULL))
    {
        (*casa)->pos_hand=2;
        *casa=hit_casa(*casa,head,cartas_restantes,num_baralhos);
        calcula_dinheiro(head_players,casa);
        *jogo_terminou=true;
    }
    if(aux_player->next!=NULL)
         aux_player=aux_player->next;
    }
    
    return aux_player;
}

void clean_list_player(player **_head)
{
    player *aux = *_head;
    player *tmp = NULL;

    while(aux != NULL)
    {
        tmp=aux;
        aux=aux->next;
        free(tmp);
    }
*_head=NULL;
}
  
