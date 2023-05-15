//
// Created by comeb on 28/04/2023.
//
#ifndef PROJET_HEADER_H
#define PROJET_HEADER_H

/// ~~~~~~~~~~~~~~ Define ~~~~~~~~~~~~~~
#define LONG 1200
#define LARG 650
#define NB_JOUEUR 2


/// ~~~~~~~~~~~~~~ Bibliothèques ~~~~~~~~~~~~~~
#include <stdio.h>
#include <allegro.h>
#include <time.h>

/// ~~~~~~~~~~~~~~ Structure ~~~~~~~~~~~~~~
/*typedef struct{
    int tickets;
    char nom[50];


}joueur;*/

typedef struct {
    int x;
    int y;
    int tickets;
    BITMAP* sprite;

    int performance_Canards;
    int performance_TirAuxBallons;
    int performance_CrossyRoad;
}joueur;


/// ~~~~~~~~~~~~~~ Prototypes ~~~~~~~~~~~~~~
int choixJoueurs(joueur *tabJoueur);
void tank_trouble();
void crossy_road();





#endif //PROJET_HEADER_H
