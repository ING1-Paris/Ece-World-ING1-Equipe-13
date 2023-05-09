//
// Created by comeb on 03/05/2023.
//
#include "Header.h"

void crossy_road(){
    /// ############### BITMAPS
    //BITMAP chat1[];
    //BITMAP chat2[];
    BITMAP *tronc;
    BITMAP *riviere;
    BITMAP *herbe1;
    BITMAP *herbe2;
    BITMAP *route;
    BITMAP *pieces[6];

    ///load_bitmap();
    tronc =load_bitmap("CR_Tronc.bmp",NULL);
    if (!tronc){
        allegro_message("pas pu trouver tronc.bmp");
        exit(EXIT_FAILURE);}

    riviere =load_bitmap("CR_Riviere.bmp",NULL);
    if (!riviere){
        allegro_message("pas pu trouver CR_Riviere.bmp");
        exit(EXIT_FAILURE);}

    herbe1 =load_bitmap("CR_Herbe1.bmp",NULL);
    if (!herbe1){
        allegro_message("pas pu trouver CR_Riviere.bmp");
        exit(EXIT_FAILURE);}

    herbe2 =load_bitmap("CR_Herbe2.bmp",NULL);
    if (!herbe2){
        allegro_message("pas pu trouver CR_Riviere.bmp");
        exit(EXIT_FAILURE);}

    route =load_bitmap("CR_Route.bmp",NULL);
    if (!route){
        allegro_message("pas pu trouver CR_Riviere.bmp");
        exit(EXIT_FAILURE);}

    //pieces =load_bitmap("CR_Pieces.bmp", NULL);

    /// ############### VARIABLES
    int troncX1, troncY1, troncX2, troncY2; //La position des troncs
    int posX1, posY1, posX_J2, posY_J2; //La position des deux joueurs

    int scancode =0, touche =0; //Clavier
    int sens =0;

    while (!key[KEY_ESC]) {

        ///GESTION TOUCHES
        if(keypressed()){
            touche =readkey();
            scancode =touche>>8;

            if(scancode ==KEY_DOWN){ //BAS
                posY1+= 0;}
            if(scancode ==KEY_UP){ //HAUT
                posY1-= 0;}
            if(scancode ==KEY_LEFT){ //GAUCHE
                posX1-= 0;
                sens =1;}
            if(scancode ==KEY_RIGHT){ //DROITE
                posX1+= 0;
                sens =2;}
        }
    }
}