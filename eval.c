#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
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

//FONCTION D'EVALUATION STATIQUE
//materiel + psqt
int eval()
{
	int score = 0;		//final position score
	int p = 0; 			//game phase
	int mat;			//matérial score
	int mg = 0,eg = 0;	//middle game and endgame scores
	int w_pst_mg = 0,w_pst_eg = 0;	//psqt scores for white
	int b_pst_mg = 0,b_pst_eg = 0;	//psqt scores for black
	int w_mob = 0;		//mobility score for white
	int b_mob = 0;		//mobility score for black
	int w_passers = 0;	//passed pawns score for white
	int b_passers = 0;	//passed pawns score for black
	
	//phase
	p = phase();
	
	//material score
	mat = eval_dif_materiel();
	mg += mat;
	eg += mat;

	//psqt score
	eval_psqt_w(bb, &w_pst_mg, &w_pst_eg);
	eval_psqt_b(bb, &b_pst_mg, &b_pst_eg);
	mg += (w_pst_mg - b_pst_mg);
	eg += (w_pst_eg - b_pst_eg); 

	//mobility (empty squares no defended by enemies pawns)
	//mobility *3 for knight   mobility *2 for bishops
	//mobility *1 for rooks	  mobility *0.5 for queens
	w_mob = eval_w_mob(bb);
	b_mob = eval_b_mob(bb);
	mg += (w_mob - b_mob);
	eg += (w_mob - b_mob);
	
	//passed pawns
	w_passers = pion_passe_blanc();
	b_passers = pion_passe_noir();
	
	mg += (20 * (w_passers - b_passers));
	eg += (40 * (w_passers - b_passers));
	
	//tappered eval score
	score = ((mg * (256 - coef_phase[p])) + (eg * coef_phase[p])) / 256;

	//display scores
	if(e_valide)
	{
		printf("phase : %d  et coef %d\n",p,coef_phase[p]);
		printf("material : %5d %5d\n",eval_materiel_blanc(),eval_materiel_noir());
		printf("psqt     : %5d %5d   %5d %5d\n",w_pst_mg,w_pst_eg,b_pst_mg,b_pst_eg);
		printf("mobility : %5d %5d \n",w_mob,b_mob);
		printf("passers  : %5d %5d \n",(20 * (w_passers - b_passers)),(40 * (w_passers - b_passers)));
		printf("mg et eg : %5d %5d\n",mg,eg);
		printf("score    : %5d\n",score);
	}

	//return score
	if(side == BLANC)
		return score;
	return -score;
}

//EVAL PSQT
void eval_psqt_w(U64 *ptab, int *smg, int *seg)
{
	U64 x = 0x0;
	int sq;
	int score = 0;

	//pions
	x = ptab[WP];
	if(e_valide)
		printf("pions blancs : \n");
	while(x)
	{
		sq = LSB(x);
		*smg += psqt_mg[WP][sq];
		*seg += psqt_eg[WP][sq];
		if(e_valide)
			printf("    %s (%2d)  psqt mg et eg : %2d  %2d \n",coord_une_case(sq),sq,psqt_mg[WP][sq],psqt_eg[WP][sq]);
		x &= (x-1);
	}
	//cavaliers
	x = ptab[WN];
	if(e_valide)
		printf("cavaliers blancs : \n");
	while(x)
	{
		sq = LSB(x);
		*smg += psqt_mg[WN][sq];
		*seg += psqt_eg[WN][sq];
		if(e_valide)
			printf("    %s (%2d)  psqt mg et eg : %2d  %2d \n",coord_une_case(sq),sq,psqt_mg[WN][sq],psqt_eg[WN][sq]);
		x &= (x-1);
	}
	//fous
	x = ptab[WB];
	if(e_valide)
		printf("fous blancs : \n");
	while(x)
	{
		sq = LSB(x);
		*smg += psqt_mg[WB][sq];
		*seg += psqt_eg[WB][sq];
		if(e_valide)
			printf("    %s (%2d)  psqt mg et eg : %2d  %2d \n",coord_une_case(sq),sq,psqt_mg[WB][sq],psqt_eg[WB][sq]);
		x &= (x-1);
	}
	//tours
	x = ptab[WR];
	if(e_valide)
		printf("tours blanches : \n");
	while(x)
	{
		sq = LSB(x);
		*smg += psqt_mg[WR][sq];
		*seg += psqt_eg[WR][sq];
		if(e_valide)
			printf("    %s (%2d)  psqt mg et eg : %2d  %2d \n",coord_une_case(sq),sq,psqt_mg[WR][sq],psqt_eg[WR][sq]);
		x &= (x-1);
	}
	//dames
	x = ptab[WQ];
	if(e_valide)
		printf("dames blanches : \n");
	while(x)
	{
		sq = LSB(x);
		*smg += psqt_mg[WQ][sq];
		*seg += psqt_eg[WQ][sq];
		if(e_valide)
			printf("    %s (%2d)  psqt mg et eg : %2d  %2d \n",coord_une_case(sq),sq,psqt_mg[WQ][sq],psqt_eg[WQ][sq]);
		x &= (x-1);
	}
	//roi
	if(e_valide)
		printf("roi blanc : \n");
	sq = LSB(ptab[WK]);
	*smg += psqt_mg[WK][sq];
	*seg += psqt_eg[WK][sq];
	if(e_valide)
		printf("    %s (%2d)  psqt mg et eg : %2d  %2d \n",coord_une_case(sq),sq,psqt_mg[WK][sq],psqt_eg[WK][sq]);
}

