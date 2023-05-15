//
// Created by comeb on 03/05/2023.
//
#include "Header.h"
#define NBR_RONDIN 6

typedef struct chose{
    int troncX, troncY;
    int troncX2; //Si le type est 2 alors on affiche 2 troncs donc 2 coordonnées
    int taille, type; //10-150-200
    int vitesse;
}t_tronc;

typedef struct bla{
    int posX, posY, sens;
    int vitesse;
    int numero;
}t_joueur;


void crossy_road(){

    //██████╗ ██╗████████╗███╗   ███╗ █████╗ ██████╗ ███████╗
    //██╔══██╗██║╚══██╔══╝████╗ ████║██╔══██╗██╔══██╗██╔════╝
    //██████╔╝██║   ██║   ██╔████╔██║███████║██████╔╝███████╗
    //██╔══██╗██║   ██║   ██║╚██╔╝██║██╔══██║██╔═══╝ ╚════██║
    //██████╔╝██║   ██║   ██║ ╚═╝ ██║██║  ██║██║     ███████║
    //╚═════╝ ╚═╝   ╚═╝   ╚═╝     ╚═╝╚═╝  ╚═╝╚═╝     ╚══════╝
    //BITMAP chat1[];
    //BITMAP chat2[];
    BITMAP *riviere =load_bitmap("CR_Riviere.bmp",NULL);
    BITMAP *route =load_bitmap("CR_Route.bmp",NULL);
    BITMAP *perso = load_bitmap("CR_PersoTest.bmp", NULL);
    BITMAP *finish =load_bitmap("CR_Finish.bmp",NULL);

    char nomfichier[256];
    BITMAP *tronc[3];
    for(int i =0; i<3; i++){
        sprintf(nomfichier,"CR_Tronc%d.bmp",i); // sprintf permet de faire un printf dans une chaine
        tronc[i] = load_bitmap(nomfichier,NULL);
        if (!tronc[i]){
            allegro_message("pas pu trouver %s",nomfichier);
            exit(EXIT_FAILURE);}
    }

    BITMAP *herbe[3];
    for(int i =0; i<3; i++){
        sprintf(nomfichier,"CR_Herbe%d.bmp",i+1); // sprintf permet de faire un printf dans une chaine
        herbe[i] = load_bitmap(nomfichier,NULL);
        if (!herbe[i]){
            allegro_message("pas pu trouver %s",nomfichier);
            exit(EXIT_FAILURE);}
    }

    BITMAP* coin_sheet = load_bitmap("CR_Piece.bmp", NULL);
    int coin_width = 54, coin_height = 51, coin_x = 0, coin_y = 0;
    int coin_index = 0;
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

    //██╗   ██╗ █████╗ ██████╗ ██╗ █████╗ ██████╗ ██╗     ███████╗███████╗
    //██║   ██║██╔══██╗██╔══██╗██║██╔══██╗██╔══██╗██║     ██╔════╝██╔════╝
    //██║   ██║███████║██████╔╝██║███████║██████╔╝██║     █████╗  ███████╗
    //╚██╗ ██╔╝██╔══██║██╔══██╗██║██╔══██║██╔══██╗██║     ██╔══╝  ╚════██║
    // ╚████╔╝ ██║  ██║██║  ██║██║██║  ██║██████╔╝███████╗███████╗███████║
    //  ╚═══╝  ╚═╝  ╚═╝╚═╝  ╚═╝╚═╝╚═╝  ╚═╝╚═════╝ ╚══════╝╚══════╝╚══════╝
    //Troncs
    t_tronc tabTronc[NBR_RONDIN];
    tabTronc[0].vitesse =3; tabTronc[1].vitesse =2; tabTronc[2].vitesse =2; tabTronc[3].vitesse =4, tabTronc[4].vitesse =4, tabTronc[5].vitesse =3;

    for(int i=0; i<NBR_RONDIN; i++) {
        tabTronc[i].type =rand() % (3); //Random entre 0 et 2 soit les 3 tailles de tronc possible
        tabTronc[i].taille =100 + tabTronc[i].type*50; //Taille du tronc
    }
    for(int i=0; i <NBR_RONDIN; i +=2){ //Position initiale troncs pairs à gauche
        tabTronc[i].troncX =0;
        if(tabTronc[i].type ==2)
            tabTronc[i].troncX2 = tabTronc[i].troncX -300;
    }
    for(int i=1; i <NBR_RONDIN; i +=2){ //Position initiale troncs impairs à droite
        tabTronc[i].troncX =LONG -150;
        if(tabTronc[i].type ==2)
            tabTronc[i].troncX2 = tabTronc[i].troncX +300;
    }


    //Temps
    double tempsPasse = 0;
    double intervalle = 0.008; //Intervalle en secondes

    //Joueur
    t_joueur tabJoueur[2];
    tabJoueur[0].posX =LONG/2+5; tabJoueur[0].posY =LARG +decalage-50;
    tabJoueur[1].posX =LONG/2; tabJoueur[1].posY =LARG +decalage-50;
    int JoueurEnCours =0;
    int scancode =0, touche =0; //Clavier

    //Terrain
    int modifTerrain =0, boucle =0;

// ██████╗  ██████╗ ██╗   ██╗ ██████╗██╗     ███████╗    ██████╗ ██████╗ ██╗███╗   ██╗ ██████╗██╗██████╗  █████╗ ██╗     ███████╗
// ██╔══██╗██╔═══██╗██║   ██║██╔════╝██║     ██╔════╝    ██╔══██╗██╔══██╗██║████╗  ██║██╔════╝██║██╔══██╗██╔══██╗██║     ██╔════╝
// ██████╔╝██║   ██║██║   ██║██║     ██║     █████╗      ██████╔╝██████╔╝██║██╔██╗ ██║██║     ██║██████╔╝███████║██║     █████╗
// ██╔══██╗██║   ██║██║   ██║██║     ██║     ██╔══╝      ██╔═══╝ ██╔══██╗██║██║╚██╗██║██║     ██║██╔═══╝ ██╔══██║██║     ██╔══╝
// ██████╔╝╚██████╔╝╚██████╔╝╚██████╗███████╗███████╗    ██║     ██║  ██║██║██║ ╚████║╚██████╗██║██║     ██║  ██║███████╗███████╗
// ╚═════╝  ╚═════╝  ╚═════╝  ╚═════╝╚══════╝╚══════╝    ╚═╝     ╚═╝  ╚═╝╚═╝╚═╝  ╚═══╝ ╚═════╝╚═╝╚═╝     ╚═╝  ╚═╝╚══════╝╚══════╝
    while (!key[KEY_ESC]) {
        clock_t start_time = clock();

        ///GESTION TOUCHES
        if(keypressed()){
            touche =readkey();
            scancode =touche>>8;

            if(scancode ==KEY_DOWN){ //BAS
                tabJoueur[JoueurEnCours].posY+= 50;}
            if(scancode ==KEY_UP){ //HAUT
                tabJoueur[JoueurEnCours].posY-= 50;}
            if(scancode ==KEY_LEFT){ //GAUCHE
                tabJoueur[JoueurEnCours].posX-= 50;
                tabJoueur[JoueurEnCours].sens =1;}
            if(scancode ==KEY_RIGHT){ //DROITE
                tabJoueur[JoueurEnCours].posX+= 50;
                tabJoueur[JoueurEnCours].sens =2;}
        }

        ///############### AFFICHAGE TERRAIN ###############
        //Terrain
        draw_sprite(buffer, herbe[1], 300, LARG +decalage -1200); //Herbe très haute
        draw_sprite(buffer, riviere, 300, LARG +decalage -1050); //Rivière 2
        draw_sprite(buffer, herbe[2], 300, LARG +decalage -800); //Herbe haute
        draw_sprite(buffer, herbe[0], 300, LARG +decalage -550); //Herbe moyenne
        draw_sprite(buffer, route, 300, LARG +decalage -700 -15); //Route
        draw_sprite(buffer, riviere, 300, LARG +decalage -450); //Rivière
        draw_sprite(buffer, herbe[1], 300, LARG +decalage -150); //Herbe basse
        draw_sprite(buffer, finish, 550, 0);

        ///############### GESTION DES TRONCS ###############
            /*Les troncs sont entre la ligne 5 et 8 soit 4 lignes -> On choisit aléatoirement la longueur du tronc par ligne
             * sachant que si 100 -> 3 troncs, si 150 -> 2 troncs, si 200 -> 1 seul
             * sachant que ligne 5 vitesse 2, l6 - 3, l7 - 1, l8 - 3 */
        ///Deplacement des troncs
        clock_t end_time = clock();
        tempsPasse += (double) (end_time - start_time) / CLOCKS_PER_SEC;
        if (tempsPasse >= intervalle){ /// BOUCLE D'ANIMATION : Exécuter la boucle tous les X intervalles
            tempsPasse = 0;
            tabJoueur[JoueurEnCours].posX +=tabJoueur[JoueurEnCours].vitesse; //Déplacement du joueur
            //Gestion du déplacement des troncs
            for(int i=0; i<NBR_RONDIN; i+=2) {
                tabTronc[i].troncX += tabTronc[i].vitesse; // Mise à jour de la position de chaque rondin
                if(tabTronc[i].type ==2){tabTronc[i].troncX2 += tabTronc[i].vitesse;}
            }
            for(int i=1; i<NBR_RONDIN; i+=2) {
                tabTronc[i].troncX -= tabTronc[i].vitesse; // Mise à jour de la position de chaque rondin
                if (tabTronc[i].type == 2) {tabTronc[i].troncX2 -= tabTronc[i].vitesse;}
            }
        }
        // Blindage pour dépassement de l'écran
        for(int i=0; i<NBR_RONDIN; i +=2){ //Rondin de gauche à droite
            if(tabTronc[i].troncX >950){                          tabTronc[i].troncX =85 + tabTronc[i].type*50;}
            if(tabTronc[i].type ==2 && tabTronc[i].troncX2 >950){ tabTronc[i].troncX2 =85 + tabTronc[i].type*50;}
        }
        for(int i=1; i<NBR_RONDIN; i +=2) { //Rondin de droite à gauche
            if(tabTronc[i].troncX <85 + tabTronc[i].type*50){                         tabTronc[i].troncX =LONG -245;}
            if(tabTronc[i].type ==2 && tabTronc[i].troncX2 <85 + tabTronc[i].type*50){tabTronc[i].troncX2 =LONG -245;}
        }
        for(int i=0; i <NBR_RONDIN; i++){ //BOUCLE FINALE AFFICHAGE DES RONDINS
            tabTronc[i].troncY =(LARG+decalage)-200-i*50;
            draw_sprite(buffer, tronc[tabTronc[i].type], tabTronc[i].troncX, tabTronc[i].troncY); //Affichage du rondin
            if(tabTronc[i].type ==2){draw_sprite(buffer, tronc[tabTronc[i].type], tabTronc[i].troncX2, tabTronc[i].troncY);} //Affichage du rondin supp si type =2
        }
        //On veut détecter si le joueur est sur un rondin, sachant que les troncs sont de 3 longueurs différentes
        //Cette taille est stockée dans le tableau tabTypeTronc : 0 =200 pixels, 1 =150 pixels, 2 =100 pixels
        //Le joueur fait 50*50 pixels et il se déplace toujours de 50 pixels donc il bouge avec le rondin
        //qui est le plus dans son carré de 50*50 pixels
        ///Déplacement du joueur avec le rondin
        for(int i=0; i<NBR_RONDIN; i+=2){ //Boucle pour les rondins de gauche à droite
            //Si joueur même Y et rondin pas type 2
            if((tabJoueur[JoueurEnCours].posY/50) == (tabTronc[i].troncY/50)){
                if(tabTronc[i].type !=2 && (tabTronc[i].troncX <=tabJoueur[JoueurEnCours].posX && (tabTronc[i].troncX +tabTronc[i].taille) >=tabJoueur[JoueurEnCours].posX)){
                    tabJoueur[JoueurEnCours].vitesse = tabTronc[i].vitesse;}
                else{tabJoueur[JoueurEnCours].vitesse = 0;}
            }
        }
        for(int i=1; i<NBR_RONDIN; i+=2){ //Boucle pour les rondins de droite à gauche
            //Si joueur même Y et rondin pas type 2
            if((tabJoueur[JoueurEnCours].posY/50) == (tabTronc[i].troncY/50)) {
                if (tabTronc[i].type != 2 && (tabTronc[i].troncX / 50 <= tabJoueur[JoueurEnCours].posX / 50 && (tabTronc[i].troncX + tabTronc[i].taille) / 50 >=tabJoueur[JoueurEnCours].posX / 50)) {
                    tabJoueur[JoueurEnCours].vitesse = (-tabTronc[i].vitesse);
                } else { tabJoueur[JoueurEnCours].vitesse = 0; }
            }
        }
        //Blindage si joueur pas sur une ligne de rondin
        if(tabJoueur[JoueurEnCours].posY/50 >tabTronc[0].troncY/50  || tabJoueur[JoueurEnCours].posY/50 <tabTronc[NBR_RONDIN-1].troncY /50){
            tabJoueur[JoueurEnCours].vitesse = 0;}





        //Personnage
        draw_sprite(buffer, perso, tabJoueur[JoueurEnCours].posX, tabJoueur[JoueurEnCours].posY);

        //Grille
        for(int i=6; i <20; i++){ //De base 24 carrés moins 5 par cotés
            vline(buffer, i*50, 0, LARG+decalage, makecol(255,255,255));}
        for(int i=0; i <23; i++){
            hline(buffer, 0, i*50, LONG, makecol(255,255,255));}

        //Buffer
            //Décalage hors écran
        if(tabJoueur[JoueurEnCours].posY <=600 && boucle ==0){
            boucle =1;
            modifTerrain =decalage;
            for(int i=0; i<500; i++){
                blit(buffer,screen,0,decalage-i,0,0,LONG,LARG+decalage);}
        }
        if(tabJoueur[JoueurEnCours].posY >=650 && boucle ==1){
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