#ifndef SERVICETUBE_H
#define SERVICETUBE_H

//MODULE: serviceTube
//DESCRIPTION: fonction pour gérer des "pipes"

//HISTORIQUE:
// 2022-10-06, Yves Roy, creation

//DEFINITIONS REQUISES PAR LE MODULE:
//Dependances materielles
//(copiez et adaptez ce qui suit dans "main.h")
//pas de dependances materielles

//Dependances logicielles
//(copiez et adaptez ce qui suit dans "main.h")
//pas de dependances logicielles

//INFORMATION PUBLIQUE:
//Definitions publiques:
#define SERVICETUBE_EN_LECTURE 0   
#define SERVICETUBE_EN_ECRITURE 1

//Fonctions publiques:
int serviceTube_preparePourEcriture(int Tube[]);
int serviceTube_fermeUnTubeEnEcriture(int Tube[]);
int serviceTube_preparePourLecture(int Tube[]);
int serviceTube_preparePourLectureSansBloquer(int Tube[]);
int serviceTube_fermeUnTubeEnLecture(int Tube[]);
int serviceTube_lisDansUnTube(int Tube[], char* Tampon, unsigned int LongueurDuTampon);
void serviceTube_ecrisDansUnTube(const char* Tampon, int LongueurDuTampon, int Tube[]);
int serviceTube_initialise(void);

//Variables publiques:
//pas de variables publiques

#endif

