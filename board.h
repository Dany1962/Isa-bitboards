#ifndef BOARD_H_INCLUDED
#define BOARD_H_INCLUDED

#include "bitboard.h"

#define VRAI 1
#define FAUX 0

//couleur
#define BLANC   0
#define NOIR    1
#define OPP(x)  ((x == 1) ? 0 : 1)

#define MAXPLY 100

//pieces indexes
#define WP		    1	//white pawn
#define WN		    2	//white knight
#define WB	        3	//white bishop
#define WR	        4	//white rook
#define WQ	        5	//white queen
#define WK	        6	//white king

#define BP		    7	//black pawn
#define BN		    8	//black knight
#define BF	        9	//black bishop
#define BR	       10	//black rook
#define BQ	       11	//black queen
#define BK	       12	//black king

#define WPC			13	//white pieces
#define BPC			14	//black pieces
#define ALP			15	//all pieces
#define NOP			16	//no pieces

#define VIDE 		0

//TYPES DE COUPS
#define NONE	   10
#define NORMAL		1	//simple move
#define CAPTURE		2	//capture
#define EN_PASSANT	3	//ep move
#define PETIT_ROQUE	4	//short castle
#define GRAND_ROQUE 5	//long castle
#define Q_PROMO		6
#define R_PROMO		7
#define B_PROMO		8
#define N_PROMO		9

//CASES ALPHA
#define A1   0
#define B1   1
#define C1   2
#define D1   3
#define E1   4
#define F1   5
#define G1   6
#define H1   7
#define A2   8
#define B2   9
#define C2  10
#define D2  11
#define E2  12
#define F2  13
#define G2  14
#define H2  15
#define A3  16
#define B3  17
#define C3  18
#define D3  19
#define E3  20
#define F3  21
#define G3  22
#define H3  23
#define A4  24
#define B4  25
#define C4  26
#define D4  27
#define E4  28
#define F4  29
#define G4  30
#define H4  31
#define A5  32
#define B5  33
#define C5  34
#define D5  35
#define E5  36
#define F5  37
#define G5  38
#define H5  39
#define A6  40
#define B6  41
#define C6  42
#define D6  43
#define E6  44
#define F6  45
#define G6  46
#define H6  47
#define A7  48
#define B7  49
#define C7  50
#define D7  51
#define E7  52
#define F7  53
#define G7  54
#define H7  55
#define A8  56
#define B8  57
#define C8  58
#define D8  59
#define E8  60
#define F8  61
#define G8  62
#define H8  63

int echiquier[64];
static const int init_echiquier[64] =
{
	   4,   2,   3,   5,   6,   3,   2,   4,
	   1,   1,   1,   1,   1,   1,   1,   1,
	VIDE,VIDE,VIDE,VIDE,VIDE,VIDE,VIDE,VIDE,
	VIDE,VIDE,VIDE,VIDE,VIDE,VIDE,VIDE,VIDE,
	VIDE,VIDE,VIDE,VIDE,VIDE,VIDE,VIDE,VIDE,
	VIDE,VIDE,VIDE,VIDE,VIDE,VIDE,VIDE,VIDE,
	   7,   7,   7,   7,   7,   7,   7,   7,
	  10,   8,   9,  11,  12,   9,   8,  10
};

typedef struct tag_MOVE
{
    int from;
    int dest;
    int piece_from;
    int piece_dest;
    int type;
    int ep_flag;
    int evaluation;
    int killer_code;
}   MOVE;


typedef struct tag_HIST
{
    MOVE m;
    int cap;
    int castle;
    int ep;
    int cinquante;
    U64 hash_code;
}               HIST;

HIST hist[500];

int side;           //couleur qui joue (etat : blanc, noir, vide)
int computer_side;  //couleur du programme
int hdp;            //numéro du coup en cours ds la recherche ou la partie
//int nodes;          //compteur de noeuds visités
int q_nodes;        // noeuds visités qs
int prof;           //profondeur de recherche
int max_depth;      //profondeur de recherche maximum
int castle;         //roque
int ep;             //en passant
int cinquante;      //règle des cinquante coups
int prof;

static const int castle_mask[64] =
{

	13, 15, 15, 15, 12, 15, 15, 14,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15,
     7, 15, 15, 15,  3, 15, 15, 11,
};

//Variables pour tests
int show_atacks;
int show_roques;
int show_move;
int bump_tree;
int e_valide;
int w_castle;
int b_castle;

//FONCTIONS


#endif // BOARD_H_INCLUDED
