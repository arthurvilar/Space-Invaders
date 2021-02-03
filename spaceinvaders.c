#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <unistd.h>
#include "lib_lista.h"

#define ALIEN1 " O o \n-HHH-\n/\\ /\\\0"
#define ALIEN2 ".v_v.\n}WMW{\n / \\ \0"
#define ALIEN3 " nmn \ndbMdb\n_/-\\_\0"

/*pontuacao de cada alien*/
#define PONTO1A 50
#define PONTO2A 30
#define PONTO3A 10
#define PMAE 100

#define BARREIRA1 "H"

#define NAVEMAE " /MMMMM\\ \nAMoMoMoMA\n \\/'-'\\/ \0"

#define TANQUE " /^\\ \nMMMMM\0"

#define EXPLOSAO " \\'/ \n-   -\n /,\\ \0"
#define MINIEXPLOSAO "\\|/\n/|\\\0"

#define TIRO "|\0" /*tiro do tanque*/
#define BOMBA "$\0" /*tiro dos aliens*/

/*controla a frequencia de aparicao*/
#define FREQMAE 800
#define FREQBOMBA 60

#define VIVO 1
#define MORTO 0

/*controlam a direção do alien*/
#define DIR 1 
#define ESQ -1 


void borda(int y1, int x1, int y2, int x2) {
    
    mvhline(y1, x1, 0, x2-x1);
    mvhline(y2, x1, 0, x2-x1);
    mvvline(y1, x1, 0, y2-y1);
    mvvline(y1, x2, 0, y2-y1);
    mvaddch(y1, x1, ACS_ULCORNER);
    mvaddch(y2, x1, ACS_LLCORNER);
    mvaddch(y1, x2, ACS_URCORNER);
    mvaddch(y2, x2, ACS_LRCORNER);
}

void insere_tanque(t_lista *l_tanque) {

	t_nodo *tanque;
	tanque = (t_nodo *) malloc (sizeof(t_nodo));
	
	tanque->sprite1 = TANQUE;
	tanque->posy = 34;
	tanque->posx = 44;
	tanque->vida = VIVO;
	tanque->tipo = 0;

	insere_lista(tanque, l_tanque);
}

void insere_barreira(t_lista *l_barreira) {

	int lin, col, k;
	t_nodo *barreira;

	for (col = 14; col <= 77; col += 21) {
		for (k = 0; k < 7; k++) {	
			for (lin = 29; lin <= 31; lin++) {
				
				barreira = (t_nodo *) malloc(sizeof(t_nodo));

				barreira->vida = VIVO;
				barreira->posy = lin;
				barreira->posx = col + k;
				barreira->sprite1 = BARREIRA1;

				insere_lista(barreira, l_barreira);
			}
		}
	}
}

void insere_tiro(t_lista *l_tiro, t_lista *l_tanque) {

	t_nodo *tiro;
	tiro = (t_nodo *) malloc(sizeof(t_nodo));

	tiro->vida = VIVO;
	tiro->posy = 33;
	tiro->posx = l_tanque->ini->prox->posx + 2;
	tiro->sprite1 = TIRO;
	tiro->tipo = 0;

	insere_lista(tiro, l_tiro);
}

void insere_aliens(t_lista *l_aliens) {  
	
	t_nodo *alien;
	
	int i, j;  /*i controla a linha inicial de cada alien e j a coluna*/
	
	for (i = 4; i <= 20; i += 4) {
		for (j = 0; j <= 70; j += 7) {
			
			alien = (t_nodo *) malloc(sizeof(t_nodo));
			
			if (i == 4) {                  /*define a aparencia dos aliens*/
				alien->sprite1 = ALIEN1;
				alien->tipo = 1;
			}
			else if ((i == 8) || (i == 12)) {
				alien->sprite1 = ALIEN2;
				alien->tipo = 2;
			}
			else if (i > 12) {
				alien->sprite1 = ALIEN3;
				alien->tipo = 3; 
			}
			
			alien->posy = i;
			alien->posx = j;
			alien->vida = VIVO;
			
			insere_lista(alien, l_aliens);
		}
	}
}

void insere_navemae(t_lista *l_mae) {

	t_nodo *mae;
	mae = (t_nodo *) malloc(sizeof(t_nodo));

	mae->posy = 1;
	mae->posx = 0;
	mae->vida = VIVO;
	mae->sprite1 = NAVEMAE;
	mae->tipo = 0;

	insere_lista(mae, l_mae);
}

void insere_bomba(t_lista *l_bomba, int lin, int col) {

	t_nodo *bomba;
	bomba = (t_nodo *) malloc(sizeof(t_nodo));

	bomba->posy = lin;
	bomba->posx = col;
	bomba->sprite1 = BOMBA;
	bomba->vida = VIVO;
	bomba->tipo = 0;

	insere_lista(bomba, l_bomba);
}

