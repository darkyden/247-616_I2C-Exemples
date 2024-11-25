#include "main.h"

#define PIPE_PARENT_TO_CHILD "/tmp/pipe_p2c"

void cleanup() {
    unlink(PIPE_PARENT_TO_CHILD);
}

int main() {
    int pipe_fd;
    char command;
    pid_t child_pid;

    // Crée un tuyau non-bloquant
    mkfifo(PIPE_PARENT_TO_CHILD, 0666);
    atexit(cleanup);

    // Fork pour créer le processus fils
    child_pid = fork();
    if (child_pid == 0) {
        execl("./child", "child", PIPE_PARENT_TO_CHILD, NULL);
        perror("Erreur dans exec du processus fils");
        exit(1);
    }

    printf("Appuyez sur 'M' pour lancer les mesures, 'Q' pour quitter.\n");

    while (1) {
        command = getchar();
        if (command == 'M' || command == 'm') {
            // Envoie une commande au processus fils pour démarrer les mesures
            pipe_fd = open(PIPE_PARENT_TO_CHILD, O_WRONLY | O_NONBLOCK);
            write(pipe_fd, "START\n", 6);
            close(pipe_fd);
        } else if (command == 'Q' || command == 'q') {
            // Envoie une commande au processus fils pour arrêter
            pipe_fd = open(PIPE_PARENT_TO_CHILD, O_WRONLY | O_NONBLOCK);
            write(pipe_fd, "STOP\n", 5);
            close(pipe_fd);

            kill(child_pid, SIGTERM); // Termine le processus fils
            wait(NULL); // Attendre la fin du fils
            break;
        }
    }

    return 0;
}