void eval_psqt_b(U64 *ptab, int *smg, int *seg)
{
	U64 x = 0x0;
	int sq;
	int score = 0;

	//pions
	x = ptab[BP];
	if(e_valide)
		printf("pions noirs : \n");
	while(x)
	{
		sq = LSB(x);
		*smg += psqt_mg[BP][sq];
		*seg += psqt_eg[BP][sq];
		if(e_valide)
			printf("    %s (%2d)  psqt mg et eg : %2d  %2d \n",coord_une_case(sq),sq,psqt_mg[BP][sq],psqt_eg[BP][sq]);
		x &= (x-1);
	}
	//cavaliers
	if(e_valide)
		printf("cavaliers noirs : \n");
	x = ptab[BN];
	while(x)
	{
		sq = LSB(x);
		*smg += psqt_mg[BN][sq];
		*seg += psqt_eg[BN][sq];
		if(e_valide)
			printf("    %s (%2d)  psqt mg et eg : %2d  %2d \n",coord_une_case(sq),sq,psqt_mg[BN][sq],psqt_eg[BN][sq]);
		x &= (x-1);
	}
	//fous
	x = ptab[BF];
	if(e_valide)
		printf("fous noirs : \n");
	while(x)
	{
		sq = LSB(x);
		*smg += psqt_mg[BF][sq];
		*seg += psqt_eg[BF][sq];
		if(e_valide)
			printf("    %s (%2d)  psqt mg et eg : %2d  %2d \n",coord_une_case(sq),sq,psqt_mg[BF][sq],psqt_eg[BF][sq]);
		x &= (x-1);
	}
	//tours
	x = ptab[BR];
	if(e_valide)
		printf("tours noires : \n");
	while(x)
	{
		sq = LSB(x);
		*smg += psqt_mg[BR][sq];
		*seg += psqt_eg[BR][sq];
		if(e_valide)
			printf("    %s (%2d)  psqt mg et eg : %2d  %2d \n",coord_une_case(sq),sq,psqt_mg[BR][sq],psqt_eg[BR][sq]);
		x &= (x-1);
	}
	//dames
	if(e_valide)
		printf("dames noires : \n");
	x = ptab[BQ];
	while(x)
	{
		sq = LSB(x);
		*smg += psqt_mg[BQ][sq];
		*seg += psqt_eg[BQ][sq];
		if(e_valide)
			printf("    %s (%2d)  psqt mg et eg : %2d  %2d \n",coord_une_case(sq),sq,psqt_mg[BQ][sq],psqt_eg[BQ][sq]);
		x &= (x-1);
	}
	//roi
	if(e_valide)
		printf("roi noir : \n");
	sq = LSB(ptab[BK]);
	*smg += psqt_mg[BK][sq];
	*seg += psqt_eg[BK][sq];
	if(e_valide)
		printf("    %s (%2d)  psqt mg et eg : %2d  %2d \n",coord_une_case(sq),sq,psqt_mg[BK][sq],psqt_eg[BK][sq]);
}

