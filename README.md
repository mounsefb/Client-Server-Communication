**Client-Serveur avec Moniteurs**

**Introduction**
Ce projet implémente un modèle client-serveur avec la communication entre les processus gérée par 
des moniteurs et des tubes. Le client envoie des messages au serveur, qui les reçoit et les 
traite selon une architecture de type lecteur-rédacteur.

**Structure du Code**
Le code est structuré en plusieurs fichiers pour une meilleure lisibilité et modularité dont les principales composantes sont :

*CL_def.h*: Header commun contenant les définitions et structures utiles aux implémenattions

*CL_include*: Header commun contenant les bibliothéques (pré-définies ou crées) utilisées dans les autres fichiers.

*Lecteur.c* et *Redacteur.c* : Implémentation des fonctions liées aux opérations de lecture et d'écriture des données.

*Driver.c* : Implémentation qui permet l'affichage dans le terminal des données reçu par le rédacteur

*Moniteurs.c* : Implémentation qui récupére une messagerie, effectue la connexion au serveur, initialise deux tubes pour les couples lecteur-rédacteur, lance les deux couples en parallèle, puis effectue la déconnexion du serveur.

**Compilation**
Le programme est compilé avec la commande make dans chacun des dossiers (Client ou Serveur).

Exemple de compilation :
    make
    *ou*
    make CL

**Utilisation**
Le programme prend en argument le nombre d'itérations à effectuer correspondant aux nombres de secondes d'activité. 

Exemple d'utilisation :
    ./CL 10

Le serveur envoie des messages aux clients, qui les traitent à l'aide de leur moniteur. Le nombre d'itérations spécifié détermine la durée d'exécution du client.

ATTENTION : l'appel au serveur et au client doit se faire dans 2 terminaux différents.

**Gestion des Erreurs**
Le programme effectue des vérifications sur les paramètres passés en ligne de commande, ainsi que sur les résultats des opérations système. En cas d'erreur, des messages explicites sont affichés, et le programme se termine proprement.
