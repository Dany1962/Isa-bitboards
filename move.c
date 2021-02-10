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

//play a move 
int do_move(MOVE m)
{
	int in_check = FAUX;
	int xside = OPP(side);
	int from = m.from;
	int to = m.dest;
	int p_from = m.piece_from;
	int p_to = m.piece_dest;
	int type = m.type;
	int pce;
	int x;

	//sauvegardes
	hist[hdp].m = m;
	hist[hdp].cap = p_to;
    hist[hdp].castle = castle;
    hist[hdp].ep = ep;
    hist[hdp].cinquante = cinquante;
    hist[hdp].hash_code = hash_position;

    //vider la case départ
    clear_sq(side, from, p_from);

    switch(type)
    {
    	case NONE 		: 	break;
		case NORMAL 	: 	fill_sq(side, to, p_from);
							break;
		case CAPTURE 	: 	clear_sq(xside, to, p_to);
							fill_sq(side, to, p_from);
							break;
		case EN_PASSANT : 	if(side == BLANC)
							{
								clear_sq(xside, to-8, BP);
								fill_sq(side, to, WP);
							}
							else
							{
								clear_sq(xside, to+8, WP);
								fill_sq(side, to, BP);
							}
							break;
		case PETIT_ROQUE:   if(side == BLANC)
							{
								fill_sq(side, G1, WK);
								clear_sq(side, H1, WR);
								fill_sq(side, F1, WR);
							}
							else
							{
								fill_sq(side, G8, BK);
								clear_sq(side, H8, BR);
								fill_sq(side, F8, BR);
							}
							break;
		case GRAND_ROQUE: 	//affiche_petit_echiquier();
                            if(side == BLANC)
							{
								fill_sq(side, C1, WK);
								clear_sq(side, A1, WR);
								fill_sq(side, D1, WR);
							}
							else
							{
								fill_sq(side, C8, BK);
								clear_sq(side, A8, BR);
								fill_sq(side, D8, BR);
							}
							//affiche_petit_echiquier();
							//scanf("%d",&x);
							break;
		case Q_PROMO	: 	pce = ((side == BLANC) ? WQ : BQ);
							if(p_to)
								clear_sq(xside, to, p_to);
							fill_sq(side, to, pce);
							break;
		case R_PROMO	: 	pce = ((side == BLANC) ? WR : BR);
							if(p_to)
								clear_sq(xside, to, p_to);
							fill_sq(side, to, pce);
							break;
		case B_PROMO	: 	pce = ((side == BLANC) ? WB : BF);
							if(p_to)
								clear_sq(xside, to, p_to);
							fill_sq(side, to, pce);
							break;
		case N_PROMO	: 	pce = ((side == BLANC) ? WN : BN);
							if(p_to)
								clear_sq(xside, to, p_to);
							fill_sq(side, to, pce);
							break;
		default			: break;
	}

	//update profondeur et nbre de coups
	prof++;
	hdp++;

	//mise a jour de la case de prise en passant
	ep = m.ep_flag;
	if(ep != -1)
        hash_position ^= val_en_passant[ep];

	//mise a jour de cinquante (pour la règle des 50 coups
    //sans coups de pions ou prise de piece
    if(echiquier[to] == WP || echiquier[to] == BP ||
	type == CAPTURE || type == EN_PASSANT)
		cinquante = 0;
	else
		++cinquante;

	//mise a jour pour droits aux roques
    castle &= castle_mask[from] & castle_mask[to];
    hash_position ^= droit_aux_roques[castle];

    if(type == PETIT_ROQUE || type == GRAND_ROQUE)
    {
        side = (BLANC + NOIR) - side;
        return FAUX;
    }

	in_check = king_atacked(side);
	side = (BLANC + NOIR) - side;
	hash_position ^= val_couleur[side];

	if(in_check)
    {
        undo_move();
        return VRAI;
    }

	return FAUX;
}

