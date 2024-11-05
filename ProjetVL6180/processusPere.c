//processusPere
//Historique: 
//2022-10-06, Yves Roy, creation

//INCLUSIONS
#include <stdio.h>
#include <errno.h>
#include <unistd.h>

#include "main.h"
#include "interfaceTouche.h"
#include "serviceTube.h"
#include "processusPere.h"

//Definitions privees
//pas de definitions privees

//Declarations de fonctions privees:
//pas de fonctions privees

//Definitions de variables privees:
char processusPere_toucheLue;
char processusPere_tamponPereVersFils[100];

//Definitions de fonctions privees:
//pas de fonctions privees

//Definitions de variables publiques:
//pas de variables publiques

//Definitions de fonctions publiques:
int processusPere_gere(void)
{
    fprintf(stdout,"Tapez:\n\r");
    fprintf(stdout, "%c: pour terminer.\n", MAIN_QUITTE);
    fprintf(stdout, "%c: pour mesurer.\n", MAIN_MESURE);
    fflush(stdout);

    while (processusPere_toucheLue != MAIN_QUITTE)
    {
        processusPere_toucheLue = interfaceTouche_lit();
        printf("processusPere: Caractère lu = '%c'\n", processusPere_toucheLue); fflush(stdout);
        switch (processusPere_toucheLue)
        {
            case MAIN_MESURE:
                sprintf(processusPere_tamponPereVersFils, "%c", MAIN_MESURE);
            break;
            case MAIN_QUITTE:
                sprintf(processusPere_tamponPereVersFils, "%c", MAIN_QUITTE);
            break;
        }
        serviceTube_ecrisDansUnTube(processusPere_tamponPereVersFils,
                sizeof(processusPere_tamponPereVersFils),
                main_tubePereVersFils);
    }
    return 0;
}

int processusPere_initialise(void)
{
    processusPere_toucheLue = '\0';
    return 0;
}
