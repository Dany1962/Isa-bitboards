#ifndef BITBOARD_H_INCLUDED
#define BITBOARD_H_INCLUDED

typedef unsigned long long  U64;

#define LSB(x) (__builtin_ctzll(x))
#define MSB(x) (__builtin_clzll(x))
#define POP(x) (__builtin_popcountll(x))

//rangées et colonnes
#define r_1     0x00000000000000FF
#define r_2     0x000000000000FF00
#define r_3     0x0000000000FF0000
#define r_4     0x00000000FF000000
#define r_5     0x000000FF00000000
#define r_6     0x0000FF0000000000
#define r_7     0x00FF000000000000
#define r_8     0xFF00000000000000

#define f_a     0x0101010101010101
#define f_b     0x0202020202020202
#define f_c     0x0404040404040404
#define f_d     0x0808080808080808
#define f_e     0x1010101010101010
#define f_f     0x2020202020202020
#define f_g     0x4040404040404040
#define f_h     0x8080808080808080

#define ROW(x)  ((x) >> 3)
#define COL(x) ((x) & 7)

//TABLE BITBOARDS :
//indice 0  	: bitboard vide
//indices 1-6 	: bitboards pièces blanches
//indices 7-12	: bitboards pièces noires
//indice 13		: bitboard de toute les pièces blanches		(bb 13 |= bb 1 à bb 6)
//indice 14		: bitboard de toute les pièces noires		(bb 14 |= bb 7 à bb 12)
//indice 15		: bitboard des pièces noires et blanches	(bb 15 |= bb 13 | bb 14)
//indice 16		: bitboard des cases vides 					(bb 16 = ~bb 15)
U64 bb[17];

static const U64 w_promo = r_8;
static const U64 b_promo = r_1;

static const U64 notAFile = 0xfefefefefefefefe;
static const U64 notHFile = 0x7f7f7f7f7f7f7f7f;
static const U64 notRank8 = 0x00ffffffffffffff;
static const U64 notRank1 = 0xffffffffffffff00;

//MASQUES

//pour le déplacement d'un pion blanc d'une case    : case pion >> 8
//pour le déplacement d'un pion blanc de deux cases : case pion >> 16
//pour le déplacement d'un pion noir  d'une case    : case pion << 8
//pour le déplacement d'un pion noir  de deux cases : case pion << 16
//cavalier
U64 mask_n[64];
//fou
U64 mask_bishop[64];
U64 mask_b_no[64];
U64 mask_b_ne[64];
U64 mask_b_so[64];
U64 mask_b_se[64];
//tour
U64 mask_rook[64];
U64 mask_r_n[64];
U64 mask_r_s[64];
U64 mask_r_e[64];
U64 mask_r_o[64];
//dame = tour + fou
//roi
U64 mask_k[64];
U64 mask_wkinc[64];   //white pawns atack mask
U64 mask_bkinc[64];		//black pawns atack mask
U64 BIT[64];


//FONCTIONS
void mask_roi();
void mask_cavalier();
void mask_fou();
void mask_tour();
void set_bit(U64 *pbitboard, int y, int type);		//met le bit numéro y du bitboard à 1
void clear_bit(U64 *pbitboard, int y, int type);		//met le bit numéro y du bitboard à 1


#endif // BITBOARD_H_INCLUDED
