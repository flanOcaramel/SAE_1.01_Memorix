#include <stdlib.h>
#include <time.h>

#include "../include/jeu.h"

int main(void)
{
    /* Sans srand, rand() donnerait toujours la meme partie. */
    srand((unsigned)time(NULL));

    jouerPartie();

    return 0;
}
