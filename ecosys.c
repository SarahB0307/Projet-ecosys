#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "ecosys.h"

/* Parametres globaux de l'ecosysteme (externes dans le ecosys.h)*/
float p_ch_dir=0.4;
float p_reproduce_proie=0.2;
float p_reproduce_predateur=0.25;
int temps_repousse_herbe=-12;

/* PARTIE 1*/
/* Fourni: Part 1, exercice 4, question 2 */
Animal *creer_animal(int x, int y, float energie) {
	assert(x >= 0 && x < SIZE_X && y >= 0 && y < SIZE_Y);
	Animal *na = (Animal *)malloc(sizeof(Animal));
	assert(na);
	na->x = x;
	na->y = y;
	na->energie = energie;
	na->dir[0] = rand() % 3 - 1;
	na->dir[1] = rand() % 3 - 1;
	na->suivant = NULL;
	return na;
}


/* Fourni: Part 1, exercice 4, question 3 */
Animal *ajouter_en_tete_animal(Animal *liste, Animal *animal) {
  assert(animal);
  assert(!animal->suivant);
  animal->suivant = liste;
  return animal;
}

/* A faire. Part 1, exercice 6, question 2 */
void ajouter_animal(int x, int y,  float energie, Animal **liste_animal) {
	Animal *na=creer_animal(x,y,energie);
	*liste_animal=ajouter_en_tete_animal(*liste_animal, na);
}

/* A Faire. Part 1, exercice 5, question 5 */
void enlever_animal(Animal **liste, Animal *animal) {
	assert(animal);
	Animal* temp=*liste; 
	Animal* a=NULL;
	if(temp==animal){
		*liste=(*liste)->suivant;
		free(temp);
	}
	else{
		a=(*liste)->suivant;
		while(a && a!=animal){	
			a=a->suivant;
			temp=temp->suivant;
		}
		if(a==animal){
			temp->suivant=a->suivant;
			temp=a;
			free(temp);
		}
	}	
	return ;
}

/* A Faire. Part 1, exercice 6, question 7 */
Animal* liberer_liste_animaux(Animal *liste) {
	Animal* p;
	while(liste){
		p=liste->suivant;
		free(liste);
		liste=p;
	}
	return NULL;
}

/* Fourni: part 1, exercice 4, question 4 */
unsigned int compte_animal_rec(Animal *la) {
  if (!la) return 0;
  return 1 + compte_animal_rec(la->suivant);
}

/* Fourni: part 1, exercice 4, question 4 */
unsigned int compte_animal_it(Animal *la) {
  int cpt=0;
  while (la) {
    ++cpt;
    la=la->suivant;
  }
  return cpt;
}



/* Part 1. Exercice 5, question 1, ATTENTION, ce code est susceptible de contenir des erreurs... */
void afficher_ecosys(Animal *liste_proie, Animal *liste_predateur) {
  unsigned int i, j;
  char ecosys[SIZE_X][SIZE_Y];
  Animal *pa=NULL;

  /* on initialise le tableau */
  for (i = 0; i < SIZE_X; ++i) {
    for (j = 0; j < SIZE_Y; ++j) {
      ecosys[i][j]=' ';
    }
  }

  /* on ajoute les proies */
  pa = liste_proie;
  while (pa) {
  	assert(pa->x >= 0 && pa->x < SIZE_X && pa->y >= 0 && pa->y < SIZE_Y);
  	ecosys[pa->x][pa->y] = '*';
    pa=pa->suivant;
  }

  /* on ajoute les predateurs */
  pa = liste_predateur;
  while (pa) {
  	  assert(pa->x >= 0 && pa->x < SIZE_X && pa->y >= 0 && pa->y < SIZE_Y);
      if ((ecosys[pa->x][pa->y] == '@') || (ecosys[pa->x][pa->y] == '*')) { /* proies aussi present */
        ecosys[pa->x][pa->y] = '@';
      } else {
        ecosys[pa->x][pa->y] = 'O';
      }
    pa = pa->suivant;
  }

  /* on affiche le tableau */
  printf("+");
  for (j = 0; j < SIZE_Y; ++j) {
    printf("-");
  }  
  printf("+\n");
  for (i = 0; i < SIZE_X; ++i) {
    printf("|");  /* le | du debut de la ligne */
    for (j = 0; j < SIZE_Y; ++j) {
      putchar(ecosys[i][j]);
    }
    printf("|\n"); /* le | de la fin de la ligne */
  }
  printf("+");
  for (j = 0; j<SIZE_Y; ++j) {
    printf("-");
  }
  printf("+\n");
  int nbproie=compte_animal_it(liste_proie);
  int nbpred=compte_animal_it(liste_predateur);
  
  printf("Nb proies : %5d\tNb predateurs : %5d\n", nbproie, nbpred);

}


