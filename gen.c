#include <stdio.h>
#include <stdlib.h>
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

//MOVE ORDERING
int order_moves(int from, int to, int type, int *ptab)
{
    int score = 0;
    int index_piece[13] = {0, 1, 2, 3, 4, 5, 6, 1, 2, 3, 4, 5, 6};
    int pfrom = ptab[from];
    int pto   = ptab[to];

    switch(type)
    {
        case NONE 		: 	break;
		case NORMAL 	: 	score = hh[from][to];
							break;
		case CAPTURE 	: 	score = (BONUS_CAPT + (64 * index_piece[pto] - index_piece[pfrom]));
							break;
		case EN_PASSANT : 	score = (BONUS_CAPT + (64 * index_piece[pto] - index_piece[pfrom]));
							break;
		case PETIT_ROQUE:   score = BONUS_ROQUE;
							break;
		case GRAND_ROQUE:   score = BONUS_ROQUE;
							break;
		case Q_PROMO	: 	score = (BONUS_PROMO + VAL_QUEEN + (64 * index_piece[pto] - index_piece[pfrom]));
							break;
		case R_PROMO	: 	score = (BONUS_PROMO + VAL_ROOK + (64 * index_piece[pto] - index_piece[pfrom]));
							break;
		case B_PROMO	: 	score = (BONUS_PROMO + VAL_BISHOP + (64 * index_piece[pto] - index_piece[pfrom]));
							break;
		case N_PROMO	: 	score = (BONUS_PROMO + VAL_KNIGHT + (64 * index_piece[pto] - index_piece[pfrom]));
							break;
		default			: break;
	}

	return score;
}

//COPY MOVES
void copy_normal(int f, int t, int type, int epf, MOVE * pBuf, int *pMCount)
{
	MOVE move;

	move.from = f;
    move.dest = t;
    move.piece_from = echiquier[f];
    move.piece_dest = echiquier[t];
    if(echiquier[t] != VIDE)
    {
        move.type = CAPTURE;
        move.evaluation = order_moves(f, t, CAPTURE, echiquier);
    }
    else
    {
        move.type = NORMAL;
        move.evaluation = order_moves(f, t, NORMAL, echiquier);
    }
    move.ep_flag = epf;
    pBuf[*pMCount] = move;
    *pMCount = *pMCount + 1;
}

void copy_ep(int f, int t, int type, int epf, MOVE * pBuf, int *pMCount)
{
	MOVE move;

	move.from = f;
    move.dest = t;
    move.piece_from = echiquier[f];
    move.piece_dest = echiquier[t];
    move.type = type;
    move.evaluation = order_moves(f, t, type, echiquier);
    move.ep_flag = epf;
    pBuf[*pMCount] = move;
    *pMCount = *pMCount + 1;
}

void copy_castle(int f, int t, int type, int epf, MOVE * pBuf, int *pMCount)
{
	MOVE move;

	move.from = f;
    move.dest = t;
    move.piece_from = echiquier[f];
    move.piece_dest = echiquier[t];
    move.type = type;
    move.evaluation = order_moves(f, t, type, echiquier);
    move.ep_flag = epf;
    pBuf[*pMCount] = move;
    *pMCount = *pMCount + 1;
}

void copy_promo(int f, int t, int type, int epf, MOVE * pBuf, int *pMCount)
{
	MOVE move;

	move.from = f;
    move.dest = t;
    move.piece_from = echiquier[f];
    move.piece_dest = echiquier[t];
    move.type = Q_PROMO;
    move.ep_flag = epf;
    move.evaluation = order_moves(f, t, type, echiquier);
    pBuf[*pMCount] = move;
    *pMCount = *pMCount + 1;

	move.from = f;
    move.dest = t;
    move.piece_from = echiquier[f];
    move.piece_dest = echiquier[t];
    move.type = R_PROMO;
    move.ep_flag = epf;
    move.evaluation = order_moves(f, t, type, echiquier);
    pBuf[*pMCount] = move;
    *pMCount = *pMCount + 1;

    move.from = f;
    move.dest = t;
    move.piece_from = echiquier[f];
    move.piece_dest = echiquier[t];
    move.type = B_PROMO;
    move.ep_flag = epf;
    move.evaluation = order_moves(f, t, type, echiquier);
    pBuf[*pMCount] = move;
    *pMCount = *pMCount + 1;

    move.from = f;
    move.dest = t;
    move.piece_from = echiquier[f];
    move.piece_dest = echiquier[t];
    move.type = N_PROMO;
    move.ep_flag = epf;
    move.evaluation = order_moves(f, t, type, echiquier);
    pBuf[*pMCount] = move;
    *pMCount = *pMCount + 1;
}

