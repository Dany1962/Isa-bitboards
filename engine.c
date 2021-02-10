#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <inttypes.h>

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

//RECHERCHE MOTEUR D'IA---------------------------------------------------------------------------------------------------
//engine play
MOVE engine(int d, int output)
{
	MOVE m,tmp;
    MOVE moveBuf[200];
    int movecnt;
    int i,j;
    int alpha,beta;
    int illegal = -20000;
    int score = 0;
    clock_t temp;
    U64 nps;

    memset(hh, 0, sizeof(hh));
    memset(tt, 0, sizeof(tt));

    //ITERATIF DEEPENING
    debut = get_ms();

    alpha = -MATE;
    beta = MATE;
    
    positions_testees = 0;
    positions_trouvees = 0;
    positions_sauvees = 0;

    printf("depth  time  score(cp)   nodes   nodes qs\n");
	printf("---------------------------------------------------------------------------\n");
    for(i=1;i<=d;++i)
    {
    	prof = 0;
    	nodes_alpha = 0;
    	nodes_qs   = 0;

    	score = alpha_beta(-MATE, MATE, i, &m);

    	if(timeout())
            break;

    	tmp = m;

		temp = get_ms();
		
		if(output == 1)
		{
			printf(" %3d %5d  %4d  %9" PRId64 "  %9" PRId64 "",i,temp-debut,score,nodes_alpha,nodes_qs,nps);
        	for (j = 0; j < long_pv[0]; ++j)
				printf(" %s", print_move(pv[0][j].from,pv[0][j].dest));
			printf("\n");
		}
		else if(output == 2)
		{
			printf("%d %d %d %9d  ",i,score, (temp-debut)/1000, nodes_alpha+nodes_qs);
            for (j = 0; j < long_pv[0]; ++j)
				printf(" %s", print_move(pv[0][j].from,pv[0][j].dest));
			printf("\n");
			fflush(stdout);
		}

        

    	if((score > 9000) || (score < -9000))
            break;
	}

    /*
    //GENERER LES COUPS DE PROFONDEUR 1
    prof = 0;
    movecnt = gen_coups(side, moveBuf);
    affiche_liste(movecnt, moveBuf);

    alpha = -MATE;
    beta  = MATE;

    //start = get_ms();

    //RECHERCHE PRELIMINAIRE (pronfondeur 4)
    for(i=0;i<movecnt;++i)
	{
		if(do_move(moveBuf[i]))
		{
			moveBuf[i].evaluation = illegal;
			continue;
		}
		//printf("coup P1: %s\n",print_move(moveBuf[i].from,moveBuf[i].dest));
		moveBuf[i].evaluation = short_search(alpha, beta, 4);
		undo_move();
	}

	affiche_liste(movecnt, moveBuf);
	tri(movecnt, moveBuf);
	printf("liste triee \n");
	affiche_liste(movecnt, moveBuf);

	//end = get_ms();

	m = root_search(movecnt, moveBuf, d);
    */
	//printf("%d %d \n",print_move(m.from,m.dest));
	printf("---------------------------------------------------------------------------\n");
	printf("\n\n");
	return tmp;
}

//not yet used
/*
int short_search(int a, int b, int depth)
{
	MOVE moveBuf[200];
    int movecnt;
    int i;
    int score = 0;
    int val;
    int valid = 0;
    int echec;

    if(depth <= 0)
    {
    	val = qs(a, b);
		//printf("           eval : %d (alpha %d , beta %d)\n",val,a,b);
		return val;
	}

    echec = (king_atacked(side));

	movecnt = gen_coups(side, moveBuf);

	for(i=0;i<movecnt;++i)
	{
		if(do_move(moveBuf[i]))
			continue;

		//printf("	coup P2: %s  ",print_move(moveBuf[i].from,moveBuf[i].dest));
		valid++;
		score = -short_search(-b, -a, depth - 1);

		undo_move();

		if(score > a)
		{
			if (score >= b)
				return b;
			a = score;
		}
	}

	if(!valid)
	{
		if(echec)
			return (-MATE + prof);
		return 0;
	}

	return -a;
}
*/

