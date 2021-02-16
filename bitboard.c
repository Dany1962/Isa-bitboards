#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <time.h>
#include <inttypes.h>

#include "bitboard.h"

//knight's masks#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <time.h>
#include <inttypes.h>

#include "bitboard.h"

//knight's masks
void mask_cavalier()
{
	int i,y;
	int col;
	U64 x = 0x0000000000000001;
	
	for(i=0;i<64;++i)
	{
		col = COL(i);
		mask_n[i] = 0x0;
    	y = i - 6;
    	if (y >= 0 && col < 6)
    		mask_n[i] |= (x << y);
    	y = i - 10;
    	if (y >= 0 && col > 1)
    		mask_n[i] |= (x << y);
    	y = i - 15;
    	if (y >= 0 && col < 7)
    		mask_n[i] |= (x << y);
    	y = i - 17;
    	if (y >= 0 && col > 0)
    		mask_n[i] |= (x << y);
    	y = i + 6;
    	if (y < 64 && col > 1)
    		mask_n[i] |= (x << y);
    	y = i + 10;
		if (y < 64 && col < 6)
			mask_n[i] |= (x << y);
    	y = i + 15;
    	if (y < 64 && col > 0)
    		mask_n[i] |= (x << y);
    	y = i + 17;
    	if (y < 64 && col < 7)
    		mask_n[i] |= (x << y);
	}
	
	/*for(i=0;i<64;++i)
	{
		printf("sq = %d   mask = %" PRIx64 "\n",i,mask_n[i]);
	}*/
	
}

//bishop's masks
void mask_fou()
{
	int i,y;
	U64 x = 0x0000000000000001;
	
	for(i=0;i<64;++i)
	{
		mask_b_no[i] = 0x0;
		mask_b_ne[i] = 0x0;
		mask_b_so[i] = 0x0;
		mask_b_se[i] = 0x0;
		mask_bishop[i] = 0x0;
		for (y = i - 9; y >= 0 && COL(y) != 7; y -= 9) //sud ouest
			mask_b_so[i] |= (x << y);
        for (y = i - 7; y >= 0 && COL(y) != 0; y -= 7) //sud est
            mask_b_se[i] |= (x << y);     
        for (y = i + 9; y < 64 && COL(y) != 0; y += 9) //nord est
        	mask_b_no[i] |= (x << y);
        for (y = i + 7; y < 64 && COL(y) != 7; y += 7) //nord ouest
        	mask_b_ne[i] |= (x << y);
		mask_bishop[i] |= mask_b_so[i];
		mask_bishop[i] |= mask_b_se[i];
		mask_bishop[i] |= mask_b_ne[i];
		mask_bishop[i] |= mask_b_no[i];
	}
	
	/*for(i=0;i<64;++i)
	{
		printf("sq = %d   mask = %" PRIx64 "\n",i,mask_b_ne[i]);
	}*/
}

//rook masks
void mask_tour()
{
	int i,y,k;
	int col;
	U64 x = 0x0000000000000001;	
	
	for(i=0;i<64;++i)
	{
		mask_r_n[i] = 0x0;
		mask_r_s[i] = 0x0;
		mask_r_e[i] = 0x0;
	 	mask_r_o[i] = 0x0;
	 	mask_rook[i] = 0x0;
		col = COL(i);
        for (k = i - col, y = i - 1; y >= k; y--) //ouest
        	mask_r_o[i] |= (x << y);	
        for (k = i - col + 7, y = i + 1; y <= k; y++) //est
            mask_r_e[i] |= (x << y);
        for (y = i - 8; y >= 0; y -= 8)	//sud
            mask_r_s[i] |= (x << y);
        for (y = i + 8; y < 64; y += 8) //nord
            mask_r_n[i] |= (x << y);
            
        mask_rook[i] |= mask_r_o[i];
        mask_rook[i] |= mask_r_e[i];
        mask_rook[i] |= mask_r_s[i];
        mask_rook[i] |= mask_r_n[i];
	}
	
	/*for(i=0;i<64;++i)
	{
		printf("sq = %d   mask = %" PRIx64 "\n",i,mask_rook[i]);
	}*/
}