//PSEUDO LEGAL MOVES GENERATOR---------------------------------------------------------------------------
int gen_coups(int current_side, MOVE * pBuf)
{
	int movecount = 0;

	if(side == BLANC)
	{
		U64 permit = (bb[BPC] | bb[NOP]);					//moves on squares alowed --> black pieces | no pieces (empty squares)
		gen_roques_blancs(&movecount, pBuf);				//castle moves
		gen_coups_pions_blancs(&movecount, pBuf, BIT);		//Wpawns moves
		gen_coups_cavaliers(&movecount, pBuf, permit, WN);	//Wknights moves	
		gen_coups_fous(&movecount, pBuf, permit, WB);		//Wbishops moves
		gen_coups_tours(&movecount, pBuf, permit,WR);		//Wrooks moves
		gen_coups_dames(&movecount, pBuf, permit, WQ);		//Wqueens moves
		gen_coups_rois(&movecount, pBuf, permit, WK);		//Wking moves
	}
	else
	{
		U64 permit = (bb[WPC] | bb[NOP]);					//moves on squares alowed --> white pieces | no pieces (empty squares)
		gen_roques_noirs(&movecount, pBuf);					//castle moves
		gen_coups_pions_noirs(&movecount, pBuf);			//Bpawns moves
		gen_coups_cavaliers(&movecount, pBuf, permit, BN);	//Bknights moves
		gen_coups_fous(&movecount, pBuf, permit, BF);		//Bbishops moves
		gen_coups_tours(&movecount, pBuf, permit, BR);		//Brooks moves
		gen_coups_dames(&movecount, pBuf, permit, BQ);		//Bqueens moves
		gen_coups_rois(&movecount, pBuf, permit, BK);		//Bking moves
	}

	return movecount;
}

//GEN CASTLE
void gen_roques_blancs(int *pmovecount, MOVE * pBuf)
{
	U64 atk,valid,valid2,valid3;
	atk    = get_b_atk();
	valid  = (atk & bb[WK]);
	valid2 = (atk & WK_CASTLE);
	valid3 = (WK_CASTLE & bb[ALP]);

	//printf("atk : %" PRIx64 "\n",atk);

	if(castle & 1)	//o-o valide ?
	{
		if(!valid && !valid2 && !valid3)
			copy_castle(E1, G1, PETIT_ROQUE, -1, pBuf, pmovecount);

	}
	//printf("valid   :  %" PRId64 "  valid2 :  %" PRId64 "  valid3 : %" PRId64 " \n",valid,valid2,valid3);
	valid2 = (atk & WQ_CASTLE);
	valid3 = (0x000000000000000E & bb[ALP]);
	if(castle & 2)	//o-o-o
	{
		if(!valid && !valid2 && !valid3)
			copy_castle(E1, C1, GRAND_ROQUE, -1, pBuf, pmovecount);
	}
	//printf("valid   :  %" PRId64 "  valid2 :  %" PRId64 "  valid3 : %" PRId64 " \n",valid,valid2,valid3);
}

