#include <assert.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <strings.h>
#include "ecosys.h"



#define NB_PROIES 20
#define NB_PREDATEURS 20
#define T_WAIT 40000


int main(void) {
	srand(time(NULL));
	float energie_proies=5.0;
	float energie_pred=20.0;
	
	// une liste a d'un seul élement pour tester la fonction bouger_animeaux
	 /*
	Animal *a=creer_animal(2,2,10.0);
	a->dir[0]=1;
	a->dir[1]=1;
	*/
	
	// test de la fonction bouger_animaux 
	/* 
	afficher_ecosys(a,NULL);
	printf("x=%d y=%d\n",a->x,a->y);
	bouger_animaux(a);
	afficher_ecosys(a,NULL);
	printf("x=%d y=%d\n",a->x,a->y);
	*/
    	

	// test de la fonction reproduce 
	/*
	printf("Le nombre d'animeaux avant reproduce est : %d \n",compte_animal_it(a));
	reproduce(&a,1.0);
	printf("Le nombre d'animeaux apres 1 reproduce est : %d \n",compte_animal_it(a));
	reproduce(&a,1.0);
	printf("Le nombre d'animeaux apres 2 reproduce est : %d \n",compte_animal_it(a));
	*/
	
	/*l'affichage ne change pas apres les reproduce car les proies auront les mm coordonnées x et y, c'est que apres les avoir bouger que l'affichage change*/
	
	 
	// création de la liste des proies
	Animal *liste_proies=NULL;
	int i;
	for(i=0;i<NB_PROIES;i++){
		ajouter_animal(rand()/(float)(RAND_MAX/SIZE_X),rand()/(float)(RAND_MAX/SIZE_X),energie_proies,&liste_proies);
	}
	assert(compte_animal_it(liste_proies)==NB_PROIES);
	
	// test de la fonction rafraichir_proies
	/*
	int j=0;
	while(compte_animal_it(liste_proies)!=0 && j<200){
		rafraichir_proies(&liste_proies, monde);
		afficher_ecosys(liste_proies,NULL);
		usleep(T_WAIT);
		j++;
	}
	*/
	
	
	// test de la fonction animal_en_XY 
	/* 
	Animal *existe=animal_en_XY(a,1,2);
	existe!=NULL && printf("oui il existe \n") || printf("non il n'existe pas\n");
	*/
	
	// simulation de  l'ecosystème
	
	//initialiser toutes ses cases à 0
	int monde[SIZE_X][SIZE_Y]={0};  
	
	// création de la liste de prédateurs
	Animal *liste_predateurs=NULL;
	for(i=0;i<NB_PREDATEURS;i++){
		ajouter_animal(rand()/(float)(RAND_MAX/SIZE_X),rand()/(float)(RAND_MAX/SIZE_X),energie_pred,&liste_predateurs);
	}
	assert(compte_animal_it(liste_predateurs)==NB_PREDATEURS);
	
	
	// affichage + écriture sur le fichier nommé evol
	FILE* f=fopen("evol","w");
	int j=0,nb_pred=NB_PREDATEURS,nb_proies=NB_PROIES;
	while((nb_pred!=0 || nb_proies!=0) && j<500){
		fprintf(f,"%d %d %d\n",j,nb_proies,nb_pred);
		afficher_ecosys(liste_proies,liste_predateurs);
		rafraichir_predateurs(&liste_predateurs, &liste_proies);
		rafraichir_proies(&liste_proies, monde);
		rafraichir_monde(monde);
		usleep(T_WAIT);
		clear_screen();
		j++;
		nb_pred=compte_animal_it(liste_predateurs);
		nb_proies=compte_animal_it(liste_proies);
	}
	
	fclose(f);
	
	liberer_liste_animaux(liste_proies);
	liberer_liste_animaux(liste_predateurs);
	
  return 0;
}