void gera_bombas(t_lista *l_aliens, t_lista *l_bomba) {

	int percorre, i;

	if (l_aliens->tamanho == 0)
		return;

	if (rand() % FREQBOMBA == 1) {
		percorre = rand() % l_aliens->tamanho;
		inicializa_atual_inicio(l_aliens);
		for (i = 1; i < percorre; i++) 
			incrementa_atual(l_aliens);
		insere_bomba(l_bomba, l_aliens->atual->posy+2, l_aliens->atual->posx+2);
	}
}

void print_lista(t_lista *lista, WINDOW *window, int col_sprite) {   

    int i, j, k, lin, col;
    j = 0;

    inicializa_atual_inicio(lista);

    lin = lista->atual->posy;
    col = lista->atual->posx;

    for (i = 1; i <= lista->tamanho; i++) {  
        
        while (lista->atual->sprite1[j] != '\0') {   
           
            if (lista->atual->sprite1[j] == '\n') {
                lin++;
                col -= col_sprite;
            }
            else {
            	wmove(window, lin, col);
            	if (lista->atual->vida == VIVO) {
            		waddch(window, lista->atual->sprite1[j]);
            	}
            	else if (lista->atual->vida == MORTO) {
            		if (lista->atual->sprite1[1] == '\0') {
            			for (k = 0; k < 7; k++) {
            				if (MINIEXPLOSAO[k] == '\n') {
            					lin++;
            					col -= 4;
            				}
            				wmove(window, lin, col);
            				waddch(window, MINIEXPLOSAO[k]);
            				col++;
            			}
            		}
            		waddch(window, EXPLOSAO[j]);
            	}
            	col++;
        	}
        	j++;
        }

        if (lista->atual->vida == MORTO)
        	remove_item_atual(lista);

        if (lista->tamanho > 1) {
        	incrementa_atual(lista);
        	j = 0;
        	lin = lista->atual->posy;
    		col = lista->atual->posx;
    	}
    } 
    wrefresh(window);
}

void move_tiro(t_lista *l_tiro, WINDOW *tiro) {

	int i;

	if (l_tiro->tamanho < 1)
		return;

	inicializa_atual_inicio(l_tiro);

	for (i = 1; i <= l_tiro->tamanho; i++) {
		l_tiro->atual->posy--;
		if (l_tiro->atual->posy == 0)
			remove_item_atual(l_tiro);
		incrementa_atual(l_tiro);
	}
	werase(tiro);
}

void move_bomba(t_lista *l_bomba, WINDOW *enemy) {
	
	int i;

	if (l_bomba->tamanho < 1)
		return;

	inicializa_atual_inicio(l_bomba);

	for (i = 1; i <= l_bomba->tamanho; i++) {
		l_bomba->atual->posy++;
		if (l_bomba->atual->posy == 35)
			remove_item_atual(l_bomba);
		incrementa_atual(l_bomba);
	}
	werase(enemy);
}

void move_mae(t_lista *l_mae, WINDOW *enemy) {

	int i;

	inicializa_atual_inicio(l_mae);

	for (i = 1; i <= l_mae->tamanho; i++) {
		l_mae->atual->posx++;
		if (l_mae->atual->posx + 7 == 97)
			remove_item_atual(l_mae);
		incrementa_atual(l_mae);
	}
	werase(enemy);
}

void move_aliens(t_lista *l_aliens, WINDOW *enemy, int *sentido, int *alive, int *vel) {

	int i;
	int abaixa_r, abaixa_l;

	abaixa_r = 0;
	abaixa_l = 0;

	inicializa_atual_inicio(l_aliens);

	for (i = 1; i <= l_aliens->tamanho; i++) {
		if ((l_aliens->atual->posx + 5) == 98)  
			abaixa_r = 1;
		if (l_aliens->atual->posx == 0) 
			abaixa_l = 1;
		if (l_aliens->atual->posy == 32)
			*alive = 0;
		incrementa_atual(l_aliens);	
	}

	if (abaixa_r == 1 && *sentido == DIR) {
		*sentido = ESQ;
		if (*vel > 2)
			*vel = *vel - 2;
	}
	else if (abaixa_l == 1 && *sentido == ESQ) {
		*sentido = DIR;
		if (*vel > 2)
			*vel = *vel - 2;
	}

	inicializa_atual_inicio(l_aliens);

	for (i = 1; i <= l_aliens->tamanho; i++) {
		if (*sentido == DIR)
			l_aliens->atual->posx++;
		else if (*sentido == ESQ)
			l_aliens->atual->posx--;
		l_aliens->atual->posy = l_aliens->atual->posy + abaixa_r + abaixa_l;
		incrementa_atual(l_aliens);
	}
	werase(enemy);
}