void gen_roques_noirs(int *pmovecount, MOVE * pBuf)
{
	U64 atk,valid,valid2,valid3;
	atk    = get_w_atk();
	valid  = (atk & bb[BK]);        //roi en échec ou non
	valid2 = (atk & BK_CASTLE);     //cases non attaquées par l'adversaire
	valid3 = (BK_CASTLE & bb[ALP]); //cases vides

	//printf("atk : %" PRIx64 "\n",atk);

	if(castle & 4)	//o-o valide ?
	{
		if(!valid && !valid2 && !valid3)
			copy_castle(E8, G8, PETIT_ROQUE, -1, pBuf, pmovecount);
	}
	//printf("valid   :  %" PRId64 "  valid2 :  %" PRId64 "  valid3 : %" PRId64 " \n",valid,valid2,valid3);
	valid2 = (atk & BQ_CASTLE);
	valid3 = (0x0E00000000000000 & bb[ALP]);
	if(castle & 8)	//o-o-o
	{
		if(!valid && !valid2 && !valid3)
			copy_castle(E8, C8, GRAND_ROQUE, -1, pBuf, pmovecount);
	}
	//printf("valid   :  %" PRId64 "  valid2 :  %" PRId64 "  valid3 : %" PRId64 " \n",valid,valid2,valid3);
}

//CHECK IF THE KING IS IN CHECK
int king_in_check(int c)
{
	U64 atk;

	if(c == BLANC)		//white king ?
	{
		atk |= get_atk_bp(bb);		//get Bpawns atacks
		atk |= get_atk_n(BN, bb);	//get Bknights atacks
		atk |= get_atk_b(BF, bb);	//get Bbishops atacks
		atk |= get_atk_r(BR, bb);	//get Brooks atacks
		atk |= get_atk_q(BQ, bb);	//get Bqueens atacks
		atk &= bb[WK];
		if(atk)
			return VRAI;	//TRUE : king in check
		return FAUX;
	}
	else			//black king ?
	{
		atk |= get_atk_bp(bb);		//get Wpawns atacks
		atk |= get_atk_n(WN, bb);	//get Wknights atacks
		atk |= get_atk_b(WB, bb);	//get Wbishops atacks
		atk |= get_atk_r(WR, bb);	//get Wrooks atacks
		atk |= get_atk_q(WQ, bb);	//get Wqueens atacks
		atk &= bb[BK];
		if(atk)
			return VRAI;
	}

	return FAUX;
}

//GEN WHITE PAWNS-----------------------------------------------------------------------------------------------------
void gen_coups_pions_blancs(int *pmovecount, MOVE * pBuf, U64 *pbit)
{
	U64 dest_wp;
	U64 tpa;      //mean "two pushes allowed " pawns
	U64 tpb;
	U64 pea;	//mean pawn east atack
	U64 poa; 	//mean pawn west atack
	int from,to;
	U64 allowed = bb[BPC];
	U64 x = bb[WP];
	U64 ep_valid = 0x0;

	//captures
   	pea = ((x << 9) & (~f_a));
	poa = ((x << 7) & (~f_h));
	//en_passant
	if(ep != -1)
	{
		ep_valid = (pea & bb[NOP]);
		ep_valid &= BIT[ep];
		if(ep_valid)
			copy_ep(ep-9, ep, EN_PASSANT, -1, pBuf, pmovecount);
		ep_valid = (poa & bb[NOP]);
		ep_valid &= BIT[ep];
		if(ep_valid)
			copy_ep(ep-7, ep, EN_PASSANT, -1, pBuf, pmovecount);
	}
	
	//captures
	pea &= allowed;
	while (pea)
   	{
     	to = LSB(pea);
     	from = to - 9;
     	if(BIT[to] & w_promo)
			copy_promo(from, to, 0, -1, pBuf, pmovecount);
		else
			copy_normal(from, to, CAPTURE, -1, pBuf, pmovecount);
     	pea &= (pea - 1); // reset least significant bit
   	}
   	poa &= allowed;
   	while (poa)
   	{
     	to = LSB(poa);
     	from = to - 7;
     	if(BIT[to] & w_promo)
			copy_promo(from, to, 0, -1, pBuf, pmovecount);
		else
			copy_normal(from, to, CAPTURE, -1, pBuf, pmovecount);
   		poa &= (poa - 1); // reset least significant bit
   	}

	//moving two squares
 	tpb = (x << 8) & (~bb[ALP]);
   	tpa = ((tpb & r_3) << 8) & (~bb[ALP]);
   	while (tpa)
   	{
     	to = LSB(tpa);
     	from = to - 16;
     	copy_normal(from, to, NORMAL, to-8, pBuf, pmovecount);
     	tpa &= (tpa - 1);
   	}

	//moving one square
	dest_wp = (x << 8) & bb[NOP];
   	while (dest_wp)
   	{
		to = LSB(dest_wp);
   		from = to - 8;
   		if(BIT[to] & w_promo)
   			copy_promo(from, to, 0, -1, pBuf, pmovecount);
   		else
   			copy_normal(from, to, NORMAL, -1, pBuf, pmovecount);
   		dest_wp &= dest_wp - 1;
   	}
}

