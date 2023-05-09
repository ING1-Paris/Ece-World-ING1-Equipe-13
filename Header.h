//
// Created by comeb on 28/04/2023.
//
#ifndef PROJET_HEADER_H
#define PROJET_HEADER_H

/// ~~~~~~~~~~~~~~ Define ~~~~~~~~~~~~~~
#define LONG 800
#define LARG 600
#define NB_JOUEUR 2


/// ~~~~~~~~~~~~~~ Bibliothèques ~~~~~~~~~~~~~~
#include <stdio.h>
#include <allegro.h>


/// ~~~~~~~~~~~~~~ Structure ~~~~~~~~~~~~~~
/*typedef struct{
    int tickets;
    char nom[50];

    int performance_Canards;
    int performance_ParisHippiques;
    int performance_CrossyRoad;
}joueur;*/

typedef struct {
    int x;
    int y;
    int tickets;
    BITMAP* sprite;
}joueur;


/// ~~~~~~~~~~~~~~ Prototypes ~~~~~~~~~~~~~~
int choixJoueurs(joueur *tabJoueur);
void tank_trouble();
void crossy_road();





#endif //PROJET_HEADER_H
