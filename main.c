#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>
#include <time.h>

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
#include "version.h"
#include "time.h"

int main()
{
    char s[256];
	int movecnt = 0;
    MOVE moveBuf[200];
    MOVE bestMove;
    int type;
    int i;
    U64 count;

    printf("IsaBB , version beta : %d.%d%s\n",MAJOR,MINOR,STATUS_SHORT);
    printf("Date : %s/%s/%s \n",DATE,MONTH,YEAR);

	init();
	affiche_echiquier();
	affiche_options();
	show_atacks = FAUX;
	bump_tree = FAUX;

	for(;;)
	{
        //OPTION WB
        if (side == computer_side)     // programme joue
        {
            //recherche du meilleur coup a profondeur max_depth
            bestMove = engine(max_depth, 1);
            //show_move = VRAI;
            //show_atacks = VRAI;
            do_move(bestMove);
            affiche_echiquier();
            affiche_resultat();
            //side = OPP(side);
            printf("Isa joue : %s \n",print_move(bestMove.from,bestMove.dest));
            //show_move = FAUX;
            //show_atacks = FAUX;
            continue;
        }
        if (scanf("%s", s) == EOF)  //ferme le programme
            return 0;        
        if (!strcmp(s, "xboard"))   //passe en mode GUI
        {
            xboard();
            continue;
        }
        if (!strcmp(s, "quit"))     //quitte l'application
        {
            printf("bye ! ;-) \n");
            return 0;
        }
        if (!strcmp(s, "new"))      //commence une nouvelle partie
        {
            init();
			affiche_echiquier();
			affiche_options();
			continue;
        }
        if (!strcmp(s, "undo"))      //commence une nouvelle partie
        {
            if(hdp)
				undo_move();
			affiche_echiquier();
			affiche_options();
			computer_side = NONE;
			continue;
        }
        if(!strcmp(s, "d"))
		{
			affiche_echiquier();
			affiche_options();
			continue;
		}
		if (!strcmp(s, "sd"))       //profondeur fixe
        {
            scanf("%d", &max_depth);
            time_left = 2000000;
            continue;
        }
        if (!strcmp(s, "st"))       //jeu chronométré
        {
            scanf("%ld", &time_left);
            //temps_max *= 1000;
            max_depth = 100;
            continue;
        }
        if (!strcmp(s, "go"))       //engine color
        {
            computer_side = side ;
            affiche_resultat();
            continue;
        }
        if (!strcmp(s, "stop"))       //engine color
        {
            computer_side = NONE ;
            continue;
        }
        if (!strcmp(s, "eval"))       //jeu chronométré
        {
            int score;
            e_valide = VRAI;
            score = eval();
            e_valide = FAUX;
            continue;
        }
        //OPTIONS DE DEBUG------------------------------------------------------------------------------------------------
        if (!strcmp(s, "edit"))
        {
            edit_pour_debug();
            affiche_options();
            continue;
        }
		if (!strcmp(s, "gen"))
        {
			int i;
			printf("Gen souhaite (1 = Tout coups , 2 = Captures)\n");
			scanf("%d",&type);  //1 = gen , 2 = gen captures
			if(type == 1)
				movecnt = gen_coups(side, moveBuf);
            else
			    movecnt = gen_captures(side, moveBuf);
			affiche_liste(movecnt, moveBuf);
			affiche_options();
            continue;
        }
        if (!strcmp(s, "genl"))
        {
            movecnt = gen_coups(side, moveBuf);
            for(i=0;i<movecnt;++i)
            {
                if(do_move(moveBuf[i]))
                    continue;
                printf("%s  : \n",print_move(moveBuf[i].from,moveBuf[i].dest));
                undo_move();
            }
			affiche_liste(movecnt, moveBuf);
			affiche_options();
            continue;
        }
        if (!strcmp(s, "genx"))
        {
            movecnt = gen_captures(side, moveBuf);
            for(i=0;i<movecnt;++i)
            {
                if(do_move(moveBuf[i]))
                    continue;
                printf("%s  : \n",print_move(moveBuf[i].from,moveBuf[i].dest));
                undo_move();
            }
			affiche_liste(movecnt, moveBuf);
			affiche_options();
            continue;
        }
        if (!strcmp(s, "draw"))
        {
            draw_recognizer();
            affiche_options();
            continue;
        }
        if (!strcmp(s, "atk"))
        {
            show_atacks = VRAI;
            printf("attaques blancs : %" PRIx64 "\n",get_w_atk());
			printf("attaques noirs  : %" PRIx64 "\n",get_b_atk());
			affiche_options();
			show_atacks = FAUX;
            continue;
        }
        if (!strcmp(s, "bit"))
        {
            U64 bitboard = 0x0;
            //int i;
            //scanf("%d",&i);
            scanf("%" PRIx64 "",&bitboard);
			afficher_bitboard(bitboard);
			affiche_options();
            continue;
        }
        if (!strcmp(s, "bitboard"))
        {
            affiche_bitboards();
            continue;
        }
        if (!strcmp(s, "perft"))
        {
			scanf ("%d", &max_depth);
			for(i=1;i<=max_depth;++i)
			{
				debut = get_ms();
				count = perft(i);
            	fin = get_ms();
            	printf ("Perft %d	Noeuds = %" PRId64 "",i, count);
            	printf ("	Temps  = %d ms\n", fin-debut);
			}
            bump_tree = FAUX;
            continue;
    	}
    	if (!strcmp(s, "divide"))
        {
			scanf ("%d", &max_depth);
			debut = get_ms();
			count = divide(max_depth);
           	fin = get_ms();
           	printf ("Noeuds = %" PRId64 "  (temps : %d ms)\n",count,fin-debut);
			continue;
        }
        if (!strcmp(s, "tri"))
        {
            trier();
            continue;
        }
    	int coup_legal;
        coup_legal = verif_coup(s);
        if(coup_legal)
        {
            affiche_echiquier();
            affiche_resultat();
            //affiche_coups_joues();
            //printf("ALP : %" PRIx64 "  NOP : %" PRIx64 " \n ",bb[ALP],bb[NOP]);
        }
        else
        	printf("Illegal move \n");

        continue;
	}

    return 0;
}
