#include "CL_include"

/* Fonction qui permet de connecter un nouveau client au serveur 
 à l'aide de l'identifiant de messagerie msqid donnée en entrée 
 Renvoie 1 si la procédure de connexion est réussie jusqu'au bout, 0 sinon.*/
int ConnectClient(int msqid)
{
	dmsgbuf message;
	int id=getpid();

	// Le client s'identifie en émettant un message de type CONNECT contenant son PID.
	sprintf(message.txt, "%d", id);
	message.type = CONNECT;

    if (msgsnd(msqid,&message,strlen(message.txt)+1,0) <0){
		perror("msgsnd");
        printf("Client : Erreur %d EnvoiMessage : %s %ld\n",CLEerr,message.txt,message.type);
		return 0;
    }
    printf("Client : Envoi MESSAGE : %s %ld\n",message.txt,message.type);

	/* Message de connexion bien envoyé
	 Attente de la reception du message de connexion du serveur avec la Cle Client  */

	if (msgrcv(msqid,&message,L_MSG,id,0) <0)
	{
		printf("Client : Erreur %d ReceptionMessage : %s %ld\n",CLEerr,message.txt,message.type);
		return 0;
	}
    printf("Client : Reception MESSAGE avec cle client : %s %ld\n",message.txt,message.type);

	/* Reception du message de connexion du serveur 
	  Comparaison avec la clé client déjà en notre possession 
	  Dans le cas échéant, cela signifie que le serveur a déjà trop de clients connectés */	

	if (strcmp(message.txt, CleClient)==0){
		printf("Client : nb_cl < NCL_MAX , OK\n"); 
	}
	else {
		if(strcmp(message.txt, "")){printf("Client : erreur serveur saturé\n");}
		else{printf("Client : erreur INFOC, mauvais message.txt\n");}
		return 0;
	}

	/* Bonne reception du message de connexion du serveur avec la Cle Client
	 Envoi du message d'acquittement */
	sprintf(message.txt, "%d", id);
	message.type=ACK;
	if (msgsnd(msqid,&message,strlen(message.txt)+1,0) <0){
		printf("Client : ACK non envoyé \n");
		return 0;
	}
	else{
		printf("Client : ACK envoyé \n");
	}

	return 1;
}

/* Fonction qui permet de déconnecter le client du serveur 
  à l'aide de l'identifiant de messagerie msqid donnée en entrée 
  Renvoie 1 si la procédure de deconnection est réussie, 0 sinon */
int DeconnectClient(int msqid){
	dmsgbuf message;
	int id=getpid();
	sprintf(message.txt, "%d", id);
	message.type = DECONNECT;

	/*printf("erreur  :  %d", erreur);*/
	/* Envoi du message de connexion */

    if (msgsnd(msqid,&message,strlen(message.txt)+1,0) <0){
		perror("msgsnd");
        printf("Client : Erreur %d EnvoiMessage : %s %ld\n",CLEerr,message.txt,message.type);
		return 0;
    }
    printf("Client : Envoi MESSAGE : %s %ld\n",message.txt,message.type);
	return 1;
}
/* Fonction qui crée un nouveau couple lecteur-rédacteur avec un tube
   et lance les processus lecteur et rédacteur en parallèle avec fork. */
void *FirstCouple(void *data)
{
    entries_t *entre_couple = (entries_t *)data;

    /* Création de deux processus parallèles, un pour le lecteur, un autre pour le rédacteur */
    pid_t pid = fork();
    if (pid == -1)
    {
        perror("fork");
    }
    else if (pid == 0)
    {
        close(entre_couple->tube[1]);
        // Processus fils (rédacteur)
        Redacteur1(&entre_couple->tube[0], entre_couple->init);
        close(entre_couple->tube[0]);
        exit(EXIT_SUCCESS);
    }
    else
    {
        close(entre_couple->tube[0]);
        // Processus parent (lecteur)
        Lecteur1(&entre_couple->tube[1], entre_couple->init);
        close(entre_couple->tube[1]);
    }
    pthread_exit(NULL);
}

/* Fonction qui crée un nouveau couple lecteur-rédacteur avec un tube
   et lance les processus lecteur et rédacteur en parallèle avec fork. */
