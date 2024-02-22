#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <assert.h>
#include <time.h>
#include <string.h>

#include "ecosys.h"

int main(void){
  	srand(time(NULL));
	int i;
	/* création de listes liste_proies et liste_predateurs */
	Animal* liste_proies=NULL;
	Animal* liste_predateurs=NULL;
	for(i=0;i<20;i++){
		ajouter_animal(rand()%SIZE_X, rand()%SIZE_Y, 10,&liste_proies);
		ajouter_animal(rand()%SIZE_X, rand()%SIZE_Y, 10,&liste_predateurs);
	}
	
	assert(compte_animal_rec(liste_proies)==20);
	assert(compte_animal_rec(liste_predateurs)==20);
	
	afficher_ecosys(liste_proies,liste_predateurs);
	
	/* test de la fonction liberer_liste_animaux
  liste_predateurs=liberer_liste_animaux(liste_predateurs);
 liste_proies=liberer_liste_animaux(liste_proies); 
 afficher_ecosys(liste_proies,liste_predateurs); vide avec que des espaces et 
 Nb proies :     0	Nb predateurs :     0
 
  on aura avec valgrind l'affichage : total heap usage: 41 allocs, 41 frees, 2,304 bytes allocated
 donc ça marche ! */
 	
 	// écriture sur le fichier nommé "ecosys"
 	
	FILE * f=fopen("ecosys","w");
	if(f==NULL){
		printf("Erreur dans l'ouverture du fichier\n");
		return 1;
	}
	
	fprintf(f,"<proies>\n");
	Animal *a=liste_proies;
	while(a){
		fprintf(f,"x=%d y=%d dir=[%d %d] e=%f\n",a->x,a->y,a->dir[0],a->dir[1],a->energie);
		a=a->suivant;
	}
	fprintf(f,"</proies>\n");
	
	fprintf(f,"<predateurs>\n");
	a=liste_predateurs;
	while(a){
		fprintf(f,"x=%d y=%d dir=[%d %d] e=%f\n",a->x,a->y,a->dir[0],a->dir[1],a->energie);
		a=a->suivant;
	}
	fprintf(f,"</predateurs>\n");
	
	fclose(f);
	
	// lecture depuis le fichier nommé "ecosys" avec création de listes 
	
	FILE *f2=fopen("ecosys","r");
	if(f2==NULL){
		printf("Erreur d'ouverture");
		return 1;
	}
	Animal *proies=NULL;
	Animal *predateurs=NULL;
	Animal *p=NULL;
	char buffer[200];
	int x,y,dir0,dir1;
	float energie;
	
	// création de la liste proies
	fgets(buffer,200,f2);
	p=proies;
	assert(strncmp(buffer,"<proies>",8)==0);
	fgets(buffer,200,f2);
	while(strncmp(buffer,"</proies>",9)!=0){
		sscanf(buffer,"x=%d y=%d dir=[%d %d] e=%f\n",&x,&y,&dir0,&dir1,&energie);
		Animal *an=creer_animal(x,y,energie);
		an->dir[0]=dir0;
		an->dir[1]=dir1;
		an->suivant=proies;
		proies=an;
		fgets(buffer,200,f2);
	}
	assert(strncmp(buffer,"</proies>",9)==0);
	fgets(buffer,200,f2);
	
	// création de la liste predateurs
	assert(strncmp(buffer,"<predateurs>",12)==0);
	p=predateurs;
	fgets(buffer,200,f2);
	while(strncmp(buffer,"</predateurs>",13)!=0){
		sscanf(buffer,"x=%d y=%d dir=[%d %d] e=%f\n",&x,&y,&dir0,&dir1,&energie);
		Animal *an=creer_animal(x,y,energie);
		an->dir[0]=dir0;
		an->dir[1]=dir1;
		an->suivant=predateurs;
		predateurs=an;
		fgets(buffer,200,f2);
	}
	assert(strncmp(buffer,"</predateurs>",13)==0);
	fclose(f2);
	
	afficher_ecosys(proies,predateurs);
	
	liberer_liste_animaux(liste_proies);
	liberer_liste_animaux(liste_predateurs);
	
	liberer_liste_animaux(proies);
	liberer_liste_animaux(predateurs);
	
	return 0;
}	
	
