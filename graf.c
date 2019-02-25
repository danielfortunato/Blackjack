#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>               //Biblioteca utilizada para colocar a função time.
#include <stdbool.h>
#include <string.h>
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

const char myName[] = "Jose Nobre";
const char myNumber[] = "IST425407";
const char myName2[] = "Daniel Fortunato";
const char myNumber2[] = "IST181498";


void RenderTable(player*_head,player*_player,int num_players, TTF_Font *_font, SDL_Surface *_img[], SDL_Renderer* _renderer)
{
    int n=0;
    SDL_Color black = { 0, 0, 0 }; // black
    SDL_Color white = { 255, 255, 255 }; // white
    char name_money_str[STRING_SIZE];
    SDL_Texture *table_texture;
    SDL_Rect tableSrc, tableDest, playerRect,playerRect2;
    int separatorPos = (int)(0.95f*WIDTH_WINDOW); // separates the left from the right part of the window
    int height;
    player*aux=NULL;
    char _bust[]="BUST";
    char _sur[]="SURRENDER";
    char _blackjack[]="BLACKJACK!!";
    char _double[]="DOUBLE";
    // set color of renderer to some color
    SDL_SetRenderDrawColor( _renderer, 255, 255, 255, 255 );
    char tipo_str[3];
    char EA[3]="EA";
    char HU[3]="HU";

    // clear the window
    SDL_RenderClear( _renderer );

    tableDest.x = tableSrc.x = 0;
    tableDest.y = tableSrc.y = 0;
    tableSrc.w = _img[0]->w;
    tableSrc.h = _img[0]->h;

    tableDest.w = separatorPos;
    tableDest.h = HEIGHT_WINDOW;

    table_texture = SDL_CreateTextureFromSurface(_renderer, _img[0]);
    SDL_RenderCopy(_renderer, table_texture, &tableSrc, &tableDest);

    // render the IST Logo
    height = RenderLogo(separatorPos, 0, _img[1], _renderer);

    // render the student name
    height += RenderText(separatorPos+3*MARGIN, height, myName, _font, &black, _renderer);

    // this renders the student number
    RenderText(separatorPos+3*MARGIN, height, myNumber, _font, &black, _renderer);

    height=height+30;

    height += RenderText(separatorPos+3*MARGIN, height, myName2, _font, &black, _renderer);
    // this renders the student number
    RenderText(separatorPos+3*MARGIN, height, myNumber2, _font, &black, _renderer);
    height=height+30;
    // renders the squares + name for each player
    aux=_head;
    for(n=0;n<MAX_PLAYERS;n++)
    {   
        if(aux->tipo==0)
            strcpy(tipo_str,HU);
            else
                strcpy(tipo_str,EA);
         if(aux->active==true)
         {       
            sprintf(name_money_str,"%s [%s] Money:%d $",aux->name,tipo_str,aux->money);
            RenderText(separatorPos+3*MARGIN, height,name_money_str,_font, &black, _renderer);
            height=height+20;
         }
        aux=aux->next;
    }

    // renders the areas for each player: names and money too !
    aux=_head;
      for(n=0;n<MAX_PLAYERS;n++)
        {
            playerRect.x = n*(separatorPos/4-5)+10;
            playerRect.y = (int) (0.55f*HEIGHT_WINDOW);
            playerRect.w = separatorPos/4-5;
            playerRect.h = (int) (0.42f*HEIGHT_WINDOW);

            if(aux->active==true)
            {
                sprintf(name_money_str,"%s -> %d $ Pontos: %d",aux->name,aux->aposta,aux->pontos);
                RenderText(playerRect.x+20, playerRect.y-30, name_money_str, _font, &white, _renderer);
            }

            SDL_RenderDrawRect(_renderer, &playerRect);
            if(aux!=NULL)
            {
                //Vai realizar o render dos retangulos que indicam o estado do jogador.
                if(aux->status!=0&&aux->active==true)
                        {
                            playerRect2.x = n*(separatorPos/4-5)+10;
                            playerRect2.y = (int) (0.45f*HEIGHT_WINDOW);
                            playerRect2.w = separatorPos/4-5;
                            playerRect2.h = (int) (0.05f*HEIGHT_WINDOW);
                            
                            if(aux->status==1)
                            {
                                SDL_SetRenderDrawColor(_renderer,0,250,0, 255 );
                                SDL_RenderDrawRect(_renderer, &playerRect2);
                                SDL_RenderFillRect(_renderer, &playerRect2);
                                RenderText(playerRect2.x+60, playerRect2.y, _blackjack, _font, &black, _renderer);
                            }
                            if(aux->status==2)
                            {
                                SDL_SetRenderDrawColor(_renderer,200,0,0, 255 );
                                SDL_RenderDrawRect(_renderer, &playerRect2);
                                SDL_RenderFillRect(_renderer, &playerRect2);
                                RenderText(playerRect2.x+75, playerRect2.y, _bust, _font, &black, _renderer);
                            }
                            if(aux->status==3)
                            {
                                SDL_SetRenderDrawColor(_renderer,0,200,100, 255 );
                                SDL_RenderDrawRect(_renderer, &playerRect2);
                                SDL_RenderFillRect(_renderer, &playerRect2);
                                RenderText(playerRect2.x+60, playerRect2.y, _sur, _font, &black, _renderer);
                            }
                            if(aux->status==4)
                            {
                                SDL_SetRenderDrawColor(_renderer,0,250,50, 255 );
                                SDL_RenderDrawRect(_renderer, &playerRect2);
                                SDL_RenderFillRect(_renderer, &playerRect2);
                                RenderText(playerRect2.x+60, playerRect2.y, _double, _font, &black, _renderer);
                            }
                                    
                        }
                        
                if(aux==_player&&aux->active==true)
                {
                   SDL_SetRenderDrawColor(_renderer,255,0,0, 255 );
                   SDL_RenderDrawRect(_renderer, &playerRect);
                      
                }
                else
                {
                   SDL_SetRenderDrawColor(_renderer,255,255,255, 255 );
                   SDL_RenderDrawRect(_renderer, &playerRect);
                }

            }
            if(aux->next!=NULL)
                    aux=aux->next;
        }
    n=0;




           // destroy everything
        SDL_DestroyTexture(table_texture);
}