//EVAL MOBILITY
int eval_w_mob(U64 *ptab)
{
	U64 bp_atk = 0x0;	//attaques pions noirs
	U64 x = 0x0;
	int score = 0;
	int ns = 0,bs = 0,rs = 0,qs = 0;
	
	bp_atk = get_atk_bp(ptab);
	
	//cavalier---------------------------------------------
	x = get_atk_n(WN, ptab);
	x &= ptab[NOP];
	//	restriction attaques pions noirs
	x &= (~bp_atk);
	ns = ((POP(x)) * 3);
	if(e_valide)
	{
		printf("--cavaliers--\n");
		printf("score : %d\n",ns);
	}
	score += ns;
	//fou---------------------------------------------------
	x = get_atk_b(WB, ptab);
	x &= ptab[NOP];
	//	restriction attaques pions noirs
	x &= (~bp_atk);
	bs = ((POP(x)) * 2);
	if(e_valide)
	{
		printf("--fous--\n");
		printf("score : %d\n",bs);
	}
	score += bs;
	//tour--------------------------------------------------
	x = get_atk_r(WR, ptab);
	x &= ptab[NOP];
	//	restriction attaques pions noirs
	x &= (~bp_atk);
	rs = (POP(x));
	if(e_valide)
	{
		printf("--tours--\n");
		printf("score : %d\n",rs);
	}
	score += rs;
	//DAME--------------------------------------------------
	x = get_atk_q(WQ, ptab);
	x &= ptab[NOP];
	//	restriction attaques pions noirs
	x &= (~bp_atk);
	qs = ((POP(x)) / 2);
	if(e_valide)
	{
		printf("--dames--\n");
		printf("score : %d\n",qs);
	}
	score += qs;
	
	return score;
}

int eval_b_mob(U64 *ptab)
{
	U64 wp_atk = 0x0;	//attaques pions blancs
	U64 x = 0x0;
	int score = 0;
	int ns = 0,bs = 0,rs = 0,qs = 0;

	wp_atk = get_atk_wp(ptab);

	//cavalier-------------------------------------------------
	x = get_atk_n(BN, ptab);
	x &= ptab[NOP];
	//	restriction attaques pions noirs
	x &= (~wp_atk);
	ns = ((POP(x)) * 3);
	if(e_valide)
	{
		printf("--cavaliers--\n");
		printf("score : %d\n",ns);
	}
	score += ns;
	//fou-----------------------------------------------------
	x = get_atk_b(BF, ptab);
	x &= ptab[NOP];
	//	restriction attaques pions noirs
	x &= (~wp_atk);
	bs = ((POP(x)) * 2);
	if(e_valide)
	{
		printf("--fous--\n");
		printf("score : %d\n",bs);
	}
	score += bs;
	//tour---------------------------------------------------
	x = get_atk_r(BR, ptab);
	x &= ptab[NOP];
	//	restriction attaques pions noirs
	x &= (~wp_atk);
	rs = (POP(x));
	if(e_valide)
	{
		printf("--tours--\n");
		printf("score : %d\n",rs);
	}
	score += rs;
	//DAME---------------------------------------------------
	x = get_atk_q(BQ, ptab);
	x &= ptab[NOP];
	//	restriction attaques pions noirs
	x &= (~wp_atk);
	qs = ((POP(x)) / 2);
	if(e_valide)
	{
		printf("--dames--\n");
		printf("score : %d\n",qs);
	}
	score += qs;
	return score;
}
//EVAL PASSERS
int pion_passe_blanc()
{
	U64 mask_bp = 0x0;
	U64 x;
	U64 w_passers = 0x0;
	int sq_passer;
	int nb_passers;
	int bp;

	x = bb[BP];

	while(x)
	{
		bp = LSB(x);
		mask_bp |= mask_front_bp[bp];
		x &= (x -1);
		//printf("bp = %d   mask front bp = %" PRIx64 "\n",bp,mask_front_bp[bp]);
	}

	w_passers = (bb[WP] & (~mask_bp));
	//printf("w passer  = %" PRIx64 "\n",w_passers);
	nb_passers = POP(w_passers);

	/*while(w_passers)
	{
		sq_passer = LSB(w_passers);
		//printf("pion passe blanc : %d \n",sq_passer);
		w_passers &= (w_passers - 1);
	}*/

	/*printf("bb.bp     = %" PRIx64 "\n",bb.bp);
	printf("bb.wp     = %" PRIx64 "\n",bb.wp);
	printf("mask.bp   = %" PRIx64 "\n",mask_bp);
	printf("~mask     = %" PRIx64 "\n",(~mask_bp));
	printf("bp        = %d\n",bp);
	printf("sq passer = %d\n",sq_passer);
	printf("sq passer = %d\n",nb_passers);*/

	return(nb_passers);  //ou wpassers on verra ....
}