//GEN BLACKS PAWNS
void gen_coups_pions_noirs(int *pmovecount, MOVE * pBuf)
{
	U64 dest_bp;
	U64 tpa;      //mean "two pushes allowed " pawns
	U64 tpb;
	U64 pea;	//mean pawn east atack
	U64 poa; 	//mean pawn west atack
	int from,to;
	U64 allowed = bb[WPC];
	U64 x = bb[BP];
	U64 ep_valid = 0x0;

   	pea = ((x >> 7) & (~f_a));
	poa = ((x >> 9) & (~f_h));
	//en_passant
	if(ep != -1)
	{
		ep_valid = (pea & bb[NOP]);
		ep_valid &= BIT[ep];
		if(ep_valid)
			copy_ep(ep+7, ep, EN_PASSANT, -1, pBuf, pmovecount);
		ep_valid = (poa & bb[NOP]);
		ep_valid &= BIT[ep];
		if(ep_valid)
			copy_ep(ep+9, ep, EN_PASSANT, -1, pBuf, pmovecount);
	}
	//captures
	pea &= allowed;
	poa &= allowed;
	while (pea)
   	{
     	to = LSB(pea);
     	from = to + 7;
   		if(BIT[to] & b_promo)
			copy_promo(from, to, 0, -1, pBuf, pmovecount);
		else
			copy_normal(from, to, CAPTURE, -1, pBuf, pmovecount);
   		//printf("capture se : %s\n",print_move(from,to));
     	pea &= (pea - 1); // reset least significant bit
     	//printf("pea : %" PRIx64 "\n",pea,poa);
   	}

   	while (poa)
   	{
     	to = LSB(poa);
     	from = to + 9;
   		if(BIT[to] & b_promo)
			copy_promo(from, to, 0, -1, pBuf, pmovecount);
		else
			copy_normal(from, to, CAPTURE, -1, pBuf, pmovecount);
   		//printf("capture so : %s\n",print_move(from,to));
   		poa &= (poa - 1); // reset least significant bit
   	}

   	//moving two squares
   	tpb = (x >> 8) & (~bb[ALP]);
   	tpa = ((tpb & r_6) >> 8) & (~bb[ALP]);
   	//printf("tpb =   tpa = %" PRIx64 "  %" PRIx64 "\n",tpb,tpa);
   	while (tpa)
   	{
     	to = LSB(tpa);
     	from = to + 16;
     	copy_normal(from, to, NORMAL, to+8, pBuf, pmovecount);
     	tpa &= tpa - 1; // reset least significant bit
   	}

   	//moving one square
	dest_bp = (x >> 8) & bb[NOP];
   	while (dest_bp)
   	{
     	to = LSB(dest_bp);
   		from = to + 8;
   		if(BIT[to] & b_promo)
   			copy_promo(from, to, 0, -1, pBuf, pmovecount);
   		else
   			copy_normal(from, to, NORMAL, -1, pBuf, pmovecount);
   		dest_bp &= dest_bp - 1; // reset least significant bit
   	}
}

//GAN KNIGHTS MOVES
void gen_coups_cavaliers(int *pmovecount, MOVE * pBuf, U64 permit, int pce)
{
	U64 x;
	U64 allowed = permit;
	U64 dest;
	int from,to;

	x = bb[pce];

    while(x)
    {
	   from = LSB(x);
	   dest = mask_n[from];
	   dest &= allowed;
	   x &= (x - 1);
	   while(dest)
	   {
	   		to = LSB(dest);
			copy_normal(from, to, 0, -1, pBuf, pmovecount);
			dest &= (dest - 1);
	   }
    }
}

