#ifndef GEN_H_INCLUDED
#define GEN_H_INCLUDED

#include "board.h"

#define BONUS_CAPT      50000
#define BONUS_PROMO     40000
#define BONUS_ROQUE     30000

//castle masks
#define WK_CASTLE 0x0000000000000060
#define WQ_CASTLE 0x000000000000000C
#define BK_CASTLE 0x6000000000000000
#define BQ_CASTLE 0x0C00000000000000

//FONCTIONS
//GEN DE TOUT LES COUPS
int gen_coups(int current_side, MOVE * pBuf);
void gen_coups_pions_blancs(int *pmovecount, MOVE * pBuf, U64 *pbit);
void gen_coups_pions_noirs(int *pmovecount, MOVE * pBuf);
void gen_coups_cavaliers(int *pmovecount, MOVE * pBuf, U64 permit, int pce);
void gen_coups_fous(int *pmovecount, MOVE * pBuf, U64 permit, int pce);
void gen_coups_tours(int *pmovecount, MOVE * pBuf, U64 permit, int pce);
void gen_coups_dames(int *pmovecount, MOVE * pBuf, U64 permit, int pce);
void gen_coups_rois(int *pmovecount, MOVE * pBuf,U64 permit, int pce);
void gen_roques_blancs(int *pmovecount, MOVE * pBuf);
void gen_roques_noirs(int *pmovecount, MOVE * pBuf);
//GEN CAPTURES SEULEMENT
int gen_captures(int current_side, MOVE * pBuf);
void gen_captures_pions_blancs(int *pmovecount, MOVE * pBuf);
void gen_captures_pions_noirs(int *pmovecount, MOVE * pBuf);
//COPIE COUPS
void copy_normal(int f, int t, int type, int epf, MOVE * pBuf, int *pMCount);
void copy_castle(int f, int t, int type, int epf, MOVE * pBuf, int *pMCount);
void copy_ep(int f, int t, int type, int epf, MOVE * pBuf, int *pMCount);
void copy_castle(int f, int t, int type, int epf, MOVE * pBuf, int *pMCount);
void copy_promo(int f, int t, int type, int epf, MOVE * pBuf, int *pMCount);
//SCORE DES COUPS
int order_moves(int from, int to, int type, int *ptab);
//fonction retourne vrai si la case sq est attaquée par couleur c
int square_atacked(int c, int sq);

U64 get_dir_lsb(U64 *ptab, int sq);
U64 get_dir_msb(U64 *ptab, int sq);

//FONCTIONS RECHERCHE DES PIECES CLOUEES
U64 get_w_pinned();
U64 get_b_pinned();
//fonctions retournent un bitboard de toutes les cases attaquées par toute les pièces
U64 get_w_atk();
U64 get_b_atk();
U64 get_atk_wp(U64 *pbitboard);
U64 get_atk_bp(U64 *pbitboard);
U64 get_atk_n(int pce, U64 *pbitboard);
U64 get_atk_b(int pce, U64 *pbitboard);
U64 get_atk_r(int pce, U64 *pbitboard);
U64 get_atk_q(int pce, U64 *pbitboard);
U64 get_atk_k(int pce, U64 *pbitboard);

int king_in_check(int c);
#endif // GEN_H_INCLUDED
