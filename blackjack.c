#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>               //Biblioteca utilizada para colocar a função time.
#include <stdbool.h>
#include <string.h>
#include "funcoes.h"
#include "graf.h"



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


int main( int argc, char* args[] )
{
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    TTF_Font *serif = NULL;
    SDL_Surface *cards[MAX_DECK_SIZE+1], *imgs[2];
    SDL_Event event;
    int delay = 300;
    int quit = 0;
    int i=0;//Variáveis utilizadas para baralhar as cartas.
    int n=0;//Variáveis de apoio ao calculo dos pontos e na distribuição das cartas.
    bool jogo_terminou=false;
    int num_baralhos=0;
    int num_players=0;
    int cartas_restantes=0;//Variavel auxiliar para a remoção das cartas que indica o número de cartas no baralho.
    srand(time(NULL));
    house* casa=NULL;
    player*head_players=NULL;
    player*aux_player=NULL;
    node *head=NULL;
    node* carta_a_colocar=NULL;

    //Realiza a alocação da memoria para a casa.
    casa=(house*)malloc(sizeof(house));
    //Lê os parametros através do ficheiro txt com estes.
    head_players=ler_ficheiro_parametros(&num_baralhos,&num_players,args[1]);
    cartas_restantes=52*num_baralhos;
    aux_player=head_players;
    //Realiza o preenchimento do mega-baralho das cartas.
    head=preenche_lista_cartas(head,num_baralhos);
   
    i=0;
    aux_player=head_players;
    //Coloca o top das listas das mão dos jogadores a NULL de modo a serem colocadas cartas nestas.
    while(i<4)
    {
       aux_player->top=NULL;
       aux_player=aux_player->next;
       i++;
    }
    
    //Reposição das variáveis necessárias de modo a iniciar um novo jogo.
    i=0;
    n=0;
    head_players = repor_variaveis(head_players);
    casa=repor_casa(casa);
    head_players=repor_jogos(head_players);
    aux_player=head_players;

   	// initialize graphics
	InitEverything(WIDTH_WINDOW, HEIGHT_WINDOW, &serif, imgs, &window, &renderer);
    // loads the cards images
    LoadCards(cards);

    //Distribuição das cartas na primeira ronda.
     aux_player=head_players;
     while(i<num_players)
        {
            while(n<2)
            {
                head=remove_carta(head,&carta_a_colocar,&cartas_restantes);
                aux_player->top=push(aux_player->top,carta_a_colocar->carta);
                aux_player->pontos=conta_pontos(aux_player,carta_a_colocar->carta.codigo);

                if(aux_player->pontos==21)
                        aux_player->status=1;

                cartas_restantes--;
                n++;
            }
            aux_player=aux_player->next;
            n=0;
            i++;

        }
    //Distribuição das cartas da casa.
     while(n<2)
        {
            head=remove_carta(head,&carta_a_colocar,&cartas_restantes);
            casa->top=push(casa->top,carta_a_colocar->carta);
            casa->pontos=conta_pontos_casa(casa,carta_a_colocar->carta.codigo);
            cartas_restantes--;
            if(casa->pontos==21)
                casa->status=1;
            n++;
        }

    aux_player=head_players;
  
        
 	while(quit==0)
    {
        // while there's events to handle
        //Realiza a estratégia e o jogo com jogadores EA:
        if(aux_player->tipo==1)
        {
            aux_player=estrategia_EA(&head_players,aux_player,&casa,&head,&cartas_restantes,&jogo_terminou,num_baralhos);
            continue;
        }
        else
        if((aux_player->status==1||aux_player->active==false)&&aux_player->next!=NULL&&jogo_terminou==false)
            aux_player=aux_player->next;
           else if(aux_player->active==false&&aux_player->next==NULL&&jogo_terminou==false)
               {
                casa->pos_hand=2;
                casa=hit_casa(casa,&head,&cartas_restantes,num_baralhos);
                calcula_dinheiro(&head_players,&casa);
                jogo_terminou=true;
                }
            else
                {
                while( SDL_PollEvent( &event ) )
                {
                    if( event.type == SDL_QUIT )
                    {
                        // quit the program
                        return 0;
                    }
                    else if ( event.type == SDL_KEYDOWN )
                    {
                        switch ( event.key.keysym.sym )
                        {

	                        case SDLK_s:
                                // stand !
                                if(jogo_terminou ==false)
                                {
                                	aux_player= S(&casa,&head_players,aux_player,&head,&cartas_restantes,&jogo_terminou,num_baralhos);     
                                }
		                        break;

	                        case SDLK_h:
		                        // hit !
		                        if(jogo_terminou ==false)
                                {
		                             aux_player= H(&casa,&head_players,aux_player,&head,&cartas_restantes,&jogo_terminou,num_baralhos);                
                                }
                                break;

                            case SDLK_q:
                                    //Sair do jogo caso seja pressionada a tecla "q",cria o ficheiro .txt e realiza a limpeza das listas com memorias alocadas.
                                    if(jogo_terminou==true)
                                    {
                                        ficheiro_txt(head_players,casa);
                                        free(casa);
                                        clean_list(&head);
                                        clean_list_player(&head_players);
                                        return 0;
                                    }

                                break;
                            
                            case SDLK_n:
                                        //Cria um novo jogo.
                                        if(jogo_terminou==true)
                                        {
                                            i=0;
                                            aux_player=head_players;
                                            while(i<4)//Vai limpar as listas com as mãos de cada jogador.
                                              {
                                                clean_list(&aux_player->top);
                                                aux_player=aux_player->next;
                                                i++;
                                              }
                                                clean_list(&casa->top);//Limpa a lista que contem a mão da casa.
                                                casa=repor_casa(casa);//Repoem a
                                                head_players=repor_variaveis(head_players);
                                                aux_player=head_players;
                                                i=0;
                                                n=0;
                                            
                                            while(aux_player!=NULL)
                                                {
                                                    if(aux_player->active==false)
                                                    {
                                                        aux_player=aux_player->next;
                                                        continue;
                                                    }
                                                    
                                                    while(n<2)
                                                    {
                                                        head=remove_carta(head,&carta_a_colocar,&cartas_restantes);
                                                        aux_player->top=push(aux_player->top,carta_a_colocar->carta);
                                                        aux_player->pontos=conta_pontos(aux_player,carta_a_colocar->carta.codigo);
                                                        if(aux_player->pontos==21)
                                                               aux_player->status=1;
                                                        cartas_restantes--;
                                                        if(cartas_restantes==0)
                                                        {
                                                             head=NULL;
                                                             head=preenche_lista_cartas(head,num_baralhos);
                                                             cartas_restantes=52*num_baralhos;
                                                        }
                                                        n++;
                                                    }
                                                aux_player=aux_player->next;
                                                n=0;
                                                i++;
                                                }

                                          while(n<2)
                                                {
                                                    head=remove_carta(head,&carta_a_colocar,&cartas_restantes);
                                                    casa->top=push(casa->top,carta_a_colocar->carta);
                                                    casa->pontos=conta_pontos_casa(casa,carta_a_colocar->carta.codigo);
                                                    cartas_restantes--;
                                                    if(cartas_restantes==0)
                                                        {
                                                             head=NULL;
                                                             head=preenche_lista_cartas(head,num_baralhos);
                                                             cartas_restantes=52*num_baralhos;
                                                        }
                                                    n++;
                                                }
                                            aux_player=head_players;
                                            jogo_terminou=false;
                                        }
                                        break;
                            
                            case SDLK_d:
                                        aux_player= D(&casa,&head_players,aux_player,&head,&cartas_restantes,&jogo_terminou,num_baralhos);             
                                        break;

                            case SDLK_r:
                                        aux_player= R(&casa,&head_players,aux_player,&head,&cartas_restantes,&jogo_terminou,num_baralhos);  
                                        break;
                            case SDLK_b:
                                       if(jogo_terminou==true)
                                       {
                                        head_players=bet(head_players);
                                       }    
	                        default:
		                        break;
                        }
                    }
                }

    }
            // render game table1
            RenderTable(head_players,aux_player,num_players,serif, imgs, renderer);
            // render house cards
            RenderHouseCards(casa, cards, renderer);
            // render player cards
            RenderPlayerCards(head_players,serif,cards, renderer);
            // render in the screen all changes above
            SDL_RenderPresent(renderer);
            // add a delay
            SDL_Delay( delay );

    }

    // free memory allocated for images and textures and close everything including fonts
    UnLoadCards(cards);
    TTF_CloseFont(serif);
    SDL_FreeSurface(imgs[0]);
    SDL_FreeSurface(imgs[1]);
    SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();




return (EXIT_SUCCESS);
}
 