//GEN BISHOPS MOVES
void gen_coups_fous(int *pmovecount, MOVE * pBuf, U64 permit, int pce)
{
	U64 x;
	U64 atk;
	int from,to;
	U64 allowed = permit;

	x = bb[pce];
	atk = 0ULL;

	while(x)
	{
		from = LSB(x);
		x &= (x - 1);

		atk |= get_dir_lsb(mask_b_ne, from);
		atk |= get_dir_lsb(mask_b_no, from);
		atk |= get_dir_msb(mask_b_se, from);
		atk |= get_dir_msb(mask_b_so, from);

		atk &= allowed;

		while(atk)
		{
			to = LSB(atk);
			copy_normal(from, to, 0, -1, pBuf, pmovecount);
			atk &= (atk - 1);
		}
	}
}

//GEN ROOKS MOVES
void gen_coups_tours(int *pmovecount, MOVE * pBuf, U64 permit, int pce)
{
	U64 x;
	U64 atk;
	int from,to;
	U64 allowed = permit;

	x = bb[pce];
	atk = 0ULL;

	while(x)
	{
		from = LSB(x);
		x &= (x - 1);

		atk |= get_dir_lsb(mask_r_n, from);
		atk |= get_dir_msb(mask_r_s, from);
		atk |= get_dir_lsb(mask_r_e, from);
		atk |= get_dir_msb(mask_r_o, from);

		atk &= allowed;

		while(atk)
		{
			to = LSB(atk);
			copy_normal(from, to, 0, -1, pBuf, pmovecount);
			atk &= (atk - 1);
		}
	}
}

//GEN QUEENS MOVES
void gen_coups_dames(int *pmovecount, MOVE * pBuf, U64 permit, int pce)
{
	U64 x;
	U64 atk;
	int from,to;
	U64 allowed = permit;

	x = bb[pce];
	atk = 0ULL;

	while(x)
	{
		from = LSB(x);
		x &= (x - 1);

		atk |= get_dir_lsb(mask_b_ne, from);
		atk |= get_dir_lsb(mask_b_no, from);
		atk |= get_dir_msb(mask_b_se, from);
		atk |= get_dir_msb(mask_b_so, from);
		atk |= get_dir_lsb(mask_r_n, from);
		atk |= get_dir_msb(mask_r_s, from);
		atk |= get_dir_lsb(mask_r_e, from);
		atk |= get_dir_msb(mask_r_o, from);

		atk &= allowed;
		//afficher_bitboard(atk);

		while(atk)
		{
			to = LSB(atk);
			copy_normal(from, to, 0, -1, pBuf, pmovecount);
			atk &= (atk - 1);
		}
	}
}

//GEN KINGS MOVES
void gen_coups_rois(int *pmovecount, MOVE * pBuf, U64 permit, int pce)
{
	U64 x;
	U64 allowed = permit;
	U64 dest;
	int from,to,type;

	x = bb[pce];

	if(!x)  return;

	from = LSB(x);
	dest = mask_k[from];
	dest &= allowed;

	while(dest)
	{
	   	to = LSB(dest);
		copy_normal(from, to, type, -1, pBuf, pmovecount);
		dest &= (dest - 1);
	}
}


//PSEUDO LEGAL CAPTURES MOVES GENERATOR--------------------------------------------------------------------------------------
int gen_captures(int current_side, MOVE * pBuf)
{
	int movecount = 0;

	if(side == BLANC)
	{
		U64 permit = bb[BPC];
		gen_captures_pions_blancs(&movecount, pBuf);
		gen_coups_cavaliers(&movecount, pBuf, permit, WN);
		gen_coups_fous(&movecount, pBuf, permit, WB);
		gen_coups_tours(&movecount, pBuf, permit, WR);
		gen_coups_dames(&movecount, pBuf, permit, WQ);
		gen_coups_rois(&movecount, pBuf, permit, WK);
	}
	else
	{
		U64 permit = bb[WPC];
		gen_captures_pions_noirs(&movecount, pBuf);
		gen_coups_cavaliers(&movecount, pBuf, permit, BN);
		gen_coups_fous(&movecount, pBuf, permit, BF);
		gen_coups_tours(&movecount, pBuf, permit, BR);
		gen_coups_dames(&movecount, pBuf, permit, BQ);
		gen_coups_rois(&movecount, pBuf, permit, BK);
	}

	return movecount;
}