void analisa_colisao_mae(t_lista *elemento, t_lista *tiro, int *pontos) {

	int j;

	inicializa_atual_inicio(tiro);
	inicializa_atual_inicio(elemento);
	
	for (j = 0; j < tiro->tamanho; j++) {
		if (tiro->atual->posx >= elemento->atual->posx && 
			tiro->atual->posx <= elemento->atual->posx+8 && 
			tiro->atual->posy >= elemento->atual->posy && 
			tiro->atual->posy <= elemento->atual->posy+2) {
				remove_item_atual(tiro);
				elemento->atual->vida = MORTO;
				*pontos = *pontos + PMAE;
		}
		incrementa_atual(tiro);
	}
}

void analisa_colisao_barreira(t_lista *elemento, t_lista *tiro) {

	int i, j;

	inicializa_atual_inicio(tiro);
	
	for (j = 0; j < tiro->tamanho; j++) {
		inicializa_atual_inicio(elemento);
		for (i = 0; i < elemento->tamanho; i++) {
			if (tiro->atual->posx >= elemento->atual->posx && 
				tiro->atual->posx <= elemento->atual->posx+4 && 
				tiro->atual->posy >= elemento->atual->posy && 
				tiro->atual->posy <= elemento->atual->posy+2) {
					remove_item_atual(tiro);
			}
			incrementa_atual(elemento);
		}
		incrementa_atual(tiro);
	}
}

void analisa_colisao5x1(t_lista *elemento, t_lista *tiro, int *pontos) {

	int i, j;

	inicializa_atual_inicio(tiro);
	
	for (j = 0; j < tiro->tamanho; j++) {
		inicializa_atual_inicio(elemento);
		for (i = 0; i < elemento->tamanho; i++) {
			if (tiro->atual->posx >= elemento->atual->posx && 
				tiro->atual->posx <= elemento->atual->posx+4 && 
				tiro->atual->posy >= elemento->atual->posy && 
				tiro->atual->posy <= elemento->atual->posy+2) {
					remove_item_atual(tiro);
					elemento->atual->vida = MORTO;	
					if (elemento->atual->tipo == 1)	
						*pontos = *pontos + PONTO1A;
					else if (elemento->atual->tipo == 2)
						*pontos = *pontos + PONTO2A;
					else if (elemento->atual->tipo == 3)
						*pontos = *pontos + PONTO3A;
			}
			incrementa_atual(elemento);
		}
		incrementa_atual(tiro);
	}
}

void analisa_colisao1x1(t_lista *bomba, t_lista *tiro) {

	int i, j;

	inicializa_atual_inicio(tiro);
	
	for (j = 0; j < tiro->tamanho; j++) {
		inicializa_atual_inicio(bomba);
		for (i = 0; i < bomba->tamanho; i++) {
			if (tiro->atual->posx == bomba->atual->posx && 
				tiro->atual->posy == bomba->atual->posy) {
					remove_item_atual(tiro);
					bomba->atual->vida = MORTO;
			}
			else 
			if (tiro->atual->posx == bomba->atual->posx &&
				tiro->atual->posy == bomba->atual->posy - 1) {
					remove_item_atual(tiro);
					bomba->atual->vida = MORTO;
			}
			incrementa_atual(bomba);
		}
		incrementa_atual(tiro);
	}
}


