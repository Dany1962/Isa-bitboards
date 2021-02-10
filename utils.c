#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>
#include <time.h>
#include <sys/timeb.h>

#include "bitboard.h"
#include "inits.h"
#include "gen.h"
#include "board.h"
#include "edit.h"
#include "eval.h"
#include "utils.h"
#include "move.h"
#include "engine.h"
#include "hash.h"
#include "time.h"

char *coord_une_case(int sq)
{
    static char str[3];

    sprintf(str,"%c%d",COL(sq) + 'a', ROW(sq)+1);

    return str;
}

char *print_move(int f, int t)
{
	static char move[256] = "";

	if(echiquier[t] != VIDE)
		sprintf(move,"%c%d%c%d",COL(f) + 'a', ROW(f)+1,COL(t) + 'a', ROW(t)+1);
	else
		sprintf(move,"%c%d%c%d",COL(f) + 'a', ROW(f)+1,COL(t) + 'a', ROW(t)+1);

	return move;
}

void affiche_echiquier()
{
	char pieces[] = " INBRQK|nbrqk";
    int i,j;
    U64 hash_scratch;

    printf("   +---+---+---+---+---+---+---+---+\n");
	for(i=56;i>=0;i-=8)
    {
		//printf("%d   ",i);
		printf(" %d |",ROW(i)+1);
		for(j=i;j<=(i+7);++j)
		{
			//printf("%d ",j);
			if(echiquier[j] == 0)
				printf("   |");
			else
				printf(" %c |",pieces[echiquier[j]]);
		}
		printf("\n");
		printf("   +---+---+---+---+---+---+---+---+\n");
	}

	    printf("     a   b   c   d   e   f   g   h\n\n");

	   /* for(i=56;i>=0;i-=8)
	    {
	    	for(j=i;j<=i+7;++j)
	    	{
	    		printf("%2d ",echiquier[j]);
			}
			printf("\n");
		}*/  
	printf("phase         : %d\n",phase());
	printf("castle        : %d\n",castle);
	printf("side   	      : %s\n",(side == BLANC ? "Blanc" : "Noir"));
	printf("computer side : %s\n",(computer_side == BLANC ? "Blanc" : "Noir"));
	printf("hdp           : %d\n",hdp);
	printf("50 moves rule : %d\n",cinquante);
	printf("hash code     : %" PRIx64 "\n\n",hash_position);
	hash_scratch = init_code_position();
	printf("hash scratch  : %" PRIx64 "\n\n",hash_position);
	printf("Positions_testees  : %10d\n",positions_testees);
    printf("Positions_trouvees : %10d\n",positions_trouvees);
    printf("Positions_sauvees  : %10d\n",positions_sauvees);
	position_to_fen();
}

void affiche_petit_echiquier()
{
	char pieces[] = " INBRQK|nbrqk";
    int i,j;

    printf("   Isa BitBoards : Version dev (0.00)\n\n");

	for(i=56;i>=0;i-=8)
    {
		//printf("%d   ",i);
		printf("%d ",ROW(i)+1);
		for(j=i;j<=(i+7);++j)
		{
			if(echiquier[j] == 0)
				printf(".");
			else
				printf("%c",pieces[echiquier[j]]);
		}
		printf("\n");
	}

	    printf("  abcdefgh\n\n");

	printf("side   	      : %d\n",side);
	printf("computer side : %d\n",computer_side);
}


