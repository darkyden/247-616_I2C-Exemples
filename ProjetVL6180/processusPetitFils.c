#include "main.h"
#include "serviceTube.h"
#include "interfaceVL6180x.h"
#include "processusPetitFils.h"

//définitions privées
#define PROCESSUSPETITFILS_PERIODE_DE_LA_BASE_DE_TEMPS_EN_MS 1
#define PROCESSUSPETITFILS_TEMPS_DE_DEMARRAGE_DU_CAPTEUR_EN_MS 1
#define PROCESSUSPETITFILS_COMPTE_AVANT_D_UTILISER_LE_CAPTEUR \
		(PROCESSUSPETITFILS_TEMPS_DE_DEMARRAGE_DU_CAPTEUR_EN_MS / \
		 PROCESSUSPETITFILS_PERIODE_DE_LA_BASE_DE_TEMPS_EN_MS)
#define PROCESSUSPETITFILS_COMPTE_MAXIMAL_ENTRE_LES_LECTURES \
		(PROCESSUSPETITFILS_TEMPS_ENTRE_LES_LECTURES_DE_DISTANCE_EN_MS / \
		 PROCESSUSPETITFILS_PERIODE_DE_LA_BASE_DE_TEMPS_EN_MS)

//déclaration des fonctions privées
int processusPetitFils_attendApresLeCapteur(void);
int processusPetitFils_attendUneDemandeDeMesures(void);
int processusPetitFils_faitDesMesures(void);

//variables privées
int (*processusPetitFils_execute)(void);
float processusPetitFils_distance;
long processusPetitFils_compteurDeTemps;
char processusPetitFils_tamponFilsVersPetitFils[100];
char processusPetitFils_tamponPetitFilsVersFils[100];

//fonctions privées
int processusPetitFils_attendApresLeCapteur(void)
{
	processusPetitFils_compteurDeTemps++;
	if (processusPetitFils_compteurDeTemps < PROCESSUSPETITFILS_COMPTE_AVANT_D_UTILISER_LE_CAPTEUR)
	{
		return 0;
	}
	
	processusPetitFils_execute = processusPetitFils_attendUneDemandeDeMesures;	
	return 0;
}

int processusPetitFils_attendUneDemandeDeMesures(void)
{
	if (processusPetitFils_tamponFilsVersPetitFils[0] != MAIN_MESURE)
	{
		return 0;
	}
	processusPetitFils_compteurDeTemps = 0;
	processusPetitFils_execute = processusPetitFils_faitDesMesures;
	return 0;
}

int processusPetitFils_faitDesMesures(void)
{
	processusPetitFils_compteurDeTemps++;
	if (processusPetitFils_compteurDeTemps < PROCESSUSPETITFILS_COMPTE_MAXIMAL_ENTRE_LES_LECTURES)
	{
		return 0;
	}
	processusPetitFils_compteurDeTemps = 0;
	if (interfaceVL6180x_litUneDistance(&processusPetitFils_distance) < 0)
   	{
		printf("erreur: processusPetitFils interfaceVL6180x_litUneDistance");
        fflush(stdout);		
        return -1;
    }
    sprintf(processusPetitFils_tamponPetitFilsVersFils, "%2.1f", processusPetitFils_distance);
    serviceTube_ecrisDansUnTube(processusPetitFils_tamponPetitFilsVersFils,
    		sizeof(processusPetitFils_tamponPetitFilsVersFils),
    		main_tubePetitFilsVersFils);
	return 0;
}

//variables publiques
//pas de variables publiques

//fonctions publiques
int processusPetitFils_gere(void)
{
int retour;
	while (1)
	{
		usleep(PROCESSUSPETITFILS_PERIODE_DE_LA_BASE_DE_TEMPS_EN_MS * 1000);
		retour = serviceTube_lisDansUnTube(main_tubeFilsVersPetitFils,
				processusPetitFils_tamponFilsVersPetitFils, 
				sizeof(processusPetitFils_tamponFilsVersPetitFils));
		if (retour < 0) { return -1;}
		if (retour > 0) 
		{
			if (processusPetitFils_tamponFilsVersPetitFils[0] == MAIN_QUITTE)
			{ 
				return 0;
			}
		}
		if (processusPetitFils_execute() < 0) { return -1;}
	}
	return 0;
}

int processusPetitFils_initialise(void)
{
	processusPetitFils_compteurDeTemps = 0;
	processusPetitFils_execute = processusPetitFils_attendApresLeCapteur;
	return 0;
}