int pion_passe_noir()
{
	U64 mask_wp = 0x0;
	U64 x;
	U64 b_passers = 0x0;
	int sq_passer;
	int nb_passers;
	int wp;

	x = bb[WP];

	while(x)
	{
		wp = LSB(x);
		mask_wp |= mask_front_wp[wp];
		x &= (x -1);
	}

	b_passers = (bb[BP] & (~mask_wp));
	nb_passers = POP(b_passers);

	/*while(b_passers)
	{
		sq_passer = LSB(b_passers);
		printf("pion passe blanc : %d \n",sq_passer);
		b_passers &= (b_passers - 1);
	}*/

	return(nb_passers);  //ou wpassers on verra ....
}

//EVAL MATERIAL
int eval_dif_materiel()   //retourne la diférence de matériel "in white POV"
{
	return(eval_materiel_blanc() - eval_materiel_noir());
}

int eval_materiel_blanc()
{
	int score = 0;

	score = ((POP(bb[WP]) * VAL_PAWN)+(POP(bb[WN]) * VAL_KNIGHT)+(POP(bb[WB]) * VAL_BISHOP)+
			(POP(bb[WR]) * VAL_ROOK)+(POP(bb[WQ]) * VAL_QUEEN));
	//printf("w_mat : %5d",score);
	return score;
}

int eval_materiel_noir()
{
	int score = 0;

	score = ((POP(bb[BP]) * VAL_PAWN)+(POP(bb[BN]) * VAL_KNIGHT)+(POP(bb[BF]) * VAL_BISHOP)+
			(POP(bb[BR]) * VAL_ROOK)+(POP(bb[BQ]) * VAL_QUEEN));
	//printf("       b_mat : %5d  ",score);
	return score;
}

//DRAW RECOGNIZER
void draw_recognizer()
{
	//pour un draw recognizer rapide , on compte le materiel
	//hors roi qui doit pas ecceder la valeur d'UNE piece mineure DANS LES DEUX CAMPS
	//avec aucun pions
	if((POP(bb[WP] | bb[BP]) == 0) && (eval_materiel_blanc() <= 10300) && (eval_materiel_noir() <= 10300))
		printf("nulle materiel insufisant\n");
}

//TO DO :
int KBB_K_recognizer()
{
	if(POP(bb[WP] | bb[BP]) == 0)   //aucuns pions ?
	{

	}
	return 0;
}

int KBN_K_recognizer()
{
	if(POP(bb[WP] | bb[BP]) == 0)   //aucuns pions ?
	{

	}
	return 0;
}

//calcul phase
int phase()
{
	int p = 0;
	
	p += (4 * POP(bb[WQ] | bb[BQ]));
	p += (2 * POP(bb[WR] | bb[BR]));
	p += (POP(bb[WN] | bb[BN] | bb[WB] | bb[BF]));
	//printf("%d  %d  %d\n",(4 * POP(bb[WQ] | bb[BQ])),(2 * POP(bb[WR] | bb[BR])),(POP(bb[WN] | bb[BN] | bb[WB] | bb[BF])));
	p = (24 - p);
	//printf("p = %d\n",p);
	return p;
}