//GEN CAPTURES WHITE PAWNS
void gen_captures_pions_blancs(int *pmovecount, MOVE * pBuf)
{
	U64 pea;	//mean pawn east atack
	U64 poa; 	//mean pawn west atack
	int from,to;
	U64 allowed = bb[BPC];
	U64 x = bb[WP];
	U64 dest_wp;
	U64 ep_valid = 0x0;

	pea = ((x << 9) & ~f_a);
	poa = ((x << 7) & ~f_h);
	//en_passant
	if(ep != -1)
	{
		ep_valid = (pea & bb[NOP]);
		ep_valid &= BIT[ep];
		if(ep_valid)
			copy_ep(ep-9, ep, EN_PASSANT, -1, pBuf, pmovecount);
		ep_valid = (poa & bb[NOP]);
		ep_valid &= BIT[ep];
		if(ep_valid)
			copy_ep(ep-7, ep, EN_PASSANT, -1, pBuf, pmovecount);
	}

	//captures
	pea &= allowed;
	while (pea)
   	{
     	to = LSB(pea);
     	from = to - 9;
     	if(BIT[to] & b_promo)
   			copy_promo(from, to, 0, -1, pBuf, pmovecount);
		else
			copy_normal(from, to, CAPTURE, -1, pBuf, pmovecount);
     	pea &= pea - 1;
   	}
   	poa &= allowed;
   	while (poa)
   	{
     	to = LSB(poa);
     	from = to - 7;
     	if(BIT[to] & b_promo)
   			copy_promo(from, to, 0, -1, pBuf, pmovecount);
   		else
			copy_normal(from, to, CAPTURE, -1, pBuf, pmovecount);
   		poa &= poa - 1;
   	}

   	//avance une case seulement promo moves
	/*dest_wp = (x << 8) & bb[NOP];
   	while (dest_wp)
   	{
		to = LSB(dest_wp);
   		from = to - 8;
   		if(BIT[to] & w_promo)
   			copy_promo(from, to, 0, -1, pBuf, pmovecount);
   		dest_wp &= dest_wp - 1;
   	}*/
}

//GEN CAPT BLACK PAWNS--------------------------------------------------------------------------------------------------
void gen_captures_pions_noirs(int *pmovecount, MOVE * pBuf)
{
	U64 pea;	//mean pawn east atack
	U64 poa; 	//mean pawn west atack
	int from,to;
	U64 allowed = bb[WPC];
	U64 x = bb[BP];
	U64 dest_bp;
	U64 ep_valid;


   	pea = ((x >> 7) & ~f_a);
	poa = ((x >> 9) & ~f_h);
	//en_passant
	if(ep != -1)
	{
		ep_valid = (pea & bb[NOP]);
		ep_valid &= BIT[ep];
		if(ep_valid)
			copy_ep(ep+7, ep, EN_PASSANT, -1, pBuf, pmovecount);
		ep_valid = (poa & bb[NOP]);
		ep_valid &= BIT[ep];
		if(ep_valid)
			copy_ep(ep+9, ep, EN_PASSANT, -1, pBuf, pmovecount);
	}
	//captures
	pea &= allowed;
	while (pea)
   	{
     	to = (63 - MSB(pea));
     	from = to + 7;
     	if(BIT[to] & b_promo)
   			copy_promo(from, to, 0, -1, pBuf, pmovecount);
   		else
			copy_normal(from, to, CAPTURE, -1, pBuf, pmovecount);
     	pea &= pea - 1;
   	}
   	poa &= allowed;
	//printf("poa = %" PRIx64 "\n",poa);
   	while (poa)
   	{
     	to = (63 - MSB(poa));
     	from = to + 9;
     	if(BIT[to] & b_promo)
   			copy_promo(from, to, 0, -1, pBuf, pmovecount);
		else
			copy_normal(from, to, CAPTURE, -1, pBuf, pmovecount);
   		poa &= poa - 1;
   	}

   	//avance une case
	/*dest_bp = (x >> 8) & bb[NOP];
   	while (dest_bp)
   	{
     	to = LSB(dest_bp);
   		from = to + 8;
   		if(BIT[to] & b_promo)
   			copy_promo(from, to, 0, -1, pBuf, pmovecount);
   		dest_bp &= dest_bp - 1;
   	}*/
}