void affiche_options()
{
	/*printf("\n\n\n");
	printf("OPTIONS  :\n");
	printf("-------------------------------------------------------------------------\n");
	printf("quit  : Quitte l'application \n");
	printf("new   : Commence une nouvelle partie\n");
	printf("undo  : Anulation d'un coup\n");
	printf("d     : Affiche l'echiquier\n");
	printf("sd 'n': Parametre le moteur sur 'n' coups de profondeur\n");
	printf("st 'n': Parametre le moteur sur 'n' secondes par coups\n");
	printf("go    : Le moteur joue avec la couleur en cours\n");
	printf("stop  : Le moteur arrete de jouer\n");
	printf("eval  : Communique l'evaluation statique de la position , en centi-pions\n");
	printf("-------------------------------------------------------------------------\n");
	printf("edit  : Permet d'entrer une position au choix \n");
	printf("gen   : Genere les coups pseudos légaux pour la couleur en cours\n");
	printf("genl  : Genere les coups légaux pour la couleur en cours\n");
	printf("genx  : Genere les captures pour la couleur en cours\n");
	printf("draw  : Communique si une position de nule FIDE est trouvée\n");
	printf("atk   : Communique les attaques de chaque pièces de chaque couleur\n");
	printf("bit   : Affiche un long long en échiquier (0 et 1)\n");
	printf("perft 'n' : Genere le nombre de coups légaux a profondeur 'n'\n");
	printf("divide 'n': Genere le nombre de coups pour chaque 'root moves'\n");
	printf("tri   : Genere une liste de coups , l'evalue et la trie (test de la vitesse de tri)\n");
	printf("-------------------------------------------------------------------------\n");*/
	printf("\n\n\n");
	printf("OPTIONS  :\n");
	printf("-------------------------------------------------------------------------\n");
	printf("quit  : Leave the application \n");
	printf("new   : Starting a new game\n");
	printf("undo  : Undo a move\n");
	printf("d     : Display chessboard\n");
	printf("sd 'n': Search with fixed depth to 'n' moves\n");
	printf("st 'n': Search with fixed time in 'n' miliseconds''\n");
	printf("go    : Engine play with the curent side\n");
	printf("stop  : Engine stop\n");
	printf("eval  : Display the static evaluation of the current position\n");
	printf("-------------------------------------------------------------------------\n");
	printf("edit  : Edit a position \n");
	printf("gen   : Generate the pseudos legal moves for the current position\n");
	printf("genl  : Generate the legal moves for the curent position\n");
	printf("genx  : Generate the captures for the curent position\n");
	printf("draw  : Show if a fide draw is found\n");
	printf("atk   : Show each atacks of each pieces of each color\n");
	printf("bit   : display a long long in a chessboard form\n");
	printf("perft 'n' : Genere le nombre de coups légaux a profondeur 'n'\n");
	printf("divide 'n': Genere le nombre de coups pour chaque 'root moves'\n");
	printf("tri   : Generate a move list , sort it and display it\n");
	printf("-------------------------------------------------------------------------\n");
}

void affiche_liste(int ctr, MOVE *pbuf)
{
	int i;
	int f,t;

	printf("--------------------------------------------------------------------|\n");
	printf("index | from    to    pfrom    pto    type    ep    alpha    score  |\n");
	printf("--------------------------------------------------------------------|\n");

	for(i=0;i<ctr;++i)
	{
		f = pbuf[i].from;t = pbuf[i].dest;
		printf(" %2d   |  %2d     %2d     %2d      %3d     %2d     %2d    %s     %3d |\n",i,
		pbuf[i].from,pbuf[i].dest,pbuf[i].piece_from,pbuf[i].piece_dest,pbuf[i].type,pbuf[i].ep_flag,
		print_move(f, t),pbuf[i].evaluation);
	}

	printf("--------------------------------------------------------------------|\n");

}

void affiche_bitboards()
{
	printf("Vide : %" PRIx64 "\n",bb[0]);
	printf("WP   : %" PRIx64 "\n",bb[1]);
	printf("WN   : %" PRIx64 "\n",bb[2]);
	printf("WB   : %" PRIx64 "\n",bb[3]);
	printf("WR   : %" PRIx64 "\n",bb[4]);
	printf("WQ   : %" PRIx64 "\n",bb[5]);
	printf("WK   : %" PRIx64 "\n",bb[6]);
	printf("-----------------------------------------\n");
	printf("BP   :                   %" PRIx64 "\n",bb[7]);
	printf("BN   :                   %" PRIx64 "\n",bb[8]);
	printf("BB   :                   %" PRIx64 "\n",bb[9]);
	printf("BR   :                   %" PRIx64 "\n",bb[10]);
	printf("BQ   :                   %" PRIx64 "\n",bb[11]);
	printf("BK   :                   %" PRIx64 "\n",bb[12]);
	printf("------------------------------------------\n");
	printf("WPCE : %" PRIx64 "\n",bb[13]);
	printf("BPCE :                   %" PRIx64 "\n",bb[14]);
	printf("-------------------------------------------\n");
	printf("ALP  :          %" PRIx64 "\n",bb[15]);
	printf("NOP  :          %" PRIx64 "\n",bb[16]);


}

void afficher_bitboard(U64 bitboard)
{
	int i,j,sq;
	int bit_tab[64];
	U64 x = bitboard;

	memset(bit_tab, 0, sizeof(bit_tab));

	printf("%" PRIx64 "\n",bitboard);

	while(x)
	{
		sq = LSB(x);
		bit_tab[sq] = 1;
		x &= (x - 1);
	}

	for(i=56;i>=0;i-=8)
	{
		for(j=i;j<=i+7;++j)
		{
			printf("%d ",bit_tab[j]);
		}
		printf("\n");
	}
	printf("\n");
}