//kings masks
void mask_roi()
{
	int i;
	int col;
	int sq;
	U64 x = 0x0000000000000001;

	for(i=0;i<64;++i)
	{
		mask_k[i] = 0x0;
		col = COL(i);
    	if (col)
    	{
    		sq = i-1;
    		mask_k[i] |= (x << sq);
		}
    	if (col < 7)
    	{
    		sq = i+1;
    		mask_k[i] |=  (x << sq);
		}
    	if (i > 7)
    	{
    		sq = i-8;
    		mask_k[i] |=  (x << sq);
		}
    	if (i < 56)
    	{
    		sq = i+8;
    		mask_k[i] |=  (x << sq);
		}
    	if (col && i > 7)
    	{
    		sq = i-9;
    		mask_k[i] |=  (x << sq);
		}
    	if (col < 7 && i > 7)
    	{
    		sq = i-7;
    		mask_k[i] |=  (x << sq);
		}
		if (col && i < 56)
		{
    		sq = i+7;
    		mask_k[i] |=  (x << sq);
		}
    	if (col < 7 && i < 56)
    	{
    		sq = i+9;
			mask_k[i] |=  (x << sq);
		}
	}
	
	x = 0x0000000000000001;
	
	//king in check by pawns mask
	for(i=0;i<64;++i)
	{		
		mask_wkinc[i] = 0x0;
		mask_bkinc[i] = 0x0;
		col = COL(i);
		
		if(col)
		{
			mask_wkinc[i] |= (BIT[i+7]);
			mask_bkinc[i] |= (BIT[i-9]);
		}
		if(col != 7)
		{
			mask_wkinc[i] |= (BIT[i+9]);
			mask_bkinc[i] |= (BIT[i-7]);
		}			
	}
	
	/*for(i=0;i<64;++i)
	{
		printf("sq = %d   mask = %" PRIx64 "\n",i,mask_bkinc[i]);
	}*/
}

//put a bit to '1' in a bitboard
//*pbitboard  bb[] array
//y = the square
//type = bitboard to modify (bb[] array
void set_bit(U64 *pbitboard, int y, int type)  
{
	/*U64 x = pbitboard[type];
		
	x = (BIT[y] | x);
	
	pbitboard[type] = x;*/
	pbitboard[type] |= BIT[y];	
}

//put a bit to '0' in a bitboard 
void clear_bit(U64 *pbitboard, int y, int type)
{
	/*U64 x = pbitboard[type];
	
	x = (x ^ BIT[y]);
	
	pbitboard[type] = x;*/
	pbitboard[type] ^= BIT[y];	
}



void mask_cavalier()
{
	int i,y;
	int col;
	U64 x = 0x0000000000000001;
	
	for(i=0;i<64;++i)
	{
		col = COL(i);
		mask_n[i] = 0x0;
    	y = i - 6;
    	if (y >= 0 && col < 6)
    		mask_n[i] |= (x << y);
    	y = i - 10;
    	if (y >= 0 && col > 1)
    		mask_n[i] |= (x << y);
    	y = i - 15;
    	if (y >= 0 && col < 7)
    		mask_n[i] |= (x << y);
    	y = i - 17;
    	if (y >= 0 && col > 0)
    		mask_n[i] |= (x << y);
    	y = i + 6;
    	if (y < 64 && col > 1)
    		mask_n[i] |= (x << y);
    	y = i + 10;
		if (y < 64 && col < 6)
			mask_n[i] |= (x << y);
    	y = i + 15;
    	if (y < 64 && col > 0)
    		mask_n[i] |= (x << y);
    	y = i + 17;
    	if (y < 64 && col < 7)
    		mask_n[i] |= (x << y);
	}
	
	/*for(i=0;i<64;++i)
	{
		printf("sq = %d   mask = %" PRIx64 "\n",i,mask_n[i]);
	}*/
	
}

//bishop's masks
void mask_fou()
{
	int i,y;
	U64 x = 0x0000000000000001;
	
	for(i=0;i<64;++i)
	{
		mask_b_no[i] = 0x0;
		mask_b_ne[i] = 0x0;
		mask_b_so[i] = 0x0;
		mask_b_se[i] = 0x0;
		mask_bishop[i] = 0x0;
		for (y = i - 9; y >= 0 && COL(y) != 7; y -= 9) //sud ouest
			mask_b_so[i] |= (x << y);
        for (y = i - 7; y >= 0 && COL(y) != 0; y -= 7) //sud est
            mask_b_se[i] |= (x << y);     
        for (y = i + 9; y < 64 && COL(y) != 0; y += 9) //nord est
        	mask_b_no[i] |= (x << y);
        for (y = i + 7; y < 64 && COL(y) != 7; y += 7) //nord ouest
        	mask_b_ne[i] |= (x << y);
		mask_bishop[i] |= mask_b_so[i];
		mask_bishop[i] |= mask_b_se[i];
		mask_bishop[i] |= mask_b_ne[i];
		mask_bishop[i] |= mask_b_no[i];
	}
	
	/*for(i=0;i<64;++i)
	{
		printf("sq = %d   mask = %" PRIx64 "\n",i,mask_b_ne[i]);
	}*/
}

