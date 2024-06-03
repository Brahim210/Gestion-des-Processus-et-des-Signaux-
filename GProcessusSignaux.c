#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <semaphore.h>
#include <fcntl.h>

#define NUM_CHILDREN 4

sem_t *sem_start;
pid_t children[NUM_CHILDREN];

void handle_signal(int sig) {
    if (sig == SIGUSR1) {
        printf("Child %d received SIGUSR1, starting task...\n", getpid());
        // Simuler une tâche par une boucle et un sleep
        for (int i = 0; i < 5; i++) {
            sleep(1);
            printf("Child %d working...\n", getpid());
        }
        // Envoyer le signal de confirmation au père
        kill(getppid(), SIGUSR2);
        exit(0);
    }
}

void parent_signal_handler(int sig) {
    if (sig == SIGUSR2) {
        printf("Parent received confirmation from a child.\n");
    }
}

int main() {
    // Initialiser le sémaphore POSIX
    sem_start = sem_open("/sem_start", O_CREAT, 0644, 0);
    if (sem_start == SEM_FAILED) {
        perror("sem_open");
        exit(EXIT_FAILURE);
    }

    signal(SIGUSR2, parent_signal_handler);

    // Créer les processus fils
    for (int i = 0; i < NUM_CHILDREN; i++) {
        pid_t pid = fork();
        if (pid == 0) {
            // Fils
            signal(SIGUSR1, handle_signal);
            sem_wait(sem_start); // Attendre le signal de départ
            pause(); // Attendre un signal pour commencer la tâche
        } else if (pid > 0) {
            // Père
            children[i] = pid;
        } else {
            perror("fork");
            exit(EXIT_FAILURE);
        }
    }

    // Synchronisation : Tous les fils attendent le signal de départ
    sleep(2); // Simuler une préparation
    for (int i = 0; i < NUM_CHILDREN; i++) {
        sem_post(sem_start); // Lancer les fils
    }

    // Envoyer un signal à chaque fils pour commencer la tâche
    for (int i = 0; i < NUM_CHILDREN; i++) {
        kill(children[i], SIGUSR1);
    }

    // Attendre que tous les fils terminent
    for (int i = 0; i < NUM_CHILDREN; i++) {
        wait(NULL);
    }

    // Fermer et supprimer le sémaphore
    sem_close(sem_start);
    sem_unlink("/sem_start");

    return 0;
}
