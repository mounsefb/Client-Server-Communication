#include "CL_include"

/* Fonction du processus rédacteur voie 1 qui lit les données à partir du tube,
   les stocke dans un tableau temporaire et appelle la fonction Driver pour afficher le message. */
void Redacteur1(int *tube, init_t init)
{
    int i = 0, j = 0, tmp[5], x;

    while (j < init.nb_iteration)
    {
        if (read(*tube, &x, sizeof(int)))
        {
            tmp[i] = x; i++; j++;

            // Si le rédacteur a reçu 5 éléments du lecteur, appelle Driver pour afficher le message.
            if (i == 5)
            {
                Driver(tmp, 1);
                i = 0;
            }
        }
    }
}

/* Fonction du processus rédacteur voie 0 qui lit les données à partir du tube,
   les stocke dans un tableau temporaire et appelle la fonction Driver pour afficher le message. */
void Redacteur0(int *tube, init_t init)
{
    int i = 0, j = 0, tmp[5], x;
    while (j < init.nb_iteration)
    {
        if (read(*tube, &x, sizeof(int)))
        {
            tmp[i] = x; i++; j++;

            // Si le rédacteur a reçu 5 éléments du lecteur, appelle Driver pour afficher le message.
            if (i == 5)
            {
                Driver(tmp, 0);
                i = 0;
            }
        }
    }
}
