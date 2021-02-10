#ifndef ENGINE_H_INCLUDED
#define ENGINE_H_INCLUDED

#include "board.h"


int hh[64][64];			//history table

//used for TT
int positions_testees;
int positions_trouvees;
int positions_sauvees;

//used for display PV
int long_pv[MAXPLY+1];
MOVE pv[MAXPLY+1][MAXPLY+1];

//fonctions
MOVE engine(int d, int output);
//not used yest
/*
int short_search(int a, int b, int depth);
int qs(int a, int b);
void tri(int ctr, MOVE *pbuf);
void triRapid(MOVE *tab, int first, int last);
MOVE root_search(int ctr, MOVE *pbuf, int depth);
*/
int alpha_beta(int alpha, int beta, int depth, MOVE * pBestMove);
int quiesce(int a, int b, MOVE *best);
int reps();
void meilleur_coup_suivant(MOVE *ptable, int nb_coups, int debut);
#endif // ENGINE_H_INCLUDED
