#include "main.h"


#define PIPE_PARENT_TO_CHILD "/tmp/pipe_p2c"
#define PIPE_CHILD_TO_GRANDCHILD "/tmp/pipe_c2g"
#define PIPE_GRANDCHILD_TO_CHILD "/tmp/pipe_g2c"

void cleanup() {
    unlink(PIPE_CHILD_TO_GRANDCHILD);
    unlink(PIPE_GRANDCHILD_TO_CHILD);
}

int Child_main(int argc, char *argv[]) {
    int pipe_p2c, pipe_c2g, pipe_g2c;
    char buffer[10];
    pid_t grandchild_pid;

    // Création des tuyaux
    mkfifo(PIPE_CHILD_TO_GRANDCHILD, 0666);
    mkfifo(PIPE_GRANDCHILD_TO_CHILD, 0666);
    atexit(cleanup);

    // Fork pour le processus petit-fils
    grandchild_pid = fork();
    if (grandchild_pid == 0) {
        execl("./grandchild", "grandchild", PIPE_CHILD_TO_GRANDCHILD, PIPE_GRANDCHILD_TO_CHILD, NULL);
        perror("Erreur dans exec du processus petit-fils");
        exit(1);
    }

    // Ouverture du tuyau pour recevoir les commandes du père
    pipe_p2c = open(argv[1], O_RDONLY | O_NONBLOCK);

    while (1) {
        if (read(pipe_p2c, buffer, sizeof(buffer)) > 0) {
            if (strncmp(buffer, "START", 5) == 0) {
                // Envoie une commande au petit-fils pour commencer les mesures
                pipe_c2g = open(PIPE_CHILD_TO_GRANDCHILD, O_WRONLY | O_NONBLOCK);
                write(pipe_c2g, "START\n", 6);
                close(pipe_c2g);
            } else if (strncmp(buffer, "STOP", 4) == 0) {
                // Envoie une commande au petit-fils pour arrêter
                pipe_c2g = open(PIPE_CHILD_TO_GRANDCHILD, O_WRONLY | O_NONBLOCK);
                write(pipe_c2g, "STOP\n", 5);
                close(pipe_c2g);

                kill(grandchild_pid, SIGTERM);
                wait(NULL); // Attendre la fin du petit-fils
                break;
            }
        }

        // Lecture des distances depuis le petit-fils
        pipe_g2c = open(PIPE_GRANDCHILD_TO_CHILD, O_RDONLY | O_NONBLOCK);
        if (read(pipe_g2c, buffer, sizeof(buffer)) > 0) {
            printf("Distance mesurée : %s\n", buffer);
        }
        close(pipe_g2c);

        usleep(100000); // Pause de 100 ms
    }

    close(pipe_p2c);
    return 0;
}
