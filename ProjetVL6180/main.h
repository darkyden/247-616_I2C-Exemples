#ifndef MAIN_H
#define MAIN_H
//2022-08 247-616 Solution Semaine 9:
//Historique: 2022-11-01, Yves Roy, création.

#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <sys/wait.h>
#include <sys/types.h>


//definitions globales
#define MAIN_QUITTE 'Q'
#define MAIN_MESURE 'M'
//pas de definitions globales

//dependances materielles
//pas de dependances materielles

//dependances logicielles
#define PROCESSUSPETITFILS_TEMPS_ENTRE_LES_LECTURES_DE_DISTANCE_EN_MS 100

//declaration des fonctions globales:
int main(void);

//declaration des variables globales
extern int main_tubePereVersFils[2];
extern int main_tubeFilsVersPetitFils[2];
extern int main_tubePetitFilsVersFils[2];

extern FILE* main_fichierPereVersFils;
extern FILE* main_fichierFilsVersPetitFils;
extern FILE* main_fichierPetitFilsVersFils;

#endif