//not yet used
/*
int qs(int a, int b)
{
	int movecnt;
    MOVE moveBuf[200];
    int score;
    int val;
    int i;

    score = eval();

    //stand pat ?
    if(score >= b)
        return b;
    if(a < score)
        a = score;

    movecnt = gen_captures(side, moveBuf);

    for (i = 0; i < movecnt; ++i)
    {
    	if(do_move(moveBuf[i]))
    		continue;

		//printf("\n           coup P3: %s  \n",print_move(moveBuf[i].from,moveBuf[i].dest));

    	val = -qs(-b, -a);

    	undo_move();

    	if(val > a)
    	{
    		if(val >= b)
    			return b;
    		a = val;
		}
    }

    return a;
}*/

//not used
/*
//bubble sort
void tri(int ctr, MOVE *pbuf)
{
	int desordre = 1;
	int j;
	MOVE tmp;

	while(desordre)
	{
		desordre = 0;
		for(j=0;j<ctr-1;j++)
		{
			if(pbuf[j].evaluation < pbuf[j+1].evaluation)
			{
				tmp = pbuf[j+1];
				pbuf[j+1] = pbuf[j];
				pbuf[j] = tmp;
				desordre = 1;
			}
		}
	}
}*/

//not used
//quick sort
/*
void triRapid(MOVE *tab, int first, int last)
{
    int pivot, i, j;
    MOVE tmp;

    if(first < last)
    {
        pivot = first;
        i = first;
        j = last;
        while (i < j)
        {
            while(tab[i].evaluation <= tab[pivot].evaluation && i < last)
                i++;
            while(tab[j].evaluation > tab[pivot].evaluation)
                j--;
            if(i < j)
            {
                tmp = tab[j+1];
				tab[j+1] = tab[j];
				tab[j] = tmp;
            }
        }
        tmp = tab[pivot];
        tab[pivot] = tab[j];
        tab[j] = tmp;

        triRapid(tab, first, j - 1);
        triRapid(tab, j + 1, last);
    }
}
*/

//not used yet
/*
MOVE root_search(int ctr, MOVE *pbuf, int depth)
{
	MOVE tmp;
	MOVE m;
	int i,j;
	int score;
	int alpha,beta;
	int best;
	clock_t temp;

	for(j=2;j<=depth;++j)
	{
		alpha = -MATE;
		beta  = MATE;
		best = -1000;

		for(i=0;i<ctr;++i)
		{
			if(pbuf[i].evaluation == -20000)
				continue;

			score = -alpha_beta(-beta, -alpha, j, &m);
			tmp = m;

			undo_move();

			if(score > best)
			{
				tmp = m;
				best = score;
			}

			temp = clock();
            unsigned long millis = (temp -  debut) * 1000 / CLOCKS_PER_SEC;

            printf(" %3d      %5d   %3dcp    %10d        %8d\n",i,millis,score,nodes_alpha,nodes_qs);

            if((score > 90000) || (score < -90000))
                break;

		}
	}

	return tmp;
}*/

