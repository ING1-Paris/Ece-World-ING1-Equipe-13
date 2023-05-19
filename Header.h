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
#include <stdlib.h>
#include <math.h>

/// ~~~~~~~~~~~~~~ Structure ~~~~~~~~~~~~~~
/*typedef struct{
    int tickets;
    char nom[50];


}joueur;*/

typedef struct Joueur {
    int x;
    int y;
    int tickets;
    char name[30];
    BITMAP* sprite;

    int performance_Canards;
    int performance_TirAuxBallons;
    int performance_CrossyRoad;
}joueur;


/// ~~~~~~~~~~~~~~ Prototypes ~~~~~~~~~~~~~~
int menu_map(joueur *tabJoueur);

void tank_trouble();

void crossy_road();
    void finPartie();

void jeu_canards();

int jeuMemory();


#endif //PROJET_HEADER_H