int main() {
   
	/*-----VARIAVEIS-----*/

	int term_l, term_c;  /*parametro para o tamanho do terminal*/
	int key;
	int alive = 1;       /*controla o while*/
	int qnt_aliens;
	int qnt_tiros;
	int time = 0;        /*controla o tempo*/
	int sentido = 1;     /*controla o sentido dos aliens*/
	int level = 1;
	int vel_jogo;        /*velocidade do jogo*/
	int pontos = 0;


	/*-----INICIALIZACAO DAS LISTAS-----*/

	t_lista l_tanque;
	t_lista l_aliens;
	t_lista l_mae;
	t_lista l_bomba;
	t_lista l_tiro;
	t_lista l_score;
	t_lista l_barreira;
	inicializa_lista(&l_tanque);
	inicializa_lista(&l_aliens);
	inicializa_lista(&l_mae);
	inicializa_lista(&l_bomba);
	inicializa_lista(&l_tiro);
	inicializa_lista(&l_score);
	inicializa_lista(&l_barreira);


	/*-----INICIO NCURSES-----*/

	initscr();
	cbreak();               /* desabilita o buffer de entrada */
	noecho();               /* não mostra os caracteres digitados */
	nodelay(stdscr, TRUE);  /* faz com que getch não aguarde a digitação */
	keypad(stdscr, TRUE);   /* permite a leitura das setas */
	curs_set(FALSE);        /* não mostra o cursor na tela */

	
	/*-----DECLARACAO DAS JANELAS-----*/

	WINDOW * player = newwin(36, 98, 1, 1);    /*janela do canhao*/
	WINDOW * enemy = newwin(36, 98, 1, 1);     /*janela dos aliens*/
	WINDOW * mae = newwin(36, 98, 1, 1);       /*janela da nave mae*/
	WINDOW * score = newwin(36, 98, 1, 1);     /*janela do score*/


	/*-----DECLARACAO DAS CORES-----*/
	start_color();
	init_pair(2, COLOR_GREEN, COLOR_BLACK);
	init_pair(3, COLOR_RED, COLOR_BLACK);
	init_pair(4, COLOR_YELLOW, COLOR_BLACK);

	
	/*checa se o terminal tem o tamanho minimo*/
	getmaxyx(stdscr, term_l, term_c); 
	if (term_l < 38 || term_c < 100) {
		endwin();
		printf("O terminal precisa ter no minimo 38 linhas por 100 colunas\n");
		return 0;
	}

	/*cria uma borda para o tabuleiro 38x100*/
	borda(0, 0, 37, 99); 
	refresh();


	/*-----INICIO DO PROGRAMA (WHILE) PRINCIPAL-----*/

	insere_tanque(&l_tanque);

	while (alive) {
		
		insere_aliens(&l_aliens);
		insere_barreira(&l_barreira);
		
		tamanho_lista(&qnt_aliens, &l_aliens);

		l_tanque.ini->prox->posx = 44;

		vel_jogo = 21 - level;

		while (alive && qnt_aliens > 0) {
	
			gera_bombas(&l_aliens, &l_bomba);

			analisa_colisao_mae(&l_mae, &l_tiro, &pontos);
			analisa_colisao_barreira(&l_aliens, &l_barreira);
			analisa_colisao5x1(&l_aliens, &l_tiro, &pontos);
			analisa_colisao5x1(&l_tanque, &l_bomba, &pontos);
			analisa_colisao1x1(&l_bomba, &l_tiro);
			analisa_colisao1x1(&l_bomba, &l_barreira);
			analisa_colisao1x1(&l_barreira, &l_tiro);

			tamanho_lista(&qnt_tiros, &l_tiro);
			tamanho_lista(&qnt_aliens, &l_aliens);

			key = getch();

			/*move o tanque*/
			if (key == ' ') {
				if (qnt_tiros < 3) {
					insere_tiro(&l_tiro, &l_tanque);
				}
			}
			else if (key == KEY_LEFT) {
				l_tanque.ini->prox->posx--;
		    	if(l_tanque.ini->prox->posx < 0)
					l_tanque.ini->prox->posx = 0;
				werase(player);
			}
			else if (key == KEY_RIGHT) {
				l_tanque.ini->prox->posx++;
		    	if(l_tanque.ini->prox->posx > 93)
					l_tanque.ini->prox->posx = 93;
				werase(player);
			}
			else if (key == 'q') {
				endwin();
				exit(1);
			}

			wattron(player, COLOR_PAIR(2));
			wattron(mae, COLOR_PAIR(3));
			wattron(score, COLOR_PAIR(4));

			print_lista(&l_tanque, player, 5);
			print_lista(&l_barreira, enemy, 7);
			print_lista(&l_aliens, enemy, 5);
			print_lista(&l_tiro, enemy, 0);
			print_lista(&l_mae, mae, 9);
			print_lista(&l_bomba, enemy, 0);


			/*controla as velocidades dos elementos da tela*/
			if (rand() % FREQMAE == 1) {
				if (l_mae.tamanho == 0)
					insere_navemae(&l_mae);
			}
			
			if (time % 5 == 0)
				move_mae(&l_mae, enemy);

			if (time % 3 == 0) 
				move_tiro(&l_tiro, enemy);
			
			if (time % 6 == 0) 
				move_bomba(&l_bomba, enemy);
			
			if (time % vel_jogo == 0) 
				move_aliens(&l_aliens, enemy, &sentido, &alive, &vel_jogo);
			

			mvwprintw(score, 0, 45, "score: %d", pontos);
			mvwprintw(score, 0, 88, "level: %d", level);
			wrefresh(score);


			/*se o tanque morreu sai do laço e o jogo acaba*/
			if (l_tanque.ini->prox->vida == MORTO) {
				alive = 0;
				sleep(2);
			}
			
			usleep(16666);
			time++;
		}
		level++;
	}

	werase(player);
	werase(enemy);
	werase(mae);
	werase(score);
	mvwprintw(score, 18, 33, "GAME OVER, PONTUACAO: %d", pontos);
	wrefresh(score);
	sleep(4);
	endwin();
	return 1;
}