//2022-08 247-616 Solution Semaine 9:
//Historique: 2022-11-01, Yves Roy, création.

#include "main.h"
#include "piloteI2C1.h"
#include "serviceTube.h"
#include "interfaceTouche.h"
#include "interfaceVL6180x.h"
#include "processusPere.h"
#include "processusFils.h"
#include "processusPetitFils.h"

//definitions
//pas de définitions

//déclaration des fonctions privées
int main_initialise(void);

//définitions des variables privées
//pas de variables privées

//définitions des fonctions privées
int main_initialise(void)
{
	if (piloteI2C1_initialise() < 0)
	{
		printf("erreur: main_initialise piloteI2C1\n");
		return -1;
	}
	if (serviceTube_initialise() < 0)
	{
		printf("erreur: main_initialise servicePeriodique\n");
	}
	if (interfaceTouche_initialise() < 0)
	{
		printf("erreur: main_initialise interfaceTouche\n");
	}
	if (interfaceVL6810x_initialise() < 0)
	{
		printf("erreur: main_initialise interfaceVL6180x\n");
		return -1;
	}
	if (processusPere_initialise() < 0)
	{
		printf("erreur: main_initialise processusPere\n");
		return -1;
	}	
	if (processusFils_initialise() < 0)
	{
		printf("erreur: main_initialise processusFils\n");
		return -1;
	}
	if (processusPetitFils_initialise() < 0)
	{
		printf("erreur: main_initialise processusPetitFils\n");
		return -1;
	}	
	return 0;
}

//définitions de variables globales
int main_tubePereVersFils[2];
int main_tubeFilsVersPetitFils[2];
int main_tubePetitFilsVersFils[2];

//definitions des fonctions globales
int main(void)
{
  pid_t pid;
	main_initialise();
  if (pipe(main_tubePereVersFils) < 0) { return -1;}
  if (pipe(main_tubeFilsVersPetitFils) < 0) { return -1;}
  if (pipe(main_tubePetitFilsVersFils) < 0) { return -1;}
  pid = fork();
  if (pid < (pid_t)0) { return -1;}
  if (pid == (pid_t)0) //fils
  {
  	pid = fork();
    if (pid < (pid_t)0) { return -1;}
    if (pid == (pid_t)0) //petit fils
    { //la gestion d'erreur est minimaliste et des sorties sans fermer tout ce qui a été ouvert sont possibles
    	if (serviceTube_preparePourLectureSansBloquer(main_tubeFilsVersPetitFils) < 0) { return -1;}
    	if (serviceTube_preparePourEcriture(main_tubePetitFilsVersFils) < 0) { return -1;}
    	if (processusPetitFils_gere() < 0) { return -1;}
    	if (serviceTube_fermeUnTubeEnLecture(main_tubeFilsVersPetitFils) < 0) { return -1;}    	
    	if (serviceTube_fermeUnTubeEnEcriture(main_tubePetitFilsVersFils) < 0) { return -1;}
    }
    else //fils
    {
    	if (serviceTube_preparePourLectureSansBloquer(main_tubePereVersFils) < 0) { return -1;}
    	if (serviceTube_preparePourEcriture(main_tubeFilsVersPetitFils) < 0) { return -1;}
    	if (serviceTube_preparePourLectureSansBloquer(main_tubePetitFilsVersFils) < 0) { return -1;}
    	if (processusFils_gere() < 0) { return -1;}
    	wait(0);
    	if (serviceTube_fermeUnTubeEnLecture(main_tubePereVersFils) < 0) { return -1;}    	
    	if (serviceTube_fermeUnTubeEnEcriture(main_tubeFilsVersPetitFils) < 0) { return -1;}
    	if (serviceTube_fermeUnTubeEnLecture(main_tubePetitFilsVersFils) < 0) { return -1;}
    }
  }
  else //pere
  {
    if (serviceTube_preparePourEcriture(main_tubePereVersFils) < 0){return -1;}    
    if (processusPere_gere() < 0) { return -1;}
    wait(0);
    if (serviceTube_fermeUnTubeEnEcriture(main_tubePereVersFils) < 0) {return -1;}
    interfaceTouche_termine();
    piloteI2C1_fermeLeBus();
  }
  return 0;
}