/**
 * RenderHouseCards: Renders cards of the house
 * \param _house vector with the house cards
 * \param _pos_house_hand position of the vector _house with valid card IDs
 * \param _cards vector with all loaded card images
 * \param _renderer renderer to handle all rendering in a window
 */

void RenderHouseCards(house *_casa,SDL_Surface **_cards, SDL_Renderer* _renderer)
{
    int card, x, y;
    int div = WIDTH_WINDOW/CARD_WIDTH;
    node*aux=NULL;
    aux=_casa->top;

    // drawing all house cards
    for ( card = 0; card < _casa->pos_hand; card++)
    {
        // calculate its position
        x = (div/2-_casa->pos_hand/2+card)*CARD_WIDTH + 15;
        y = (int) (0.26f*HEIGHT_WINDOW);
        // render it !
        RenderCard(x, y,aux->carta.graf_val, _cards, _renderer);
        aux=aux->next;
    }
    // just one card ?: draw a card face down
    if (_casa->pos_hand == 1)
    {
        x = (div/2-_casa->pos_hand/2+1)*CARD_WIDTH + 15;
        y = (int) (0.26f*HEIGHT_WINDOW);
        RenderCard(x, y, MAX_DECK_SIZE, _cards, _renderer);
    }
}

/**
 * RenderPlayerCards: Renders the hand, i.e. the cards, for each player
 * \param _player_cards 2D array with the player cards, 1st dimension is the player ID
 * \param _pos_player_hand array with the positions of the valid card IDs for each player
 * \param _cards vector with all loaded card images
 * \param _renderer renderer to handle all rendering in a window
 */
