//serviceTube
//Historique: 
//2022-10-06, Yves Roy, creation

//INCLUSIONS
#include <unistd.h>
#include "main.h"
#include "serviceTube.h"

//Definitions privees
//pas de definitions privees

//Declarations de fonctions privees:
//pas de fonctions privees

//Definitions de variables privees:


//Definitions de fonctions privees:
//pas de fonctions privees

//Definitions de variables publiques:
//pas de variables publiques

//Definitions de fonctions publiques:
int serviceTube_preparePourEcriture(int Tube[])
{
    if (close(Tube[SERVICETUBE_EN_LECTURE]) != 0)
    { 
        return -1;
    }
    return 0;
}

int serviceTube_fermeUnTubeEnEcriture(int Tube[])
{
    if (close(Tube[SERVICETUBE_EN_ECRITURE]) != 0)
    {
        return -1;
    }
    return 0;
}

int serviceTube_preparePourLecture(int Tube[])
{
    if (close(Tube[SERVICETUBE_EN_ECRITURE]) != 0)
    {
        return -1;
    }
    return 0;
    if (fcntl(Tube[SERVICETUBE_EN_LECTURE], F_SETFL, O_NONBLOCK) < 0)
    {
        return -1;
    }    
    
}

int serviceTube_preparePourLectureSansBloquer(int Tube[])
{
    if (close(Tube[SERVICETUBE_EN_ECRITURE]) != 0)
    {
        return -1;
    }
    if (fcntl(Tube[SERVICETUBE_EN_LECTURE], F_SETFL, O_NONBLOCK) < 0)
    {
        return -1;
    }
    return 0;
}

int serviceTube_fermeUnTubeEnLecture(int Tube[])
{
    if (close(Tube[SERVICETUBE_EN_ECRITURE]) != 0)
    {
        return -1;
    }
    return 0;
}


int serviceTube_lisDansUnTube(int Tube[], char* Tampon, unsigned int LongueurDuTampon)
{
    int nombreLu;
    nombreLu = read(Tube[SERVICETUBE_EN_LECTURE], Tampon, LongueurDuTampon);
    if (nombreLu == -1)
    {
        if (errno == EAGAIN)
        {
            return 0;
        }
        else 
        {
            return -1;
        }
    }
    if (nombreLu == 0)
    {
        return 0;
    }
    return nombreLu;
}

void serviceTube_ecrisDansUnTube(const char* Tampon, int LongueurDuTampon, int Tube[])
{
    write(Tube[SERVICETUBE_EN_ECRITURE], Tampon, LongueurDuTampon);
}

int serviceTube_initialise(void)
{
    return 0;
}
