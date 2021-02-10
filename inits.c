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
#include "hash.h"
#include "time.h"

void init()
{
	init_bitboards();
	init_game();
	init_eval();
	init_hash();
}

void init_bitboards()
{
    int i;
    U64 x = 0X0000000000000001;

    //init bitboards
    bb[0] = 0x0000000000000000;
    //WP
    bb[1] = 0x000000000000FF00;
    bb[2] = 0x0000000000000042;
    bb[3] = 0x0000000000000024;
    bb[4] = 0x0000000000000081;
    bb[5] = 0x0000000000000008;
    bb[6] = 0x0000000000000010;
    //BP
    bb[7]  = 0x00FF000000000000;
    bb[8]  = 0x4200000000000000;
    bb[9]  = 0x2400000000000000;
    bb[10] = 0x8100000000000000;
    bb[11] = 0x0800000000000000;
    bb[12] = 0x1000000000000000;
    //OTHERS
    bb[13] = 0x000000000000FFFF;
    bb[14] = 0xFFFF000000000000;
    bb[15] = 0xFFFF00000000FFFF;
    bb[16] = 0x0000FFFFFFFF0000;

	//init BIT
	for(i=0;i<64;++i)
    {
    	BIT[i] = x;
    	x <<= 1;
    	//printf("i : %d   BIT[i] : %" PRIx64 "\n",i,BIT[i]);
	}

   	//masks
   	mask_roi();
   	mask_cavalier();
   	mask_fou();
	mask_tour();
}

void init_game()
{
	int sq;

	//init echiquier
	for(sq=0;sq<64;++sq)
		echiquier[sq] = init_echiquier[sq];

	side = BLANC;               //couleur qui joue
    computer_side = NOIR;       //couleur des pieces du programme (par défaut NOIR)
    max_depth = 4;              //profondeur de recherche (par defaut 4 coups)
    time_left = 2000000;
    hdp = 0;                    //nbre de coups joués
    castle = 15;
    ep = -1;
    cinquante = 0;
    prof = 0;

}

void init_eval()
{
	int sq;
	int i;

	for(i=0;i<64;++i)
	{
		mask_front_wp[i] = 0x0;
		mask_front_bp[i] = 0x0;
	}

	//init masques pions passés
	for(sq=8;sq<55;++sq)
	{
		mask_front_wp[sq] |= mask_r_n[sq];
		if(COL(sq)!= 0)
			mask_front_wp[sq] |= mask_r_n[sq-1];
		if(COL(sq)!= 7)
			mask_front_wp[sq] |= mask_r_n[sq+1];
	}

	for(sq=8;sq<55;++sq)
	{
		mask_front_bp[sq] |= mask_r_s[sq];
		if(COL(sq)!= 0)
			mask_front_bp[sq] |= mask_r_s[sq-1];
		if(COL(sq)!= 7)
			mask_front_bp[sq] |= mask_r_s[sq+1];
	}
	
	//init psqt
	for(i=0;i<64;++i)
	{
		//MG
		psqt_mg[WP][i] = pst_pb_mg[i];
		psqt_mg[BP][i] = pst_pb_mg[flip[i]];
		psqt_mg[WN][i] = pst_cb_mg[i];
		psqt_mg[BN][i] = pst_cb_mg[flip[i]];
		psqt_mg[WB][i] = pst_fb_mg[i];
		psqt_mg[BF][i] = pst_fb_mg[flip[i]];
		psqt_mg[WR][i] = pst_tb_mg[i];
		psqt_mg[BR][i] = pst_tb_mg[flip[i]];
		psqt_mg[WQ][i] = pst_db_mg[i];
		psqt_mg[BQ][i] = pst_db_mg[flip[i]];
		psqt_mg[WK][i] = pst_rb_mg[i];
		psqt_mg[BK][i] = pst_rb_mg[flip[i]];
		//EG
		psqt_eg[WP][i] = pst_pb_eg[i];
		psqt_eg[BP][i] = pst_pb_eg[flip[i]];
		psqt_eg[WN][i] = pst_cb_eg[i];
		psqt_eg[BN][i] = pst_cb_eg[flip[i]];
		psqt_eg[WB][i] = pst_fb_eg[i];
		psqt_eg[BF][i] = pst_fb_eg[flip[i]];
		psqt_eg[WR][i] = pst_tb_eg[i];
		psqt_eg[BR][i] = pst_tb_eg[flip[i]];
		psqt_eg[WQ][i] = pst_db_eg[i];
		psqt_eg[BQ][i] = pst_db_eg[flip[i]];
		psqt_eg[WK][i] = pst_rb_eg[i];
		psqt_eg[BK][i] = pst_rb_eg[flip[i]];
	}
	
	//init coef phase
	for(i=0; i<25; ++i)
    {
        coef_phase[i] = (i * 256 + (24 / 2)) / 24;
        printf("phase et coef : %2d  %3d\n",i,coef_phase[i]);
    }
}

void init_hash()
{
    init_hash_table();
    hash_position = init_code_position();
}