void RenderPlayerCards(player*_head,TTF_Font *_font, SDL_Surface **_cards, SDL_Renderer* _renderer)
{
    int num_player=0;
    player *aux=NULL;
    node*carta_jogador=NULL;
    int pos, x, y, card;
    // for every card of every player
    aux=_head;
    node*aux_lista=NULL;
    int n=0;


    while (aux!=NULL)
    {
        card=0;
        n=0;
        aux_lista=aux->top;
        while (n<aux->pos_hand&&aux->active==true)
        {
            // draw all cards of the player: calculate its position: only 4 positions are available !
            pos = card % 4;
            x = (int) num_player*((0.95f*WIDTH_WINDOW)/4-5)+(card/4)*12+15;
            y = (int) (0.55f*HEIGHT_WINDOW)+10;
            if ( pos == 1 || pos == 3) x += CARD_WIDTH + 30;
            if ( pos == 2 || pos == 3) y += CARD_HEIGHT+ 10;
            carta_jogador=aux_lista;
            RenderCard(x, y,carta_jogador->carta.graf_val, _cards, _renderer);
            aux_lista=aux_lista->next;
            n++;
            card++;

        }

        aux=aux->next;
        num_player++;
    }

}

/**
 * RenderCard: Draws one card at a certain position of the window, based on the card code
 * \param _x X coordinate of the card position in the window
 * \param _y Y coordinate of the card position in the window
 * \param _num_card card code that identifies each card
 * \param _cards vector with all loaded card images
 * \param _renderer renderer to handle all rendering in a window
 */

void RenderCard(int _x, int _y, int _num_card, SDL_Surface **_cards, SDL_Renderer* _renderer)
{
    SDL_Texture *card_text;
    SDL_Rect boardPos;

    // area that will be occupied by each card
    boardPos.x = _x;
    boardPos.y = _y;
    boardPos.w = CARD_WIDTH;
    boardPos.h = CARD_HEIGHT;

    // render it !
    card_text = SDL_CreateTextureFromSurface(_renderer, _cards[_num_card]);
    SDL_RenderCopy(_renderer, card_text, NULL, &boardPos);

    // destroy everything
    SDL_DestroyTexture(card_text);
}

/**
 * LoadCards: Loads all images of the cards
 * \param _cards vector with all loaded card images
 */
