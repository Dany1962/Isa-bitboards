#ifndef TIME_H
#define TIME_H

int debut;
int fin;
time_t    nb_secondes, tp,sec;
int temps_max;
int fin_recherche;
int mps , base , inc;
int blitz_ou_tournament;
int time_left;
int move_left;
/*
---------------------------------------------------------
*														*
*                       FONCTIONS                       *
*														*
---------------------------------------------------------
*/
int get_ms();
int controle_si_temps_depasse();
#endif // TIME_H
//fin time.h