void *SecondCouple(void *data)
{
    entries_t *entre_couple = (entries_t *)data;

    /* Création de deux processus parallèles, un pour le lecteur, un autre pour le rédacteur */
    pid_t pid = fork();
    if (pid == -1)
    {
        perror("fork");
    }
    else if (pid == 0)
    {
        close(entre_couple->tube[1]);
        // Processus fils (rédacteur)
        Redacteur0(&entre_couple->tube[0], entre_couple->init);
        close(entre_couple->tube[0]);
        exit(EXIT_SUCCESS);
    }
    else
    {
        close(entre_couple->tube[0]);
        // Processus parent (lecteur)
        Lecteur0(&entre_couple->tube[1], entre_couple->init);
        close(entre_couple->tube[1]);
    }
    pthread_exit(NULL);
}

/* Fonction d'initialisation qui crée une structure init_t contenant
   l'identifiant de messagerie, le nombre d'itérations, et le sémaphore Mutex. */
init_t Initialisation(int Msqid, int nb_iteration)
{
    init_t init;
    init.Msqid = Msqid;
    init.nb_iteration = nb_iteration;

    // Création du sémaphore Mutex
    if ((init.Semid = CreationMutex()) == -1)
    {
        printf("\nSrv : ERREUR fork  : RelMesg %d\n", RelacheMessagerie(Msqid));
        exit(0);
    }

    // Initialisation du sémaphore Mutex à 1
    if (Init_Mutex(init.Semid, 1) == SEMerr)
    {
        printf("\nSrv : ERREUR fork  : RelMesg %d\n", RelacheMessagerie(Msqid));
        printf("Srv : Destruction Mutex %d\n", DestructionMutex(init.Semid));
        exit(0);
    }

    // Allocation du tampon
    AllocTampon(&init.Tptr);

    return init;
}

/* Fonction principale qui crée une messagerie, effectue la connexion au serveur,
   initialise deux tubes pour les couples lecteur-rédacteur, lance les deux couples en parallèle,
   puis effectue la déconnexion du serveur. */
int Moniteurs(int nb_iteration)
{
	// Récupération de l'identifiant de messagerie
    int Msqid;
    Msqid = CreationMessagerie();
    if (Msqid < 0)
    {
        exit(0);
    }

	// Connexion au serveur 
    if (ConnectClient(Msqid))
    {
        printf("Client : Connexion OK\n");
    }
    else
    {
        printf("Client : Connexion échouée\n");
    }

	// Création des tubes pour la communication entre processus
    int tube1[2], tube0[2];
    if (pipe(tube1) < 0 || pipe(tube0) < 0)
    {
        return EXIT_FAILURE;
    }

	/* Création et initialisation de la variable init commune à tous les threads.
	La struture init_t a été crée pour contenir:
	- le nombre d'iteration voulu par l'utilisateur,
	- l'identifiant de la messagerie avec le serveur, 
	- l'identifiant du sémaphore qui permet de limiter l'accès à des zones critiques par exclusion mutuelle, 
	- l'adresse du buffer dans lequel écrit le serveur,
	- le tube créé pour la communication entre le processus lecteur et le processus rédacteur */
    init_t init = Initialisation(Msqid, nb_iteration);

    // Création des threads pour les deux couples lecteur-rédacteur
    pthread_t firstcoupleThread, secondcoupleThread;

	/* La structure entries_t a été créée pour être passée en entrée lors de la création des thread de couple
	Elle contient ainsi :
	- un champ init qui sera commun à tous les threads
	- le tube créé pour la communication entre le processus lecteur et le processus rédacteur */
    entries_t firstcoupleEntries, secondcoupleEntries;
    firstcoupleEntries.init = init;
    firstcoupleEntries.tube = tube0;
    secondcoupleEntries.init = init;
    secondcoupleEntries.tube = tube1;

    pthread_create(&firstcoupleThread, NULL, FirstCouple, (void *)&firstcoupleEntries);
    pthread_create(&secondcoupleThread, NULL, SecondCouple, (void *)&secondcoupleEntries);

    // Attente de la fin des threads
    pthread_join(firstcoupleThread, NULL);
    pthread_join(secondcoupleThread, NULL);

    // Déconnexion du serveur
    if (DeconnectClient(Msqid))
    {
        printf("Client : Déconnexion OK\n");
    }
    else
    {
        printf("Client : Déconnexion échouée\n");
    }

    return 1;
}
