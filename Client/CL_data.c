/* fichier CL_data.c */


#include "CL_include"

/* Fonction utile pour un lecteur qui permet de lire les données dans le tampon Tptr
 envoyées par le serveur sur la voie va et de les transferer directement
 au rédacteur à travers le tube donné en entrée. */
void LireData(int *tube, BUF *Tptr, int va)
{
    write(*tube, &(Tptr+va)->tampon[(Tptr+va)->n], sizeof(int));
}