void clear_screen() {
  printf("\x1b[2J\x1b[1;1H");  /* code ANSI X3.4 pour effacer l'ecran */
}

/* PARTIE 2*/

/* Part 2. Exercice 4, question 1 */
void bouger_animaux(Animal *la) {
	while(la){
		if((float)rand()/(float)RAND_MAX < p_ch_dir){
			la->dir[0]=rand()%3-1;
			la->dir[1]=rand()%3-1;
		}
		la->x=(la->x + la->dir[0] + SIZE_X)%SIZE_X;
		la->y=(la->y + la->dir[1] + SIZE_Y)%SIZE_Y;
		la=la->suivant;
	}
}

/* Part 2. Exercice 4, question 3 */
void reproduce(Animal **liste_animal, float p_reproduce) {
/* j'ai pas fait d'assert pour que je puisse l'utiliser pour une liste vide sans erreur */
	if(liste_animal){
		Animal *ani=*liste_animal;
		while(ani){
			if((float)rand()/(float)RAND_MAX < p_reproduce){
				ajouter_animal(ani->x, ani->y, (ani->energie)/2.0, liste_animal);
				ani->energie=(ani->energie)/2.0;
			}
			ani=ani->suivant;
		}
	}
}


/* Part 2. Exercice 6, question 1 */
void rafraichir_proies(Animal **liste_proie, int monde[SIZE_X][SIZE_Y]) {

	bouger_animaux(*liste_proie);

	Animal *a=*liste_proie;
	Animal *temp=NULL;
	while(a){
		a->energie-=1;
		if(a->energie < 0){
			temp=a;
			a=a->suivant;
			enlever_animal(liste_proie,temp);
			continue;
		}
		else if (monde[a->x][a->y] > 0){
  			a->energie += monde[a->x][a->y];
  			monde[a->x][a->y] = temps_repousse_herbe;
        }
		a=a->suivant;
	}
	reproduce(liste_proie,p_reproduce_proie);
}

/* Part 2. Exercice 7, question 1 */
Animal *animal_en_XY(Animal *l, int x, int y) {
	assert(x >= 0 && x < SIZE_X && y >= 0 && y < SIZE_Y);
	while(l){
		if(l->x == x && l->y == y)
			return l;
		l=l->suivant;	
	}
	return NULL;
} 

/* Part 2. Exercice 7, question 2 */
void rafraichir_predateurs(Animal **liste_predateur, Animal **liste_proie) {

	bouger_animaux(*liste_predateur);

	Animal *a=*liste_predateur;
	Animal *temp=NULL;
	Animal *proie=NULL;
	while(a){
		a->energie-=1;
		if(a->energie < 0){
			temp=a;
			a=a->suivant;
			enlever_animal(liste_predateur,temp);
			continue;
		}
		else{
			proie=animal_en_XY(*liste_proie,a->x,a->y);
			if(proie!=NULL){
				a->energie+=proie->energie;
				enlever_animal(liste_proie,proie);
			}
		}
		a=a->suivant;
	}
	reproduce(liste_predateur,p_reproduce_predateur);
}

/* Part 2. Exercice 5, question 2 */
void rafraichir_monde(int monde[SIZE_X][SIZE_Y]){
	int i,j;
	for(i=0;i<SIZE_X;i++){
		for(j=0;j<SIZE_Y;j++){
			monde[i][j]++;
		}
	}
}