//GET LEAST significant bitin direction
U64 get_dir_lsb(U64 *ptab, int sq)
{
	U64 atk = 0x0;
	U64 slide = 0x0;
	int sq_pinned;

	atk = ptab[sq];
	slide = (ptab[sq] & bb[ALP]);
	if(slide)
	{
		sq_pinned = LSB(slide);
		atk &= (~ptab[sq_pinned]);
	}

	return atk;
}

//get most significant bit in a dir
U64 get_dir_msb(U64 *ptab, int sq)
{
	U64 atk = 0x0;
	U64 slide = 0x0;
	int sq_pinned;

	atk = ptab[sq];
	slide = (ptab[sq] & bb[ALP]);
	if(slide)
	{
		sq_pinned = (63 - MSB(slide));
		atk &= (~ptab[sq_pinned]);
	}

	return atk;
}

//function return true if a square 'sq' is atacked by the color 'c'
int square_atacked(int c, int sq)
{
	U64 b_atk = 0x0;
	U64 w_atk = 0x0;
	U64 square = BIT[sq];

	if(side == BLANC)
	{
		b_atk = get_b_atk();
		//printf("b atk et WK : %" PRIx64 "  %" PRIx64 "\n",w_atk,get_b_atk);
		if(b_atk & square)
			return VRAI;
		return FAUX;
	}
	else
	{
		w_atk = get_w_atk();
		//printf("w atk et BK : %" PRIx64 "  %" PRIx64 "\n",w_atk,get_b_atk);
		if(w_atk & square)
			return VRAI;
		return FAUX;
	}
	return FAUX;
}

//function return a bitboard of all the squares atacked by all the white pieces
U64 get_w_atk()
{
	U64 all_atk = 0x0;
	U64 atk_wp = 0x0;
	U64 atk_wb = 0x0;
	U64 atk_wn = 0x0;
	U64 atk_wr = 0x0;
	U64 atk_wq = 0x0;
	U64 atk_wk = 0x0;

	atk_wp = get_atk_wp(bb);
	all_atk |= atk_wp;
	atk_wb = get_atk_b(WB, bb);
	all_atk |= atk_wb;
	atk_wn = get_atk_n(WN, bb);
	all_atk |= atk_wn;
	atk_wr = get_atk_r(WR, bb);
	all_atk |= atk_wr;
	atk_wq = get_atk_q(WQ, bb);
	all_atk |= atk_wq;
	atk_wk = get_atk_k(WK, bb);
	all_atk |= atk_wk;

	if(show_atacks)
	{
		printf("ALL white\n");
		afficher_bitboard(all_atk);
	}

	return all_atk;
}

//function return a bitboard of all the squares atacked by all the black  pieces
U64 get_b_atk()
{
	U64 all_atk = 0x0;
	U64 atk_bp = 0x0;
	U64 atk_bb = 0x0;
	U64 atk_bn = 0x0;
	U64 atk_br = 0x0;
	U64 atk_bq = 0x0;
	U64 atk_bk = 0x0;

	atk_bp = get_atk_bp(bb);
	all_atk |= atk_bp;
	atk_bb = get_atk_b(BF, bb);
	all_atk |= atk_bb;
	atk_bn = get_atk_n(BN, bb);
	all_atk |= atk_bn;
	atk_br = get_atk_r(BR, bb);
	all_atk |= atk_br;
	atk_bq = get_atk_q(BQ, bb);
	all_atk |= atk_bq;
	atk_bk = get_atk_k(BK, bb);
	all_atk |= atk_bk;

    if(show_atacks)
	{
		printf("ALL black\n");
		afficher_bitboard(all_atk);
	}

	return all_atk;
}