//plain alpha beta search
int alpha_beta(int alpha, int beta, int depth, MOVE * pBestMove)
{
    int i,j,k;
    int value;
    int havemove;
    int movecnt;
    int echec = FAUX;
    MOVE moveBuf[100];
    MOVE tmpMove;
    int score;
    int hashf = hashfALPHA;

    //time out
    if(timeout())
        return 0;
        
    //longueur pv
    long_pv[prof] = prof;

    //nulle règle des 50 coups? ?
    if(cinquante == 100)
        return 0;

    //nulle règle des triples répétitions ?
    if(prof && reps())
        return 0;

    //extension d'une profondeur si la couleur en cours est en échec
    echec = king_atacked(side);
    if(echec)
        depth++;

    //on ateint la profondeur en cours , quiescence
    if(depth <= 0)
    {
        value =  quiesce(alpha, beta, &tmpMove);
        return value;
    }
    
    //position dans la table de hashage ?
    if(prof)
    {
        value = probe_hash(depth, alpha, beta);
        if(value != MATE)
            return value;
    }

    //génération des coups pseudo-légaux
    movecnt = gen_coups(side, moveBuf);

    //init du pointeur et flag "au moins un coup jouable"
    havemove = 0;
    pBestMove->type = VIDE;

    //boucle coups normaux
    for (i = 0; i < movecnt; ++i)
    {
        meilleur_coup_suivant(moveBuf, movecnt, i);
		if (do_move(moveBuf[i]))
            continue;

        //compteur de coups légaux
        havemove++;

        //compteurs de positions
        nodes_alpha++;

        value = -alpha_beta(-beta, -alpha, depth - 1, &tmpMove);

        undo_move();

        //time out
        if(timeout())
            return 0;

        if (value > alpha)
        {
            *pBestMove = moveBuf[i];
            if (value >= beta)     //" cutoff "
            {
                save_hash(depth, beta, hashfBETA, pBestMove);
                if(moveBuf[i].type == NORMAL)
                {
                    //update history
                    hh[moveBuf[i].from][moveBuf[i].dest] += (depth * depth);
                    if(hh[moveBuf[i].from][moveBuf[i].dest] >= 20000)
                        hh[moveBuf[i].from][moveBuf[i].dest] /= 2;
                    for(k=0; k<i; ++k)
                    {
                        if(moveBuf[k].type == NORMAL)
                        {
                            hh[moveBuf[k].from][moveBuf[k].dest] -= depth;
                            if(hh[moveBuf[k].from][moveBuf[k].dest] < -20000)
                                hh[moveBuf[k].from][moveBuf[k].dest] /= 2;
                        }
                    }
                }
                return beta;
            }

            hashf = hashfEXACT;
            alpha = value;
            
            //mise a jour pv
            pv[prof][prof] = *pBestMove;
            for (j = prof + 1; j < long_pv[prof + 1]; ++j)
            {
                pv[prof][j] = pv[prof + 1][j];
            }
            long_pv[prof] = long_pv[prof + 1];
        }
    }

    //si aucun coups , situation de MAT ou de PAT
    if (!havemove)
    {
        if(echec)
            return -MATE + prof;
        else
            return 0;
    }

    save_hash(depth, alpha, hashf, pBestMove);

    return alpha;
}

//quiescent search
int quiesce(int alpha, int beta, MOVE * pBestMove)
{
    int i,j;
    int val;
    int movecnt;
    MOVE moveBuf[100],tmpMove;
    int score;
    
    //time out
    if(timeout())
        return 0;
    
    //longueur pv
    long_pv[prof] = prof;

    //evaluation de la position en cours
    score = eval();

    //stand pat ?
    if(score >= beta)
        return beta;

    // delta pruning
	/*const int BIG_DELTA = 975;
	if (score < alpha - BIG_DELTA)
    {
		return alpha;
	}*/

    if(alpha < score)
        alpha = score;

    //generation captures + promos dame
    pBestMove->type = VIDE;
    movecnt = gen_captures(side, moveBuf);
    
    /*if(movecnt > 5)
    {
    	int pause;
    	affiche_echiquier();
    	affiche_liste(movecnt, moveBuf);
    	scanf("%d",&pause);
	}*/
	
    if(!movecnt)
        return alpha;

    for (i = 0; i < movecnt; ++i)
    {
        meilleur_coup_suivant(moveBuf, movecnt, i);
       
        if (do_move(moveBuf[i]))
            continue;

        //compteur de qs positions
        nodes_qs++;

        val = -quiesce(-beta, -alpha, &tmpMove);

        undo_move();

        //time out
        if(timeout())
            return 0;

        if (val > alpha)
        {
            if (val >= beta)
                return beta;
            alpha = val;
            *pBestMove = moveBuf[i];
            //mise a jour pv
            pv[prof][prof] = *pBestMove;
            for (j = prof + 1; j < long_pv[prof + 1]; ++j)
            {
                pv[prof][j] = pv[prof + 1][j];
            }
            long_pv[prof] = long_pv[prof + 1];
        }
    }
    return alpha;
}

//for 3 reps rule
int reps()
{
	int i;
	int r = 0;

	for (i = hdp - cinquante; i < hdp; ++i)
    {
        //printf("hist hash et cur hash  : %" PRIx64 "   %" PRIx64 "\n",hist[i].hash_code,hash_position);
        if (hist[i].hash_code == hash_position)
			++r;
    }
    //printf("%d  \n",r);
	return r;
}

//sorting moves 
//pick the best in the move list
void meilleur_coup_suivant(MOVE *ptable, int nb_coups, int debut)
{
    int ms;
    int mi;
    int i;
    MOVE temp;

    ms = -MATE;
    for(i = debut; i < nb_coups; ++i)
    {
       if(ptable[i].evaluation > ms)
        {
            mi = i;
            ms = ptable[i].evaluation;
        }
    }

    temp = ptable[debut];
    ptable[debut] = ptable[mi];
    ptable[mi] = temp;
}
