Remarques:
- Initialisation du sémaphore :
Utilisation de sem_open pour créer un sémaphore nommé /sem_start avec une valeur initiale de 0.
- Gestion des signaux :
Le père installe un gestionnaire pour SIGUSR2 pour recevoir des confirmations des fils.
Les fils installent un gestionnaire pour SIGUSR1 pour commencer leur tâche.
- Création des processus fils :
Utilisation de fork() pour créer quatre processus fils.
Les fils attendent le signal de départ via le sémaphore et ensuite un signal SIGUSR1 pour commencer leur tâche.
- Synchronisation :
Le père utilise sem_post pour permettre aux fils de commencer à attendre le signal SIGUSR1.
- Envoi de signaux :
Le père envoie un signal SIGUSR1 à chaque fils pour les faire commencer leur tâche.
- Confirmation des tâches :
Les fils envoient un signal SIGUSR2 au père après avoir terminé leur tâche.
Le père affiche un message lorsqu'il reçoit ce signal.
- Nettoyage :
Le père attend que tous les fils se terminent puis ferme et supprime le sémaphore.
