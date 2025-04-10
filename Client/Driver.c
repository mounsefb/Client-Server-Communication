#include <stdio.h>

/* Fonction qui affiche le message d'un processus rédacteur en fonction de la voie.
   Le message est composé des éléments stockés dans le tableau temporaire. */
void Driver(int *tmp, int voie)
{
    // Vérifie la voie pour déterminer le type de processus rédacteur
    if (voie == 1) printf("Message du processus rédacteur voie 1 : ");
    else if (voie == 0) printf("Message du processus rédacteur voie 0 : ");
    else return;

    // Affiche les éléments du tableau temporaire
    for (int i = 0; i < 5; i++)
    {
        printf("%d", tmp[i]);
        if (i < 4) printf(", ");
    }
    puts("");
}
