#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
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

void edit_pour_debug()
{
	int d;                      //case selectionnée par l'utilisateur
    char s[256];                //chaine entrée par l'utilisateur
    int color = BLANC;
    int fin_saisie = FAUX;
    int prb = 0,grb = 0,prn = 0,grn = 0;
    int i;
    
    vider_echiquier();
    
    for(;;)                     //boucle infinie de saisie
    {
		affiche_echiquier();

        scanf("%s", s);
        d = s[1] - 'a';
        d += 8 * (s[2] - '1');
        
        switch(s[0])
        {
        case 'P':
        	(color == BLANC ? add_piece(BLANC, d, WP) : add_piece(NOIR, d, BP));
            break;
        case 'N':
			(color == BLANC ? add_piece(BLANC, d, WN) : add_piece(NOIR, d, BN));            
			break;
        case 'B':
			(color == BLANC ? add_piece(BLANC, d, WB) : add_piece(NOIR, d, BF));
            break;
        case 'R':
			(color == BLANC ? add_piece(BLANC, d, WR) : add_piece(NOIR, d, BR));
            break;
        case 'Q':
			(color == BLANC ? add_piece(BLANC, d, WQ) : add_piece(NOIR, d, BQ));
            break;
        case 'K':
			(color == BLANC ? add_piece(BLANC, d, WK) : add_piece(NOIR, d, BK));
            break;
        case '.':
        	fin_saisie = VRAI;
        	ep = -1;
        	if(BIT[4] & bb[WK])
        	{
        		if(BIT[7] & bb[WR])
        			prb = 1;
        		if(BIT[0] & bb[WR])
        			grb = 2;
			}
    		if(BIT[60] & bb[BK])
        	{
        		if(BIT[63] & bb[BR])
        			prn = 4;
        		if(BIT[56] & bb[BR])
        			grn = 8;
			}
			//printf("%d %d %d %d \n",prb,grb,prn,grn);
			castle = (prb ^ grb ^ prn ^ grn);
            break;
        case 'x': 
            break;
        case '#':
            vider_echiquier();
            break;
        case 'c':  //change de couleur
            color = ((color == BLANC) ? NOIR : BLANC);
            break;
        default :
            break;
        }
        if(fin_saisie)
            break;
    }
    hdp = 1;
    bb[NOP] = (~bb[ALP]);
    affiche_echiquier();
    for(i=0;i<17;++i)
		printf("i :  %d   %" PRIx64 "\n",i,bb[i]);
}

void add_piece(int c, int sq, int p)
{
	if(echiquier[sq] != VIDE)
		remove_piece(c, sq, p);
		
	 echiquier[sq] = p;
	 
	//bitboard
	set_bit(bb, sq, ALP);
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

void remove_piece(int c, int sq, int p)
{
	clear_bit(bb, sq, ALP);
	
	echiquier[sq] = VIDE;
	
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

void vider_echiquier()
{
	int i;
	
	//remove the bitboards
	for(i=0;i<17;++i)
		bb[i] = 0x0000000000000000;
    
    memset(echiquier, 0, sizeof(echiquier));
    castle = 0;
}


