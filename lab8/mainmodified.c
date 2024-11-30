
//#include <unistd.h>
//#include <stdint.h>
//#include <stdio.h>
//#include <sys/ioctl.h>
//#include <linux/i2c-dev.h> // pour les définitions IOCTL
//#include <fcntl.h>
#include "mainmodified.h"
#include "interfaceVL6180xmod.h"

#define I2C_FICHIER "/dev/i2c-1" // fichier Linux représentant le BUS I2C #1
#define I2C_ADRESSE 0x29         // adresse du VL6180x

int main()
{
    uint8_t fdPortI2C;    // descripteur de fichier pour l'I2C
    float distance;   // variable pour stocker la distance mesurée

    // Initialisation du port I2C
    fdPortI2C = open(I2C_FICHIER, O_RDWR);
    if (fdPortI2C == -1)
    {
        printf("Erreur : Impossible d'ouvrir le port I2C\n");
        return -1;
    }

    // Initialisation du capteur VL6180x
    if (interfaceVL6810x_initialise(fdPortI2C) < 0)
    {
        printf("Erreur : Initialisation du VL6180x\n");
        close(fdPortI2C);
        return -1;
    }

    // Boucle principale pour lire et afficher la distance
    while (1)
    {
        if (interfaceVL6180x_litUneDistance(&distance) == 0)
        {
            printf("Distance mesurée : %.2f mm\n", distance);
        }
        else
        {
            printf("Erreur : Lecture de la distance échouée\n");
        }

        usleep(500000); // Attente de 1 seconde avant la prochaine lecture
    }

    close(fdPortI2C); // Fermeture du descripteur de fichier
    return 0;
}
