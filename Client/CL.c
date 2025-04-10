#include "CL_include"

int main(int argc, char *argv[])
{
    // Vérification du nombre d'arguments passés lors de l'exécution du programme
    if (argc < 2)
    {
        printf("Erreur dans le passage des parametres pour cl_zz\n");
        exit(0);
    }

    // Conversion de l'argument en entier pour obtenir le nombre d'itérations
    int nb_iteration = atoi(argv[1]);
    if (nb_iteration <= 0)
    {
        printf("Erreur dans la valeur du parametre de cl_zz\n");
        exit(0);
    }

    // Définition des gestionnaires de signaux pour les signaux SIGUSR1 et SIGUSR2
    signal(SIGUSR1, litbuf1);
    signal(SIGUSR2, litbuf2);

    printf("\n* * * * * * * * * * * * * * * * * * * * * * * * *\n");
    printf("Projet CLIENT - SERVEUR\tTemps R�el E2i - Novembre\n");
    printf("* * * * * * * * * * * * * * * * * * * * * * * * *\n");
    printf("\nLancement du client pendant %d secondes\n", nb_iteration);

    // Appel de la fonction Moniteurs avec le nombre d'itérations en tant que paramètre
    Moniteurs(nb_iteration);

    return 0;
}