void LoadCards(SDL_Surface **_cards)
{
    int i;
    char filename[STRING_SIZE];

     // loads all cards to an array
    for (i = 0 ; i < MAX_DECK_SIZE; i++ )
    {
        // create the filename !
        sprintf(filename, ".//cartas//carta_%02d.png", i+1);
        // loads the image !
        _cards[i] = IMG_Load(filename);
        // check for errors: deleted files ?
        if (_cards[i] == NULL)
        {
            printf("Unable to load image: %s\n", SDL_GetError());
            exit(EXIT_FAILURE);
        }
    }
    // loads the card back
    _cards[i] = IMG_Load(".//cartas//carta_back.jpg");
    if (_cards[i] == NULL)
    {
        printf("Unable to load image: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }
}


/**
 * UnLoadCards: unloads all card images of the memory
 * \param _cards vector with all loaded card images
 */
void UnLoadCards(SDL_Surface **_array_of_cards)
{
    int i=0;
    // unload all cards of the memory: +1 for the card back
    for ( i= 0 ; i < MAX_DECK_SIZE + 1; i++ )
    {
        SDL_FreeSurface(_array_of_cards[i]);
    }
}

/**
 * RenderLogo function: Renders the IST Logo on the window screen
 * \param x X coordinate of the Logo
 * \param y Y coordinate of the Logo
 * \param _logoIST surface with the IST logo image to render
 * \param _renderer renderer to handle all rendering in a window
 */
int RenderLogo(int x, int y, SDL_Surface *_logoIST, SDL_Renderer* _renderer)
{
	SDL_Texture *text_IST;
	SDL_Rect boardPos;

    // space occupied by the logo
	boardPos.x = x;
	boardPos.y = y;
	boardPos.w = _logoIST->w;
	boardPos.h = _logoIST->h;

    // render it
	text_IST = SDL_CreateTextureFromSurface(_renderer, _logoIST);
	SDL_RenderCopy(_renderer, text_IST, NULL, &boardPos);

    // destroy associated texture !
	SDL_DestroyTexture(text_IST);
	return _logoIST->h;
}

/**
 * RenderText function: Renders the IST Logo on the window screen
 * \param x X coordinate of the text
 * \param y Y coordinate of the text
 * \param text string where the text is written
 * \param font TTF font used to render the text
 * \param _renderer renderer to handle all rendering in a window
 */
int RenderText(int x, int y, const char *text, TTF_Font *_font, SDL_Color *_color, SDL_Renderer* _renderer)
{
	SDL_Surface *text_surface;
	SDL_Texture *text_texture;
	SDL_Rect solidRect;

	solidRect.x = x;
	solidRect.y = y;
    // create a surface from the string text with a predefined font
	text_surface = TTF_RenderText_Blended(_font,text,*_color);
	if(!text_surface)
	{
	    printf("TTF_RenderText_Blended: %s\n", TTF_GetError());
	    exit(EXIT_FAILURE);
	}
    // create texture
	text_texture = SDL_CreateTextureFromSurface(_renderer, text_surface);
    // obtain size
	SDL_QueryTexture( text_texture, NULL, NULL, &solidRect.w, &solidRect.h );
    // render it !
	SDL_RenderCopy(_renderer, text_texture, NULL, &solidRect);

	SDL_DestroyTexture(text_texture);
	SDL_FreeSurface(text_surface);
	return solidRect.h;
}

/**
 * InitEverything: Initializes the SDL2 library and all graphical components: font, window, renderer
 * \param width width in px of the window
 * \param height height in px of the window
 * \param _img surface to be created with the table background and IST logo
 * \param _window represents the window of the application
 * \param _renderer renderer to handle all rendering in a window
 */
void InitEverything(int width, int height, TTF_Font **_font, SDL_Surface *_img[], SDL_Window** _window, SDL_Renderer** _renderer)
{
    InitSDL();
    InitFont();
    *_window = CreateWindow(width, height);
    *_renderer = CreateRenderer(width, height, *_window);

    // load the table texture
    _img[0] = IMG_Load("table_texture.png");
    if (_img[0] == NULL)
    {
        printf("Unable to load image: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    // load IST logo
    _img[1] = SDL_LoadBMP("ist_logo.bmp");
    if (_img[1] == NULL)
    {
        printf("Unable to load bitmap: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }
    // this opens (loads) a font file and sets a size
    *_font = TTF_OpenFont("FreeSerif.ttf", 16);
    if(!*_font)
    {
        printf("TTF_OpenFont: %s\n", TTF_GetError());
        exit(EXIT_FAILURE);
    }
}

/**
 * InitSDL: Initializes the SDL2 graphic library
 */
void InitSDL()
{
    // init SDL library
	if ( SDL_Init( SDL_INIT_EVERYTHING ) == -1 )
	{
		printf(" Failed to initialize SDL : %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
	}
}

/**
 * InitFont: Initializes the SDL2_ttf font library
 */
void InitFont()
{
	// Init font library
	if(TTF_Init()==-1)
	{
	    printf("TTF_Init: %s\n", TTF_GetError());
        exit(EXIT_FAILURE);
	}
}

/**
 * CreateWindow: Creates a window for the application
 * \param width width in px of the window
 * \param height height in px of the window
 * \return pointer to the window created
 */
SDL_Window* CreateWindow(int width, int height)
{
    SDL_Window *window;
    // init window
	window = SDL_CreateWindow( "Blackjack Razoavel", WINDOW_POSX, WINDOW_POSY, width+EXTRASPACE, height, 0 );
    // check for error !
	if ( window == NULL )
	{
		printf("Failed to create window : %s\n", SDL_GetError());
		exit(EXIT_FAILURE);
	}
	return window;
}

/**
 * CreateRenderer: Creates a renderer for the application
 * \param width width in px of the window
 * \param height height in px of the window
 * \param _window represents the window for which the renderer is associated
 * \return pointer to the renderer created
 */
SDL_Renderer* CreateRenderer(int width, int height, SDL_Window *_window)
{
    SDL_Renderer *renderer;
    // init renderer
	renderer = SDL_CreateRenderer( _window, -1, 0 );

	if ( renderer == NULL )
	{
		printf("Failed to create renderer : %s", SDL_GetError());
        exit(EXIT_FAILURE);
	}

	// set size of renderer to the same as window
	SDL_RenderSetLogicalSize( renderer, width+EXTRASPACE, height );

	return renderer;
}