//rook masks
void mask_tour()
{
	int i,y,k;
	int col;
	U64 x = 0x0000000000000001;	
	
	for(i=0;i<64;++i)
	{
		mask_r_n[i] = 0x0;
		mask_r_s[i] = 0x0;
		mask_r_e[i] = 0x0;
	 	mask_r_o[i] = 0x0;
	 	mask_rook[i] = 0x0;
		col = COL(i);
        for (k = i - col, y = i - 1; y >= k; y--) //ouest
        	mask_r_o[i] |= (x << y);	
        for (k = i - col + 7, y = i + 1; y <= k; y++) //est
            mask_r_e[i] |= (x << y);
        for (y = i - 8; y >= 0; y -= 8)	//sud
            mask_r_s[i] |= (x << y);
        for (y = i + 8; y < 64; y += 8) //nord
            mask_r_n[i] |= (x << y);
            
        mask_rook[i] |= mask_r_o[i];
        mask_rook[i] |= mask_r_e[i];
        mask_rook[i] |= mask_r_s[i];
        mask_rook[i] |= mask_r_n[i];
	}
	
	/*for(i=0;i<64;++i)
	{
		printf("sq = %d   mask = %" PRIx64 "\n",i,mask_rook[i]);
	}*/
}

//kings masks
void mask_roi()
{
	int i;
	int col;
	int sq;
	U64 x = 0x0000000000000001;

	for(i=0;i<64;++i)
	{
		mask_k[i] = 0x0;
		col = COL(i);
    	if (col)
    	{
    		sq = i-1;
    		mask_k[i] |= (x << sq);
		}
    	if (col < 7)
    	{
    		sq = i+1;
    		mask_k[i] |=  (x << sq);
		}
    	if (i > 7)
    	{
    		sq = i-8;
    		mask_k[i] |=  (x << sq);
		}
    	if (i < 56)
    	{
    		sq = i+8;
    		mask_k[i] |=  (x << sq);
		}
    	if (col && i > 7)
    	{
    		sq = i-9;
    		mask_k[i] |=  (x << sq);
		}
    	if (col < 7 && i > 7)
    	{
    		sq = i-7;
    		mask_k[i] |=  (x << sq);
		}
		if (col && i < 56)
		{
    		sq = i+7;
    		mask_k[i] |=  (x << sq);
		}
    	if (col < 7 && i < 56)
    	{
    		sq = i+9;
			mask_k[i] |=  (x << sq);
		}
	}
	
	x = 0x0000000000000001;
	
	//king in check by pawns mask
	for(i=0;i<64;++i)
	{		
		mask_wkinc[i] = 0x0;
		mask_bkinc[i] = 0x0;
		col = COL(i);
		
		if(col)
		{
			mask_wkinc[i] |= (BIT[i+7]);
			mask_bkinc[i] |= (BIT[i-9]);
		}
		if(col != 7)
		{
			mask_wkinc[i] |= (BIT[i+9]);
			mask_bkinc[i] |= (BIT[i-7]);
		}			
	}
	
	/*for(i=0;i<64;++i)
	{
		printf("sq = %d   mask = %" PRIx64 "\n",i,mask_bkinc[i]);
	}*/
}

//put a bit to '1' in a bitboard
//*pbitboard  bb[] array
//y = the square
//type = bitboard to modify (bb[] array
void set_bit(U64 *pbitboard, int y, int type)  
{
	U64 x = pbitboard[type];
		
	x = (BIT[y] | x);
	
	pbitboard[type] = x;	
}

//put a bit to '0' in a bitboard 
void clear_bit(U64 *pbitboard, int y, int type)
{
	U64 x = pbitboard[type];
	
	x = (x ^ BIT[y]);
	
	pbitboard[type] = x;
}


