#ifndef MOVE_H_INCLUDED
#define MOVE_H_INCLUDED

#include <time.h>

#include "board.h"

//pour les parties , profondeur fixe ou chronométré
int max_depth;

//nombre de noeuds alpha beta et qs
U64 nodes_alpha;
U64 nodes_qs;

//FONCTIONS
int do_move(MOVE m);
void undo_move();
void fill_sq(int c, int sq, int p);
void clear_sq(int c, int sq, int p);
int king_atacked(int c);

#endif // MOVE_H_INCLUDED
