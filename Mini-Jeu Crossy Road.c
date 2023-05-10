//
// Created by comeb on 03/05/2023.
//
#include "Header.h"

void crossy_road(){
    /// ############### BITMAPS
    //BITMAP chat1[];
    //BITMAP chat2[];
    BITMAP *riviere =load_bitmap("CR_Riviere.bmp",NULL);
    BITMAP *route =load_bitmap("CR_Route.bmp",NULL);
    BITMAP *perso = load_bitmap("CR_PersoTest.bmp", NULL);

    char nomfichier[256];
    BITMAP *tronc[3];
    for(int i =0; i<3; i++){
        sprintf(nomfichier,"CR_Tronc%d.bmp",i);
        // sprintf permet de faire un printf dans une chaine
        tronc[i] = load_bitmap(nomfichier,NULL);
        if (!tronc[i]){
            allegro_message("pas pu trouver %s",nomfichier);
            exit(EXIT_FAILURE);}
    }

    BITMAP *herbe[3];
    for(int i =0; i<3; i++){
        sprintf(nomfichier,"CR_Herbe%d.bmp",i+1);
        // sprintf permet de faire un printf dans une chaine
        herbe[i] = load_bitmap(nomfichier,NULL);
        if (!herbe[i]){
            allegro_message("pas pu trouver %s",nomfichier);
            exit(EXIT_FAILURE);}
    }

    BITMAP* coin_sheet = load_bitmap("CR_Piece.bmp", NULL);
    int coin_width = 54, coin_height = 51;
    int coin_x = 0, coin_y = 0;
    int num_sprites =6, coin_index = 0;
    BITMAP** pieces[6];
    while (coin_y < coin_sheet->h){
        BITMAP* sprite = create_bitmap(coin_width, coin_height); // Création d'un nouveau bitmap pour stocker le sprite
        blit(coin_sheet, sprite, coin_x, coin_y, 0, 0, coin_width, coin_height); // Copie du sprite à partir de la sprite sheet

        pieces[coin_index] = &sprite; // Stockage du bitmap du sprite dans le tableau
        coin_x += coin_width; // Déplacement des coordonnées pour passer au sprite suivant

        if (coin_x >= coin_sheet->w){
            coin_x = 0;
            coin_y += coin_height;}
        coin_index++; // Incrémentation de l'index du sprite dans le tableau
    }

    int decalage =500;
    BITMAP *buffer; //Double buffer
    buffer =create_bitmap(LONG,LARG+decalage);
    clear_bitmap(buffer);

    /// ############### VARIABLES
    //Troncs
    int troncX[4], troncY[4]; //La position des troncs
    int tabTypeTronc[4]; //Sa taille (100-150-200);
    for(int i=0; i<4; i++) {
        tabTypeTronc[i] =rand() % (3); //Random entre 0 et 2 soit les 3 tailles de tronc possible
    }
    int iter =0, vitesse =2;

    //Joueur
    int posX1 =605, posY1 =1100, posX_J2, posY_J2; //La position des deux joueurs
    int scancode =0, touche =0; //Clavier
    int sens =0; //Sens du joueur

    //Terrain
    int modifTerrain =0, boucle =0;

    /// ############### BOUCLE PRINCIPALE ###############
    while (!key[KEY_ESC]) {

        ///GESTION TOUCHES
        if(keypressed()){
            touche =readkey();
            scancode =touche>>8;

            if(scancode ==KEY_DOWN){ //BAS
                posY1+= 50;}
            if(scancode ==KEY_UP){ //HAUT
                posY1-= 50;}
            if(scancode ==KEY_LEFT){ //GAUCHE
                posX1-= 50;
                sens =1;}
            if(scancode ==KEY_RIGHT){ //DROITE
                posX1+= 50;
                sens =2;}
        }

        /// AFFICHAGE
        //Terrain
        draw_sprite(buffer, herbe[1], 300, LARG +decalage -1200); //Herbe très haute
        draw_sprite(buffer, riviere, 300, LARG +decalage -1050); //Rivière 2
        draw_sprite(buffer, herbe[2], 300, LARG +decalage -800); //Herbe haute
        draw_sprite(buffer, herbe[0], 300, LARG +decalage -550); //Herbe moyenne
        draw_sprite(buffer, route, 300, LARG +decalage -700 -15); //Route
        draw_sprite(buffer, riviere, 300, LARG +decalage -450); //Rivière
        draw_sprite(buffer, herbe[1], 300, LARG +decalage -150); //Herbe basse

        //Troncs
            /*Les troncs sont entre la ligne 5 et 8 soit 4 lignes -> On choisit aléatoirement la longueur du tronc par ligne
             * sachant que si 100 -> 3 troncs, si 150 -> 2 troncs, si 200 -> 1 seul
             * sachant que ligne 5 vitesse 2, l6 - 3, l7 - 1, l8 - 3 */
            //Lignes
        for(int i=0; i<4; i++){
            if(troncX[i] >950){
                troncX[i] =300;}
            else if(iter ==5){
                if(i ==0)vitesse =2;
                else if(i ==1)vitesse =3;
                else if(i ==2)vitesse =1;
                else if(i ==3)vitesse =3;
                troncX[i] += vitesse;}
            troncY[i] =(LARG+decalage)-250-i*50;
            draw_sprite(buffer, tronc[tabTypeTronc[i]], troncX[i], troncY[i]);
        }
        if(iter ==5)iter =0;
        iter++;

        //Personnage
        draw_sprite(buffer, perso, posX1, posY1);

        //Grille
        for(int i=6; i <20; i++){ //De base 24 carrés moins 5 par cotés
            vline(buffer, i*50, 0, LARG+decalage, makecol(255,255,255));}
        for(int i=0; i <23; i++){
            hline(buffer, 0, i*50, LONG, makecol(255,255,255));}

        //Buffer
            //Décalage hors écran
        if(posY1 <=600 && boucle ==0){
            boucle =1;
            modifTerrain =decalage;
            for(int i=0; i<500; i++){
                blit(buffer,screen,0,decalage-i,0,0,LONG,LARG+decalage);}
        }
        if(posY1 >=650 && boucle ==1){
            boucle =0;
            modifTerrain =0;
            for(int i=0; i<500; i++){
                blit(buffer,screen,0,i,0,0,LONG,LARG+decalage);}
        }
        blit(buffer,screen,0,decalage-modifTerrain,0,0,LONG,LARG+decalage);
        clear_bitmap(buffer);
    }
}

///SHUFFLE
/*  for (int i = 0; i < taille-1; i++){ //SHUFFLE
        int nbrRandom = rand() % (taille-i) + i;
        int tampon = tabPossession[i];
        tabPossession[i] = tabPossession[nbrRandom];
        tabPossession[nbrRandom] = tampon;
    } //Le shuffle génère un nombre aléatoire et stocke la valeur i du tableau à cette nouvelle case
    //Ensuite on prend les X premières cartes du tableau et on les donne à un joueur...
*/