//undo a move
void undo_move()
{
	hash_position ^= val_couleur[side];
	side = (BLANC + NOIR) - side;
	int xside = OPP(side);
	int pce;
	int x;

	int from,to,p_from,p_to,type;

	hdp--;
	prof--;

	from   = hist[hdp].m.from;
    to     = hist[hdp].m.dest;
    p_from = hist[hdp].m.piece_from;
    p_to   = hist[hdp].m.piece_dest;
    type   = hist[hdp].m.type;


	switch(type)
    {
    	case NONE 		: 	break;
		case NORMAL 	: 	clear_sq(side, to, p_from);
							fill_sq(side, from, p_from);
							break;
		case CAPTURE 	: 	clear_sq(side, to, p_from);
							fill_sq(xside, to, p_to);
							fill_sq(side, from, p_from);
							break;
		case EN_PASSANT : 	if(side == BLANC)
							{
								clear_sq(side, to, WP);
								fill_sq(xside, to-8, BP);
								fill_sq(side, from, WP);
							}
							else
							{
								clear_sq(side, to, BP);
								fill_sq(xside, to+8, WP);
								fill_sq(side, from, BP);
							}
							break;
		case PETIT_ROQUE: 	if(side == BLANC)
							{
								clear_sq(side, G1, WK);
								fill_sq(side, E1, WK);
								clear_sq(side, F1, WR);
								fill_sq(side, H1, WR);
							}
							else
							{
								clear_sq(side, G8, BK);
								fill_sq(side, E8, BK);
								clear_sq(side, F8, BR);
								fill_sq(side, H8, BR);
							}
							break;
		case GRAND_ROQUE: 	//affiche_petit_echiquier();
                            if(side == BLANC)
							{
								clear_sq(side, C1, WK);
								fill_sq(side, E1, WK);
								clear_sq(side, D1, WR);
								fill_sq(side, A1, WR);
							}
							else
							{
								clear_sq(side, C8, BK);
								fill_sq(side, E8, BK);
								clear_sq(side, D8, BR);
								fill_sq(side, A8, BR);
							}
							//affiche_petit_echiquier();
							//scanf("%d",&x);
							break;
		case Q_PROMO	: 	pce = ((side == BLANC) ? WQ : BQ);
							clear_sq(side, to, pce);
							if(p_to)
								fill_sq(xside, to, p_to);
							fill_sq(side, from, p_from);
							break;
		case R_PROMO	: 	pce = ((side == BLANC) ? WR : BR);
							clear_sq(side, to, pce);
							if(p_to)
								fill_sq(xside, to, p_to);
							fill_sq(side, from, p_from);break;
		case B_PROMO	: 	pce = ((side == BLANC) ? WB : BF);
							clear_sq(side, to, pce);
							if(p_to)
								fill_sq(xside, to, p_to);
							fill_sq(side, from, p_from);break;
		case N_PROMO	: 	pce = ((side == BLANC) ? WN : BN);
							clear_sq(side, to, pce);
							if(p_to)
								fill_sq(xside, to, p_to);
							fill_sq(side, from, p_from);break;
		default			: break;
	}

	//BACKUPS
	hash_position ^= droit_aux_roques[castle];
    castle = hist[hdp].castle;
    if(ep != -1)
        hash_position ^= val_en_passant[ep];
    ep = hist[hdp].ep;
    cinquante = hist[hdp].cinquante;
}

//fill a square 'sq' whith a piece 'p' of color 'c'
void fill_sq(int c, int sq, int p)
{
	//update hash code
	hash_position ^= hash_table[p][sq];

	//update position
	echiquier[sq] = p;

	//bitboardS
	set_bit(bb, sq, ALP);
	clear_bit(bb, sq, NOP);
	if(c == BLANC)
	{
		set_bit(bb, sq, WPC);
		set_bit(bb, sq, p);
	}
	else
	{
		set_bit(bb, sq, BPC);
		set_bit(bb, sq, p);
	}
}

//remove a piece 'p' of color 'c' from square 'sq'
void clear_sq(int c, int sq, int p)
{
	//update hash code
	hash_position ^= hash_table[p][sq];

	//update position
	echiquier[sq] = VIDE;

	//bitboards
	clear_bit(bb, sq, ALP);
	set_bit(bb, sq, NOP);

	if(c == BLANC)
	{
		clear_bit(bb, sq, WPC);
		clear_bit(bb, sq, p);
	}
	else
	{
		clear_bit(bb, sq, BPC);
		clear_bit(bb, sq, p);
	}
}

//check if a king is in check , to remove a move from the move list
int king_atacked(int c)
{
	U64 b_atk = 0x0;
	U64 w_atk = 0x0;

	if(side == BLANC)
	{
		if(show_move)
        {
            printf("atk  : %" PRIx64 "\n",b_atk);
        }
		b_atk = get_b_atk();
		if(show_move)
        {
            printf("atk  : %" PRIx64 "   bb[WK] : %" PRIx64 " \n",b_atk,bb[WK]);
        }
		if(b_atk & bb[WK])
			return VRAI;
		return FAUX;
	}
	else
    {
        w_atk = get_w_atk();
        if(w_atk & bb[BK])
            return VRAI;
    }

	return FAUX;
}



