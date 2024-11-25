#include "main.h"
#include "interfaceVL6180xmod.h"

#define I2C_FICHIER "/dev/i2c-1"
#define I2C_ADRESSE 0x29

int Grandchild_main(int argc, char *argv[]) {
    int pipe_c2g, pipe_g2c, fd_i2c;
    float distance;
    char command[10];

    // Initialisation du capteur
    fd_i2c = open(I2C_FICHIER, O_RDWR);
    if (fd_i2c < 0) {
        perror("Erreur d'ouverture du I2C");
        exit(1);
    }
    if (interfaceVL6810x_initialise(fd_i2c) < 0) {
        perror("Erreur d'initialisation du VL6180x");
        exit(1);
    }

    // Ouverture des tuyaux
    pipe_c2g = open(argv[1], O_RDONLY | O_NONBLOCK);
    pipe_g2c = open(argv[2], O_WRONLY | O_NONBLOCK);

    while (1) {
        if (read(pipe_c2g, command, sizeof(command)) > 0) {
            if (strncmp(command, "STOP", 4) == 0) {
                break;
            }
        }

        if (interfaceVL6180x_litUneDistance(&distance) == 0) {
            char buffer[10];
            snprintf(buffer, sizeof(buffer), "%.2f\n", distance);
            write(pipe_g2c, buffer, sizeof(buffer));
        }

        usleep(500000); // Pause de 500 ms
    }

    close(fd_i2c);
    return 0;
}
