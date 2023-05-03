//
// Created by comeb on 03/05/2023.
//
#include "Header.h"

void crossy_road(){
    /// ############### BITMAPS
    BITMAP *poulet_rouge; //Joueur 1
    BITMAP *poulet_bleu; //Joueur 2
    BITMAP *tronc;
    BITMAP *fond;

    ///load_bitmap();
    poulet_rouge =load_bitmap("poulet_rouge.bmp",NULL);
    if (!poulet_rouge){
        allegro_message("pas pu trouver poulet.bmp");
        exit(EXIT_FAILURE);}

    poulet_bleu =load_bitmap("poulet_bleu.bmp",NULL);
    if (!poulet_bleu){
        allegro_message("pas pu trouver poulet_bleu.bmp");
        exit(EXIT_FAILURE);}

    tronc =load_bitmap("tronc.bmp",NULL);
    if (!tronc){
        allegro_message("pas pu trouver tronc.bmp");
        exit(EXIT_FAILURE);}

    fond =load_bitmap("fond.bmp",NULL);
    if (!fond){
        allegro_message("pas pu trouver fond.bmp");
        exit(EXIT_FAILURE);}

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