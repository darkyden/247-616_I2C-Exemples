//2022-08 processusFils.c
//Historique: 2022-11-03, Yves Roy, création.

#include "main.h"
#include "serviceTube.h"
#include "processusFils.h"

//définitions privées
#define PROCESSUSFILS_PERIODE_DE_LA_BASE_DE_TEMPS_EN_MS 1
#define PROCESSUSFILS_COMPTE_MAXIMAL_ENTRE_LES_LECTURES \
		(PROCESSUSFILS_TEMPS_ENTRE_LES_LECTURES_DE_DISTANCE_EN_MS / \
		 PROCESSUSFILS_PERIODE_DE_LA_BASE_DE_TEMPS_EN_MS)

//déclaration des fonctions privées
int processusFils_attendUneDemandeDeMesures(void);
int processusFils_afficheDesMesures(void);

//variables privées
int (*processusFils_execute)(void);
float processusFils_distance;
char processusFils_tamponPereVersFils[100];
char processusFils_tamponFilsVersPetitFils[100];
char processusFils_tamponPetitFilsVersFils[100];

//fonctions privées
int processusFils_attendUneDemandeDeMesures(void)
{
	if (processusFils_tamponPereVersFils[0] != MAIN_MESURE)
	{
		return 0;
	}
	processusFils_execute = processusFils_afficheDesMesures;
	sprintf(processusFils_tamponFilsVersPetitFils, "%c", MAIN_MESURE);
	serviceTube_ecrisDansUnTube(processusFils_tamponFilsVersPetitFils,
			sizeof(processusFils_tamponFilsVersPetitFils),
			main_tubeFilsVersPetitFils);
	return 0;
}

int processusFils_afficheDesMesures(void)
{
int retour;
	retour = serviceTube_lisDansUnTube(main_tubePetitFilsVersFils,
			processusFils_tamponPetitFilsVersFils, 
			sizeof(processusFils_tamponPetitFilsVersFils));
	if (retour < 0) { return -1;}
	if (retour > 0) 
	{
	    printf("%s\n", processusFils_tamponPetitFilsVersFils);
    	fflush(stdout);
		return 0;
	}
	return 0;
}

//variables publiques
//pas de variables publiques

//fonctions publiques
int processusFils_gere(void)
{
int retour;
	while (1)
	{
		retour = serviceTube_lisDansUnTube(main_tubePereVersFils,
				processusFils_tamponPereVersFils, 
				sizeof(processusFils_tamponPereVersFils));
		if (retour < 0) { return -1;}
		if (retour > 0) 
		{
			if (processusFils_tamponPereVersFils[0] == MAIN_QUITTE)
			{
				sprintf(processusFils_tamponFilsVersPetitFils, "%c", MAIN_QUITTE);
        		serviceTube_ecrisDansUnTube(processusFils_tamponFilsVersPetitFils,
                		sizeof(processusFils_tamponFilsVersPetitFils),
                		main_tubeFilsVersPetitFils);
				return 0;
			}
		}
		if (processusFils_execute() < 0) { return -1;}
	}
	return 0;
}

int processusFils_initialise(void)
{
	processusFils_execute = processusFils_attendUneDemandeDeMesures;
	return 0;
}
