#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "bitboard.h"
#include "board.h"
#include "hash.h"
#include "engine.h"

void init_hash_table()
{
    int c, p, sq;
    int ctr = 0;

    memset(hash_table, 0, sizeof(hash_table));

    //code couleur/piece/case
    for(p = 1; p <= 12; ++p)
    {
        for(sq = 0; sq < 64; ++sq)
        {
            hash_table[p][sq] = codes_64_bits[ctr];
            ++ctr;
        }
    }


    //droit aux roques
    for(c = 0; c < 16; ++c)
    {
        droit_aux_roques[c] = codes_64_bits[ctr];
        ++ctr;
    }

    //codes cases en passant
    for(c = 0; c < 64; ++c)
    {
        val_en_passant[c] = codes_64_bits[ctr];
        ++ctr;
    }
    //codes couleurs
    val_couleur[BLANC] = codes_64_bits[ctr];
    ++ctr;
    val_couleur[NOIR]  = codes_64_bits[ctr];
    ++ctr;
}

U64 init_code_position()
{
    int c;
    U64 code = 0;

    for(c = 0; c < 64; ++c)
    {
        if(echiquier[c] != VIDE)
            code ^= hash_table[echiquier[c]][c];
    }
    code ^= droit_aux_roques[castle];
    if(ep != -1)
    {
		code ^= val_en_passant[ep];
    }
    code ^= val_couleur[side];
    return code;
}

int probe_hash(int depth, int alpha, int beta)
{
    HASHE *phashe = &tt[hash_position & 0x1FFFFF];

    positions_testees++;

    if(phashe->key == hash_position)
    {
        positions_trouvees++;
        if(phashe->depth >= depth)
        {
            if(phashe->flag == hashfEXACT)
                return phashe->value;
            if((phashe->flag == hashfALPHA) && (phashe->value <= alpha))
                return alpha;
            if((phashe->flag == hashfBETA) && (phashe->value >= beta))
                return beta;
        }
    }
    return MATE;
}


void save_hash(int depth, int val, int hashf, MOVE *pbest)
{
    HASHE *phashe = &tt[hash_position & 0x1FFFFF];

    positions_sauvees++;

    phashe->key     = hash_position;
    phashe->depth   = depth;
    phashe->flag    = hashf;
    phashe->value   = val;
    phashe->best    = pbest;
}