int verif_coup(char *s)
{
    int from,to;
    MOVE moveBuf[200];
    int movecnt;
    int i;

    from  = s[0] - 'a';
    from += 8 * (s[1] - '1');
	to    = s[2] - 'a';
    to   += 8 * (s[3] - '1');

    prof = 0;
    movecnt = gen_coups(side, moveBuf);

    for (i = 0; i < movecnt; i++)
        if (moveBuf[i].from == from && moveBuf[i].dest == to)
        {
            if (moveBuf[i].type > GRAND_ROQUE)      /* Promotion move? */
            {
				switch (s[4])
                {
                case 'q':
                    moveBuf[i].type = Q_PROMO;
                    break;

                case 'r':
                    moveBuf[i].type = R_PROMO;
                    break;

                case 'b':
                    moveBuf[i].type = B_PROMO;
                    break;

                case 'n':
                    moveBuf[i].type = N_PROMO;
                    break;
                default:
                    moveBuf[i].type = Q_PROMO;
                }
            }
            if (do_move(moveBuf[i]))
                return FAUX;
            else
                return VRAI;
            break;
        }
    return 0;
}

void affiche_resultat()
{
    MOVE moveBuf[200];
    int movecnt;
    int i;
    int legal = FAUX;
    int in_check;

    //on vérifie si la partie est gagnée par un joueur ou nulle
    movecnt = gen_coups(side, moveBuf);

    for (i = 0; i < movecnt; ++i)
    {
        if (do_move(moveBuf[i]))
        {
            continue;
        }
        legal = VRAI;
        undo_move();
        break;
    }

    if(!legal)
    {
        in_check = king_atacked(side);
        if(in_check)
        {
            if (side == BLANC)
            {
                printf("0-1 {Les blancs sont mat}\n");
                computer_side = NONE;
            }
            else
            {
                printf("1-0 {Les noirs sont mat}\n");
                computer_side = NONE;
            }
        }
        else
        {
            printf("1/2-1/2 {Pat}\n");
            computer_side = NONE;
        }
    }

    //on vérifie la règle de triple réptition
    if(reps() >= 3)
    {
        printf("La partie est nulle par triple repetition\n");
        computer_side = NONE;
    }

    //on vérifie la règle des 50 coups
    if(cinquante >= 100)
    {
        printf("La partie est nulle par la règle des cinquante coups\n");
        computer_side = NONE;
    }

   //on vérifie la nulle par matériel insufisant
   // if(draw_recognizer())
   //    printf("La partie est nulle par materiel insufisant\n");
}

U64 perft (int p)
{
    int i;
    int movecnt;
    U64 nodes = 0;

    if (!p)
        return 1;
    MOVE moveBuf[200];
    movecnt = gen_coups(side, moveBuf);
    for (i = 0; i < movecnt; ++i)
    {
        if (do_move(moveBuf[i]))
            continue;
       /* if(bump_tree)
        {
        	if(prof == 1)
        		printf("%s  (nodes = %d)\n",print_move(moveBuf[i].from,moveBuf[i].dest),nodes);
        	else if(prof == 2)
        		printf("   %s  (nodes = %d)\n",print_move(moveBuf[i].from,moveBuf[i].dest),nodes);
        	else if(prof == 3)
        		printf("       %s   (nodes = %d)\n",print_move(moveBuf[i].from,moveBuf[i].dest),nodes);
        	else
        		printf("            %s  (nodes = %d)\n",print_move(moveBuf[i].from,moveBuf[i].dest),nodes);
		}
		if(moveBuf[i].type == PETIT_ROQUE || moveBuf[i].type == GRAND_ROQUE)
		{
			if(side == BLANC)
				w_castle++;
			else
				b_castle++;
		}*/
        nodes += perft (p - 1);
        undo_move();
    }
    return nodes;
}

U64 divide (int p)
{
	int i;
    int movecnt;
    U64 nodes = 0;
    U64 nodes_divide = 0;
    int valid = 0;

    if (!p)
        return 1;

    MOVE moveBuf[200];
    movecnt = gen_coups(side, moveBuf);

    for (i = 0; i < movecnt; ++i)
    {
    	if (do_move(moveBuf[i]))
            continue;
        ++valid;
        printf("%s ",print_move(moveBuf[i].from,moveBuf[i].dest));
        nodes_divide += perft (p - 1);
		printf("%" PRId64 "\n",nodes_divide);
        undo_move();
        nodes += nodes_divide;
        nodes_divide = 0;
    }
	printf("moves : %d\n",valid);
    return nodes;
}

