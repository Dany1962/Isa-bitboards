#ifndef UTILS_H_INCLUDED
#define UTILS_H_INCLUDED

#include "board.h"
#include "move.h"

//FONCTIONS
char *coord_une_case(int sq);
char *print_move(int f, int t);
void affiche_echiquier();
void affiche_petit_echiquier();
void affiche_options();
void affiche_liste(int ctr, MOVE *pbuf);
void affiche_bitboards();
void afficher_bitboard();
int verif_coup(char *s);
void position_to_fen();
void fen_to_position(char *ptab);
void affiche_resultat();
U64 perft (int p);
U64 divide (int p);
int timeout();
void trier();
void affiche_coups_joues();

#endif // EDIT_H_INCLUDED
