#include "CL_include"

int voie;  // Variable globale pour indiquer la voie

/* Fonction du processus lecteur voie 0 qui effectue la lecture de données à partir du tube,
   utilise les sémaphores pour la synchronisation et affiche les données lues. */
void Lecteur0(int *tube, init_t init)
{
    for (int i = 0; i < init.nb_iteration; i++)
    {
        sleep(1);

        // Vérifie la voie avant de procéder
        if (voie == 0)
        {
            // Utilise le sémaphore pour la section critique (lecture)
            P(init.Semid, voie);
            LireData(tube, init.Tptr, voie);
            V(init.Semid, voie);
        }
    }
}

/* Fonction du processus lecteur voie 1 qui effectue la lecture de données à partir du tube,
   utilise les sémaphores pour la synchronisation et affiche les données lues. */
void Lecteur1(int *tube, init_t init)
{
    for (int i = 0; i < init.nb_iteration; i++)
    {
        sleep(1);

        // Vérifie la voie avant de procéder
        if (voie == 1)
        {
            // Utilise le sémaphore pour la section critique (lecture)
            P(init.Semid, voie);
            LireData(tube, init.Tptr, voie);
            V(init.Semid, voie);
        }
    }
}

/*/////////////////////////////////////////////////////////////////////////*/
	   
void litbuf1() /* semaphore 0 */
{
	voie=0;
	signal(SIGUSR1,litbuf1);
}


/*/////////////////////////////////////////////////////////////////////////*/

void litbuf2()
{
	voie=1;	
	signal(SIGUSR2,litbuf2);
}