void position_to_fen()
{
	int x, y, l=0, i=0, sq;
    char  ROW[8];
    char string[256];

    strcpy(string,"");

    for (y=7; y>=0; y--)
    {
        i=l=0;
        strcpy(ROW,"");
        for (x=0; x<8; x++)
        {
            sq = (y*8)+x;
            if (echiquier[sq] == VIDE) l++;
            else
            {
                if (l>0)
                {
                    ROW[i] = (char) (l+48);
                    i++;
                }
                l=0;
                switch(echiquier[sq])
                {
                	case WP: ROW[i]='P';break;
					case WN: ROW[i]='N';break; 
                	case WB: ROW[i]='B';break;
					case WR: ROW[i]='R';break;
					case WQ: ROW[i]='Q';break;
					case WK: ROW[i]='K';break;
					case BP: ROW[i]='p';break;
					case BN: ROW[i]='n';break; 
                	case BF: ROW[i]='b';break;
					case BR: ROW[i]='r';break;
					case BQ: ROW[i]='q';break;
					case BK: ROW[i]='k';break;
					default : break;	
				}
                i++;
            }
        }
        if (l>0)
        {
            ROW[i] = (char) (l+48);
            i++;
        }
        strncat(string,ROW,i);
        if (y<7) strcat(string,"/");
    }

    if (side == BLANC)
    {
        strcat(string," w ");
    }
    else
    {
        strcat(string," b ");
    }


    if(castle & 1)
    {
        strcat(string,"K");
    }
    if(castle & 2)
    {
        strcat(string,"Q");
    }
    if(castle & 4)
    {
        strcat(string,"k");
    }
    if(castle & 8)
    {
        strcat(string,"q");
    }
    if(!castle)
        strcat(string,"-");

    strcat(string," ");

    if (ep == -1)
    {
        strcat(string,"-");
    }
    else
    {
       strcat(string,coord_une_case(ep));
    }

    strcat(string," ");

    printf("%s\n",string);
}

void fen_to_position(char *ptab)
{
	int i;
	int long_string = strlen(ptab);
	int sq = -1;

	for(i=0;i<=long_string;++i)
	{
		switch(ptab[i])
		{
			case 'p'	:++sq;echiquier[sq] = BP;break;
			case 'n'	:++sq;echiquier[sq] = BN;break;
			case 'b'	:++sq;echiquier[sq] = BF;break;
			case 'r'	:++sq;echiquier[sq] = BR;break;
			case 'q'	:++sq;echiquier[sq] = BQ;break;
			case 'k'	:++sq;echiquier[sq] = BK;break;
			case 'P'	:++sq;echiquier[sq] = WP;break;
			case 'N'	:++sq;echiquier[sq] = WN;break;
			case 'B'	:++sq;echiquier[sq] = WB;break;
			case 'R'	:++sq;echiquier[sq] = WR;break;
			case 'Q'	:++sq;echiquier[sq] = WQ;break;
			case 'K'	:++sq;echiquier[sq] = WK;break;
			case '1'	:++sq;break;
			case '2'	:sq += 2;break;
			case '3'	:sq += 3;break;
			case '4'	:sq += 4;break;
			case '5'	:sq += 5;break;
			case '6'	:sq += 6;break;
			case '7'	:sq += 7;break;
			case '8'	:sq += 8;break;
			case '/'	:break;
			default		:break;
		}
		if(sq == 63)
			break;
	}
}

int timeout()
{
    clock_t temp;

    fin = get_ms();
    
    if((fin-debut) >= time_left)
        return VRAI;
    return FAUX;

}

void trier()
{
    MOVE moveBuf[200];
    int movecnt;
    int i;
    int alpha = -MATE;
    int beta  = MATE;

    movecnt = gen_coups(side, moveBuf);

    for(i=0;i<movecnt;++i)
    {
        if(do_move(moveBuf[i]))
        {
            moveBuf[i].evaluation = -20000;
            continue;
        }
        moveBuf[i].evaluation = short_search(alpha, beta, 3);;
        undo_move();
    }
    affiche_liste(movecnt, moveBuf);
    clock_t begin = clock();
    tri(movecnt, moveBuf);
    //triRapid(moveBuf, 0, movecnt-1);
    clock_t end = clock();
    affiche_liste(movecnt, moveBuf);
    unsigned long millis = (end - begin) * 1000 / CLOCKS_PER_SEC;
    printf("time : %ld\n",millis);
}

void affiche_coups_joues()
{
    int i;

    for(i=0;i<hdp;++i)
    {
        printf("%s    %" PRIx64 " \n",print_move(hist[i].m.from,hist[i].m.dest),hist[i].hash_code);
    }
}