//atacks from white pawns
U64 get_atk_wp(U64 *pbitboard)
{
	U64 pea;	//mean pawn east atack
	U64 poa; 	//mean pawn west atack
	U64 atk = 0x0;
	U64 x = pbitboard[WP];

	//captures
   	pea = ((x << 9) & (~f_a));
   	atk |= pea;
	poa = ((x << 7) & (~f_h));
   	atk |= poa;

   	if(show_atacks)
	{
		printf("Pion blanc\n");
		afficher_bitboard(atk);
	}

   	return atk;
}

//atacks from blacks pawns
U64 get_atk_bp(U64 *pbitboard)
{
	U64 pea;	//mean pawn east atack
	U64 poa; 	//mean pawn west atack
	U64 atk = 0x0;
	U64 x = pbitboard[BP];

	//captures
   	pea = ((x >> 7) & (~f_a));
   	atk |= pea;
	poa = ((x >> 9) & (~f_h));
   	atk |= poa;

   	if(show_atacks)
	{
	   	printf("Pion noir\n");
		afficher_bitboard(atk);
   	}

   	return atk;
}

//atacks from knights
U64 get_atk_n(int pce, U64 *pbitboard)
{
	U64 x;
	U64 dest;
	U64 atk = 0x0;
	int from;

	x = pbitboard[pce];

    while(x)
    {
	   from = LSB(x);
	   dest = mask_n[from];
	   atk |= dest;
	   x &= (x - 1);
    }

    if(show_atacks)
	{
    	printf("Cavalier\n");
		afficher_bitboard(atk);
	}
    return atk;
}

//atacks from bishops
U64 get_atk_b(int pce, U64 *pbitboard)
{
	U64 x;
	U64 atk = 0x0;
	int from;

	x = pbitboard[pce];
	atk = 0ULL;

	while(x)
	{
		from = LSB(x);
		x &= (x - 1);

		atk |= get_dir_lsb(mask_b_ne, from);
		atk |= get_dir_lsb(mask_b_no, from);
		atk |= get_dir_msb(mask_b_se, from);
		atk |= get_dir_msb(mask_b_so, from);
	}

	if(show_atacks)
	{
		printf("Fou\n");
		afficher_bitboard(atk);
	}
	return atk;
}

//atacks from rooks
U64 get_atk_r(int pce, U64 *pbitboard)
{
	U64 x;
	U64 atk = 0x0;
	int from;

	x = pbitboard[pce];
	atk = 0ULL;

	while(x)
	{
		from = LSB(x);
		x &= (x - 1);

		atk |= get_dir_lsb(mask_r_n, from);
		atk |= get_dir_msb(mask_r_s, from);
		atk |= get_dir_lsb(mask_r_e, from);
		atk |= get_dir_msb(mask_r_o, from);
	}

	if(show_atacks)
	{
		printf("Tour\n");
		afficher_bitboard(atk);
	}
	return atk;
}

//atacks from queens
U64 get_atk_q(int pce, U64 *pbitboard)
{
	U64 x;
	U64 atk = 0x0;
	int from;

	x = pbitboard[pce];

	while(x)
	{
		from = LSB(x);
		x &= (x - 1);

		atk |= get_dir_lsb(mask_b_ne, from);
		atk |= get_dir_lsb(mask_b_no, from);
		atk |= get_dir_msb(mask_b_se, from);
		atk |= get_dir_msb(mask_b_so, from);
		atk |= get_dir_lsb(mask_r_n, from);
		atk |= get_dir_msb(mask_r_s, from);
		atk |= get_dir_lsb(mask_r_e, from);
		atk |= get_dir_msb(mask_r_o, from);
	}

	if(show_atacks)
	{
		printf("Dame\n");
		afficher_bitboard(atk);
	}
	return atk;
}

//atacks from kings
U64 get_atk_k(int pce, U64 *pbitboard)
{
	U64 x = 0x0;
	U64 dest = 0x0;
	int from;

	x = pbitboard[pce];

	if(!x)
		return 0;

	from = LSB(x);
	dest = mask_k[from];

	if(show_atacks)
	{
		printf("Roi\n");
		afficher_bitboard(dest);
	}

    return dest;
}

