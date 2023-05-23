//
// Created by comeb on 03/05/2023.
//
#include "Header.h"
#define NBR_RONDIN 6
#define LARG 650

// ███████╗████████╗██████╗ ██╗   ██╗ ██████╗████████╗██╗   ██╗██████╗ ███████╗███████╗
// ██╔════╝╚══██╔══╝██╔══██╗██║   ██║██╔════╝╚══██╔══╝██║   ██║██╔══██╗██╔════╝██╔════╝
// ███████╗   ██║   ██████╔╝██║   ██║██║        ██║   ██║   ██║██████╔╝█████╗  ███████╗
// ╚════██║   ██║   ██╔══██╗██║   ██║██║        ██║   ██║   ██║██╔══██╗██╔══╝  ╚════██║
// ███████║   ██║   ██║  ██║╚██████╔╝╚██████╗   ██║   ╚██████╔╝██║  ██║███████╗███████║
// ╚══════╝   ╚═╝   ╚═╝  ╚═╝ ╚═════╝  ╚═════╝   ╚═╝    ╚═════╝ ╚═╝  ╚═╝╚══════╝╚══════╝

typedef struct Tronc_binoscope{
    int troncX, troncY;
    int troncX2; //Si le type est 2 alors on affiche 2 troncs donc 2 coordonnées
    int taille, type; //10-150-200
    int vitesse;
}t_tronc;

typedef struct JouJouHeure{
    int posX, posY, sens;
    int vitesse;
    int numero;
    double temps;
    int distance;
}t_joueur;

typedef struct JaimeLesTonks{
    int tonkX, tonkY;
    int vitesse;
}t_tonk;

///PROTOTYPES
void joueurSuivant(int *JoueurEnCours, t_joueur tableauJoueur[2], int decalage, int surTronc, t_tronc tabTronc[NBR_RONDIN]);
int finPartie(t_joueur tableauJoueur[2], BITMAP *perso[2], joueur *tabJoueurs);


void crossy_road(joueur *tabJoueur){

    ///Délai pour éviter superposition des touches
    for(int i=0; i<200; i++){
        if(keypressed()){
            int touche =readkey();
            rest(5);}
    }

    // ██████╗ ██╗████████╗███╗   ███╗ █████╗ ██████╗ ███████╗
    // ██╔══██╗██║╚══██╔══╝████╗ ████║██╔══██╗██╔══██╗██╔════╝
    // ██████╔╝██║   ██║   ██╔████╔██║███████║██████╔╝███████╗
    // ██╔══██╗██║   ██║   ██║╚██╔╝██║██╔══██║██╔═══╝ ╚════██║
    // ██████╔╝██║   ██║   ██║ ╚═╝ ██║██║  ██║██║     ███████║
    // ╚═════╝ ╚═╝   ╚═╝   ╚═╝     ╚═╝╚═╝  ╚═╝╚═╝     ╚══════╝
    //BITMAP chat1[];
    //BITMAP chat2[];
    BITMAP *riviere =load_bitmap("CR_Riviere.bmp",NULL);
    BITMAP *route =load_bitmap("CR_Route.bmp",NULL);
    BITMAP *finish =load_bitmap("CR_Finish.bmp",NULL);
    BITMAP *gameover =load_bitmap("CR_GameOver.bmp",NULL);
    BITMAP *victoire =load_bitmap("CR_Victoire.bmp",NULL);
    BITMAP *tonk = load_bitmap("CR_Tonk.bmp", NULL);
    BITMAP *buisson = load_bitmap("CR_Buisson.bmp", NULL);

    BITMAP *perso[2];
    perso[0]= load_bitmap("CR_Perso1.bmp", NULL);
    perso[1]= load_bitmap("CR_Perso2.bmp", NULL);

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
    BITMAP* pieces[6];
    while (coin_y < coin_sheet->h){
        BITMAP* sprite = create_bitmap(coin_width, coin_height); // Création d'un nouveau bitmap pour stocker le sprite
        blit(coin_sheet, sprite, coin_x, coin_y, 0, 0, coin_width, coin_height); // Copie du sprite à partir de la sprite sheet

        pieces[coin_index] = sprite; // Stockage du bitmap du sprite dans le tableau
        coin_x += coin_width; // Déplacement des coordonnées pour passer au sprite suivant

        if (coin_x >= coin_sheet->w){
            coin_x = 0;
            coin_y += coin_height;}
        coin_index++; // Incrémentation de l'index du sprite dans le tableau
    }

    int decalage =500;
    BITMAP *buffer; //Double buffer
    buffer =create_bitmap(largeur, hauteur + decalage);
    BITMAP *fondBuisson; fondBuisson =create_bitmap(largeur, hauteur + decalage);
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
        tabTronc[i].taille =200 - tabTronc[i].type*50; //Taille du tronc
        tabTronc[i].troncY = (hauteur + decalage) - 200 - i * 50; //Position en Y du tronc
    }
    for(int i=0; i <NBR_RONDIN; i +=2){ //Position initiale troncs pairs à gauche
        tabTronc[i].troncX =50;
        if(tabTronc[i].type ==2)
            tabTronc[i].troncX2 = tabTronc[i].troncX -300;
    }
    for(int i=1; i <NBR_RONDIN; i +=2){ //Position initiale troncs impairs à droite
        tabTronc[i].troncX = largeur - 150;
        if(tabTronc[i].type ==2)
            tabTronc[i].troncX2 = tabTronc[i].troncX +300;
    }
    int surTronc =1;

    //Tronc fond de map
    t_tronc TroncFondMap[2];
    TroncFondMap[0].troncX =50; TroncFondMap[0].troncY = 100;
    TroncFondMap[1].troncX =1050; TroncFondMap[1].troncY =150;
    TroncFondMap[0].vitesse =3; TroncFondMap[1].vitesse = -3;
    int surTroncFondMap =1;

    //Tonks
    t_tonk tabTonk[6];
    tabTonk[0].vitesse =4; tabTonk[1].vitesse =7; tabTonk[2].vitesse =5;
    tabTonk[3].vitesse =6; tabTonk[4].vitesse =4; tabTonk[5].vitesse =7;
    for(int i=0; i<3; i++){
        tabTonk[i].tonkX =200;
        tabTonk[i].tonkY = hauteur + decalage - 700 + 50 * i;}
    for(int i=0; i<3; i++){
        tabTonk[i+3].tonkX =170;
        tabTonk[i+3].tonkY = hauteur + decalage - 700 + 50 * i;}

    //Pieces
    int iterPieces =0;
    int attente =0;
    int piecesX =300; int pieces2X =850;
    int nbPieces =0;
    int ATTRAPE =0;

    //Temps
    double tempsPasse = 0;
    double intervalle = 0.008; //Intervalle en secondes
    double tempsMoins =0;

    //Joueur
    t_joueur tableauJoueur[2];
    tableauJoueur[0].posX = largeur / 2; tableauJoueur[0].posY = hauteur + decalage - 50;
    tableauJoueur[1].posX = largeur / 2; tableauJoueur[1].posY = hauteur + decalage - 50;
    int JoueurEnCours =0;
    int scancode =0, touche =0; //Clavier
    clock_t timeJoueur = clock(); //Temps pour la partie joueur
    for(int i=0; i<2; i++){tableauJoueur[i].temps =0;}

    //Terrain
    int modifTerrain =0, boucle =0;
    int tamponX, tamponY;

    int defaite =0; int finDePartie =0;

    // ██████╗ ██████╗ ███████╗      ███╗   ███╗███████╗███╗   ██╗██╗   ██╗
    // ██╔══██╗██╔══██╗██╔════╝      ████╗ ████║██╔════╝████╗  ██║██║   ██║
    // ██████╔╝██████╔╝█████╗ █████╗ ██╔████╔██║█████╗  ██╔██╗ ██║██║   ██║
    // ██╔═══╝ ██╔══██╗██╔══╝ ╚════╝ ██║╚██╔╝██║██╔══╝  ██║╚██╗██║██║   ██║
    // ██║     ██║  ██║███████╗      ██║ ╚═╝ ██║███████╗██║ ╚████║╚██████╔╝
    // ╚═╝     ╚═╝  ╚═╝╚══════╝      ╚═╝     ╚═╝╚══════╝╚═╝  ╚═══╝ ╚═════╝




// ██████╗  ██████╗ ██╗   ██╗ ██████╗██╗     ███████╗    ██████╗ ██████╗ ██╗███╗   ██╗ ██████╗██╗██████╗  █████╗ ██╗     ███████╗
// ██╔══██╗██╔═══██╗██║   ██║██╔════╝██║     ██╔════╝    ██╔══██╗██╔══██╗██║████╗  ██║██╔════╝██║██╔══██╗██╔══██╗██║     ██╔════╝
// ██████╔╝██║   ██║██║   ██║██║     ██║     █████╗      ██████╔╝██████╔╝██║██╔██╗ ██║██║     ██║██████╔╝███████║██║     █████╗
// ██╔══██╗██║   ██║██║   ██║██║     ██║     ██╔══╝      ██╔═══╝ ██╔══██╗██║██║╚██╗██║██║     ██║██╔═══╝ ██╔══██║██║     ██╔══╝
// ██████╔╝╚██████╔╝╚██████╔╝╚██████╗███████╗███████╗    ██║     ██║  ██║██║██║ ╚████║╚██████╗██║██║     ██║  ██║███████╗███████╗
// ╚═════╝  ╚═════╝  ╚═════╝  ╚═════╝╚══════╝╚══════╝    ╚═╝     ╚═╝  ╚═╝╚═╝╚═╝  ╚═══╝ ╚═════╝╚═╝╚═╝     ╚═╝  ╚═╝╚══════╝╚══════╝
    while (finDePartie ==0){
        clock_t start_time = clock();
        tamponX = tableauJoueur[JoueurEnCours].posX; tamponY = tableauJoueur[JoueurEnCours].posY;

        ///GESTION TOUCHES
        if(keypressed()){
            touche =readkey();
            scancode =touche>>8;

            if(scancode ==KEY_DOWN){ //BAS
                tableauJoueur[JoueurEnCours].posY+= 50;}
            if(scancode ==KEY_UP){ //HAUT
                tableauJoueur[JoueurEnCours].posY-= 50;}
            if(scancode ==KEY_LEFT){ //GAUCHE
                tableauJoueur[JoueurEnCours].posX-= 50;}
            if(scancode ==KEY_RIGHT){ //DROITE
                tableauJoueur[JoueurEnCours].posX+= 50;}
            rest(5);
        }

        ///############### AFFICHAGE TERRAIN ###############
        //Terrain
        draw_sprite(buffer, herbe[1], 300, hauteur + decalage - 1200); //Herbe très haute
        draw_sprite(buffer, riviere, 300, hauteur + decalage - 1050); //Rivière 2
        draw_sprite(buffer, herbe[2], 300, hauteur + decalage - 800); //Herbe haute
        draw_sprite(buffer, herbe[0], 300, hauteur + decalage - 550); //Herbe moyenne
        draw_sprite(buffer, route, 300, hauteur + decalage - 700 - 15); //Route
        draw_sprite(buffer, riviere, 300, hauteur + decalage - 450); //Rivière
        draw_sprite(buffer, herbe[1], 300, hauteur + decalage - 150); //Herbe basse
        draw_sprite(buffer, finish, 550, 0); //Ligne d'arrivée

        ///############### BOUCLE POUR TOUTES LES ANIMATIONS ###############
            /*Les troncs sont entre la ligne 5 et 8 soit 4 lignes -> On choisit aléatoirement la longueur du tronc par ligne
             * sachant que si 100 -> 3 troncs, si 150 -> 2 troncs, si 200 -> 1 seul
             * sachant que ligne 5 vitesse 2, l6 - 3, l7 - 1, l8 - 3 */
        ///Deplacement des troncs 1
        clock_t end_time = clock();
        tempsPasse += (double) (end_time - start_time) / CLOCKS_PER_SEC;
        if (tempsPasse >= intervalle){ /// BOUCLE D'ANIMATION : Exécuter la boucle tous les X intervalles
            tempsPasse = 0;
            tableauJoueur[JoueurEnCours].posX +=tableauJoueur[JoueurEnCours].vitesse; //Déplacement du joueur
            //Gestion du déplacement des troncs
            for(int i=0; i<NBR_RONDIN; i+=2) {
                tabTronc[i].troncX += tabTronc[i].vitesse; // Mise à jour de la position de chaque rondin
                if(tabTronc[i].type ==2){tabTronc[i].troncX2 += tabTronc[i].vitesse;}
            }
            for(int i=1; i<NBR_RONDIN; i+=2) {
                tabTronc[i].troncX -= tabTronc[i].vitesse; // Mise à jour de la position de chaque rondin
                if (tabTronc[i].type == 2) {tabTronc[i].troncX2 -= tabTronc[i].vitesse;}
            }

            ///Deplacement des TONKS
            for(int i=0; i <6; i ++){
                tabTonk[i].tonkX += tabTonk[i].vitesse; // Mise à jour de la position de chaque tonk
            }

            ///Deplacement Troncs Fond de map
            TroncFondMap[0].troncX +=3; TroncFondMap[1].troncX -=3;

            ///Pieces
            attente++;
            if(attente >=4){
                attente =0;
                iterPieces++;}
            if(iterPieces == 6){
                iterPieces =0;}
        }


        ///############### TRONCS RIVIERE 1 ###############
        // Blindage pour dépassement de l'écran
        for(int i=0; i<NBR_RONDIN; i +=2){ //Rondin de gauche à droite
            if(tabTronc[i].troncX >950){                          tabTronc[i].troncX =85 + tabTronc[i].type*50;}
            if(tabTronc[i].type ==2 && tabTronc[i].troncX2 >950){ tabTronc[i].troncX2 =85 + tabTronc[i].type*50;}
        }
        for(int i=1; i<NBR_RONDIN; i +=2) { //Rondin de droite à gauche
            if(tabTronc[i].troncX <85 + tabTronc[i].type*50){                         tabTronc[i].troncX = largeur - 245;}
            if(tabTronc[i].type ==2 && tabTronc[i].troncX2 <85 + tabTronc[i].type*50){tabTronc[i].troncX2 = largeur - 245;}
        }
        for(int i=0; i <NBR_RONDIN; i++){ //BOUCLE FINALE AFFICHAGE DES RONDINS
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
            if((tableauJoueur[JoueurEnCours].posY/50) == (tabTronc[i].troncY/50)){
                if(tabTronc[i].type !=2 && (tabTronc[i].troncX <=tableauJoueur[JoueurEnCours].posX+10 && (tabTronc[i].troncX +tabTronc[i].taille-45) >=tableauJoueur[JoueurEnCours].posX)){
                    tableauJoueur[JoueurEnCours].vitesse = tabTronc[i].vitesse; surTronc =1;}
                else if(tabTronc[i].type ==2 && (tabTronc[i].troncX <=tableauJoueur[JoueurEnCours].posX+7 && (tabTronc[i].troncX +tabTronc[i].taille-48) >=tableauJoueur[JoueurEnCours].posX)){
                    tableauJoueur[JoueurEnCours].vitesse = tabTronc[i].vitesse; surTronc =1;}
                else if(tabTronc[i].type ==2 && (tabTronc[i].troncX2 <=tableauJoueur[JoueurEnCours].posX+7 && (tabTronc[i].troncX2 +tabTronc[i].taille-48) >=tableauJoueur[JoueurEnCours].posX)){
                    tableauJoueur[JoueurEnCours].vitesse = tabTronc[i].vitesse; surTronc =1;}
                else{tableauJoueur[JoueurEnCours].vitesse = 0; surTronc =0;}
            }
        }
        for(int i=1; i<NBR_RONDIN; i+=2){ //Boucle pour les rondins de droite à gauche
            //Si joueur même Y et rondin pas type 2
            if((tableauJoueur[JoueurEnCours].posY/50) == (tabTronc[i].troncY/50)) {
                if(tabTronc[i].type !=2 && (tabTronc[i].troncX <=tableauJoueur[JoueurEnCours].posX+10 && (tabTronc[i].troncX + tabTronc[i].taille-45) >=tableauJoueur[JoueurEnCours].posX / 50)) {
                    tableauJoueur[JoueurEnCours].vitesse = (-tabTronc[i].vitesse); surTronc =1;}
                else if(tabTronc[i].type ==2 && (tabTronc[i].troncX <=tableauJoueur[JoueurEnCours].posX+7 && (tabTronc[i].troncX +tabTronc[i].taille-45) >=tableauJoueur[JoueurEnCours].posX)){
                    tableauJoueur[JoueurEnCours].vitesse = (-tabTronc[i].vitesse); surTronc =1;}
                else if(tabTronc[i].type ==2 && (tabTronc[i].troncX2 <=tableauJoueur[JoueurEnCours].posX+7 && (tabTronc[i].troncX2 +tabTronc[i].taille-45) >=tableauJoueur[JoueurEnCours].posX)){
                    tableauJoueur[JoueurEnCours].vitesse = (-tabTronc[i].vitesse); surTronc =1;}
                else { tableauJoueur[JoueurEnCours].vitesse = 0; surTronc =0;}
            }
        }
        //Blindage si joueur pas sur une ligne de rondin
        if(tableauJoueur[JoueurEnCours].posY/50 >tabTronc[0].troncY/50  || tableauJoueur[JoueurEnCours].posY/50 <tabTronc[NBR_RONDIN-1].troncY /50 ){
            if(tableauJoueur[JoueurEnCours].posY/50 >TroncFondMap[0].troncY/50  || tableauJoueur[JoueurEnCours].posY/50 <TroncFondMap[1].troncY /50){
                tableauJoueur[JoueurEnCours].vitesse = 0;}}
        //Blindage pour bien être dans un carré après les rondins
        if(tableauJoueur[JoueurEnCours].posY /50 ==20 || tableauJoueur[JoueurEnCours].posY /50 ==13 || tableauJoueur[JoueurEnCours].posY /50 ==4 || tableauJoueur[JoueurEnCours].posY /50 ==1){
            int tampon =tableauJoueur[JoueurEnCours].posX %50;
            if(tampon >25){tableauJoueur[JoueurEnCours].posX +=50-tampon;}
            else{tableauJoueur[JoueurEnCours].posX -=tampon;}
        }


        ///############### TONKS + ROUTE ###############
        for(int i=0;i <6; i++){ //Affichage des tonks
            draw_sprite(buffer, tonk, tabTonk[i].tonkX, tabTonk[i].tonkY);
            if(tabTonk[i].tonkX >950)
                tabTonk[i].tonkX =200;
        }


        ///############### PONT + RONDINS FOND DE MAP ###############
        //Pont
        rotate_sprite(buffer, tronc[1], hauteur / 2 + 180, 250, itofix(-64));
        rotate_sprite(buffer, tronc[1], hauteur / 2 + 230, 250, itofix(-64));
        rotate_sprite(buffer, tronc[1], hauteur / 2 + 280, 250, itofix(-64));

        //Troncs Fond de map
        for(int i =0; i<2; i++){ //Affichage
            draw_sprite(buffer, tronc[1], TroncFondMap[i].troncX, TroncFondMap[i].troncY);
        }
        if(TroncFondMap[0].troncX >950){ //Dépassement écran
            TroncFondMap[0].troncX =50;}
        if(TroncFondMap[1].troncX <150){ //Dépassement écran
            TroncFondMap[1].troncX =1050;}
        for(int i =0; i<2; i++) { //Déplacement
            if(tableauJoueur[JoueurEnCours].posY /50 == TroncFondMap[i].troncY /50){
                if(tableauJoueur[JoueurEnCours].posX >= TroncFondMap[i].troncX && tableauJoueur[JoueurEnCours].posX <= TroncFondMap[i].troncX +150){
                    tableauJoueur[JoueurEnCours].vitesse = TroncFondMap[i].vitesse;
                    surTroncFondMap =1;}
                else{
                    tableauJoueur[JoueurEnCours].vitesse = 0;
                    surTroncFondMap =0;}
            }
        } //Detection si joueur sur tronc du fond
        if(tableauJoueur[JoueurEnCours].posY/50 <=6 && tableauJoueur[JoueurEnCours].posY/50 >=4){
            if(tableauJoueur[JoueurEnCours].posX >= hauteur / 2 + 200 && tableauJoueur[JoueurEnCours].posX <= hauteur / 2 + 350){
                surTroncFondMap =1;
            }
            else{surTroncFondMap =0;}
        }

        ///############### BUISSONS ###############
        draw_sprite(buffer, buisson, 350, hauteur+decalage -150); //Première rangée
        draw_sprite(buffer, buisson, 350, hauteur+decalage -100);
        draw_sprite(buffer, buisson, 450, hauteur+decalage -100); //Deuxième rangée
        draw_sprite(buffer, buisson, 450, hauteur+decalage -50);
        if(tableauJoueur[JoueurEnCours].posY == hauteur+decalage -100 || tableauJoueur[JoueurEnCours].posY == hauteur+decalage -150){
            if(tableauJoueur[JoueurEnCours].posX >= 350 && tableauJoueur[JoueurEnCours].posX < 400){
                tableauJoueur[JoueurEnCours].posX =tamponX; tableauJoueur[JoueurEnCours].posY =tamponY;}}
        if(tableauJoueur[JoueurEnCours].posY == hauteur+decalage -100 || tableauJoueur[JoueurEnCours].posY == hauteur+decalage -50){
            if(tableauJoueur[JoueurEnCours].posX >= 450 && tableauJoueur[JoueurEnCours].posX < 500){
                tableauJoueur[JoueurEnCours].posX =tamponX; tableauJoueur[JoueurEnCours].posY =tamponY;}}

        draw_sprite(buffer, buisson, 800, 400); //2e zone
        draw_sprite(buffer, buisson, 850, 350);
        if(tableauJoueur[JoueurEnCours].posY == 400){
            if(tableauJoueur[JoueurEnCours].posX >= 800 && tableauJoueur[JoueurEnCours].posX < 850){
                tableauJoueur[JoueurEnCours].posX =tamponX; tableauJoueur[JoueurEnCours].posY =tamponY;}}
        if(tableauJoueur[JoueurEnCours].posY == 350){
            if(tableauJoueur[JoueurEnCours].posX >= 850 && tableauJoueur[JoueurEnCours].posX < 900){
                tableauJoueur[JoueurEnCours].posX =tamponX; tableauJoueur[JoueurEnCours].posY =tamponY;}}

        //3e zone autoroute
        int decalageX =0;
        for(int i=0; i<2; i++){
            for(int u=0; u<2; u++){
                if(tableauJoueur[JoueurEnCours].posY == 650){
                    if(tableauJoueur[JoueurEnCours].posX >= 300+decalageX && tableauJoueur[JoueurEnCours].posX < 350+decalageX){
                        tableauJoueur[JoueurEnCours].posX =tamponX; tableauJoueur[JoueurEnCours].posY =tamponY;}}
                draw_sprite(buffer, buisson, 300 +decalageX, 650);
                decalageX +=100;
            }
            decalageX =450;
        }

        ///############### PIECES ###############
        draw_sprite(buffer, pieces[iterPieces], piecesX, 1000);
        draw_sprite(buffer, pieces[iterPieces], pieces2X, 400);
        if(tableauJoueur[JoueurEnCours].posY == hauteur+decalage -150){
            if(tableauJoueur[JoueurEnCours].posX >= 300 && tableauJoueur[JoueurEnCours].posX < 350 && ATTRAPE ==0) {
                piecesX =0; nbPieces++; ATTRAPE =1;
                tempsMoins +=2.0;}
        }
        if(tableauJoueur[JoueurEnCours].posY == 400){
            if(tableauJoueur[JoueurEnCours].posX >= 850 && tableauJoueur[JoueurEnCours].posX < 900 && (ATTRAPE ==0 || ATTRAPE ==1)){
                pieces2X =0; nbPieces++; ATTRAPE =2;
                tempsMoins +=2.0;}
        }


        ///############### AFFICHAGE DU RESTE ###############
        //Personnage
        draw_sprite(buffer, perso[JoueurEnCours], tableauJoueur[JoueurEnCours].posX +5, tableauJoueur[JoueurEnCours].posY);

        //Grille
        /*for(int i=6; i <20; i++){ //De base 24 carrés moins 5 de chaque côtés
            vline(buffer, i*50, 0, hauteur+decalage, makecol(255,255,255));}
        for(int i=0; i <23; i++){
            hline(buffer, 0, i*50, largeur, makecol(255,255,255));}*/

        //Rectangle noirs pour masquer les troncs
        rectfill(buffer, 900, 0, largeur, hauteur +decalage, makecol(0, 0, 0));
        rectfill(buffer, 0, 0, 300, hauteur +decalage, makecol(0, 0, 0));

        //Nombre de pièces collectées
        draw_sprite(buffer, pieces[0], 10, decalage -modifTerrain);
        textprintf_ex(buffer, font, 70, 20 +decalage-modifTerrain, makecol(255, 255, 255), -1, ": %d", nbPieces);

        //Explications pieces et buisson
        draw_sprite(buffer, buisson, 50, 325+decalage);
        textprintf_ex(buffer, font, 20, 350 +decalage, makecol(255, 255, 255), -1, "Les");
        textprintf_ex(buffer, font, 20, 380 +decalage, makecol(255, 255, 255), -1, "sont des obstacles");

        draw_sprite(buffer, pieces[0], 70, 475+decalage);
        textprintf_ex(buffer, font, 20, 500 +decalage, makecol(255, 255, 255), -1, "Chaque");
        textprintf_ex(buffer, font, 125, 500 +decalage, makecol(255, 255, 255), -1, "retire 2s");
        textprintf_ex(buffer, font, 20, 530 +decalage, makecol(255, 255, 255), -1, "au temps final.");

        //Buffer
            //Décalage hors écran
        if(tableauJoueur[JoueurEnCours].posY <=600 && boucle ==0){
            boucle =1;
            modifTerrain =decalage;
            for(int i=0; i<500; i++){
                blit(buffer, screen, 0,decalage-i, 0, 0, largeur, hauteur + decalage);}
        }
        if(tableauJoueur[JoueurEnCours].posY >=650 && boucle ==1){
            boucle =0;
            modifTerrain =0;
            for(int i=0; i<500; i++){
                blit(buffer, screen, 0, i, 0, 0, largeur, hauteur + decalage);}
        }
        blit(buffer, screen, 0,decalage-modifTerrain, 0, 0, largeur, hauteur + decalage);
        clear_bitmap(buffer);


        ///Detection des collisions et des rivières
        if(((tableauJoueur[JoueurEnCours].posY /50) <=19) && ((tableauJoueur[JoueurEnCours].posY /50) >=12) && surTronc ==0){ //Detection rivière 1
            defaite =1;}
        if(surTroncFondMap ==0){ //Detection rivière 2
            defaite =1;}
        for(int i=0; i<6; i++){ //Collison avec les tonks
            if(tableauJoueur[JoueurEnCours].posY /50 == tabTonk[i].tonkY /50){
                if(tableauJoueur[JoueurEnCours].posX >= tabTonk[i].tonkX && tableauJoueur[JoueurEnCours].posX <= tabTonk[i].tonkX +50){
                    defaite =1;}
            }
        }

        /// ############### DEFAITE ###############
        if(defaite ==1) {
            tableauJoueur[JoueurEnCours].distance = ((LARG+decalage)-tableauJoueur[JoueurEnCours].posY)/50;

            clear_to_color(buffer, makecol(255, 255, 255));
            draw_sprite(buffer, perso[JoueurEnCours], tableauJoueur[JoueurEnCours].posX + 5, tableauJoueur[JoueurEnCours].posY);
            blit(buffer, screen, 0, decalage - modifTerrain, 0, 0, largeur, hauteur + decalage);
            draw_sprite(screen, gameover, 325, 150);

            while (!key[KEY_ENTER]) { rest(10); }
            //Si le joueur 2 perd, fin de la partie
            if(JoueurEnCours == 1) { finDePartie =finPartie(tableauJoueur, perso, tabJoueur); }
            else{
                clear_bitmap(screen);
                clear_bitmap(buffer);
                joueurSuivant(&JoueurEnCours, tableauJoueur, decalage, surTronc, tabTronc);
                surTroncFondMap =1;
                piecesX =300; pieces2X =850; nbPieces =0; tempsMoins =0; ATTRAPE =0; //Reset des pieces
                defaite =0;}
        }
        /// ############### VICTOIRE ###############
        if(tableauJoueur[JoueurEnCours].posY/50 ==0 &&(tableauJoueur[JoueurEnCours].posX >=550 && tableauJoueur[JoueurEnCours].posX <=680)){
            //Temps
            clock_t fin = clock(); //On récupère le temps passé par le joueur dans le jeu
            tableauJoueur[JoueurEnCours].temps = (double)(fin - timeJoueur) / CLOCKS_PER_SEC; //Calcul du temps passé pour le joueur
            tableauJoueur[JoueurEnCours].temps -=tempsMoins;
            draw_sprite(screen, victoire, 210, 150); //On affiche la victoire

            while (!key[KEY_ENTER]) { rest(10); }
            //Fin de partie si le 2e joueur a fini
            if(JoueurEnCours ==1) {
                finDePartie =finPartie(tableauJoueur, perso, tabJoueur);}
            //Reset des bitmaps et des positions
            else {
                clear_bitmap(screen);
                clear_bitmap(buffer);
                joueurSuivant(&JoueurEnCours, tableauJoueur, decalage, surTronc, tabTronc);
                surTroncFondMap = 1;
                defaite = 0;
                piecesX =300; pieces2X =850; nbPieces =0; tempsMoins =0; ATTRAPE =0; //Reset des pieces
                timeJoueur = clock(); //On relance la clock pour la partie du 2e joueur
            }
        }
    } ///FIN DU MINI JEU ---- DESTRUCTION DES BITMAPS
    destroy_bitmap(buffer);
    destroy_bitmap(perso[0]);
    destroy_bitmap(perso[1]);
    destroy_bitmap(gameover);
    destroy_bitmap(victoire);
    destroy_bitmap(tonk);
    for(int i=0; i <3; i++){
        destroy_bitmap(tronc[i]);}
    for(int i=0; i <6; i++){
        destroy_bitmap(pieces[i]);}
}

void joueurSuivant(int *JoueurEnCours, t_joueur tableauJoueur[2], int decalage, int surTronc, t_tronc tabTronc[NBR_RONDIN]){
    if(*JoueurEnCours ==0) *JoueurEnCours =1;

    *JoueurEnCours =1;
    tableauJoueur[*JoueurEnCours].posX = 400;

    ///Reset des joueurs
    tableauJoueur[0].posX = largeur / 2; tableauJoueur[0].posY = hauteur + decalage - 50;
    tableauJoueur[1].posX = largeur / 2; tableauJoueur[1].posY = hauteur + decalage - 50;
    ///Reset des troncs
    surTronc =0;
    for(int i=0; i <NBR_RONDIN; i +=2){ //Position initiale troncs pairs à gauche
        tabTronc[i].troncX =0;
        if(tabTronc[i].type ==2)
            tabTronc[i].troncX2 = tabTronc[i].troncX -300;}
    for(int i=1; i <NBR_RONDIN; i +=2){ //Position initiale troncs impairs à droite
        tabTronc[i].troncX = largeur - 150;
        if(tabTronc[i].type ==2)
            tabTronc[i].troncX2 = tabTronc[i].troncX +300;}
}

int finPartie(t_joueur tableauJoueur[2], BITMAP *perso[2], joueur *tabJoueurs){
    int joueurVictorieux =0;

    clear_to_color(screen, makecol(0, 0, 60));

    BITMAP *coupe = load_bitmap("CR_Coupe.bmp", NULL);
    draw_sprite(screen, coupe, 400, 100);
    masked_stretch_blit(perso[0], screen, 0, 0, 39, 50, 100, 200, 78, 100);
    masked_stretch_blit(perso[1], screen, 0, 0, 39, 50, 1000, 200, 78, 100);

    if(tableauJoueur[0].temps !=0){
        if(tableauJoueur[1].temps !=0){
            //Les deux joueurs ont fini on cherche donc le temps le plus petit
            if(tableauJoueur[0].temps > tableauJoueur[1].temps){
                textprintf_ex(screen, font, 100, 320, makecol(255, 255, 255),-1, "VICTOIRE !"); joueurVictorieux =1;}
            else if(tableauJoueur[0].temps < tableauJoueur[1].temps){
                textprintf_ex(screen, font, 1000, 320, makecol(255, 255, 255),-1, "VICTOIRE !"); joueurVictorieux =2;}
            textprintf_ex(screen, font, 100, 350, makecol(255, 255, 255),-1, "Temps : %f", tableauJoueur[0].temps);
            textprintf_ex(screen, font, 1000, 350, makecol(255, 255, 255),-1, "Temps : %f", tableauJoueur[1].temps);
        }
        else{
            textprintf_ex(screen, font, 100, 320, makecol(255, 255, 255),-1, "VICTOIRE !");
            textprintf_ex(screen, font, 100, 350, makecol(255, 255, 255),-1, "Temps : %f", tableauJoueur[0].temps); joueurVictorieux =1;}
    }
    else{
        if(tableauJoueur[1].temps !=0){
            textprintf_ex(screen, font, 1000, 320, makecol(255, 255, 255),-1, "VICTOIRE !");
            textprintf_ex(screen, font, 1000, 350, makecol(255, 255, 255),-1, "Temps : %f", tableauJoueur[1].temps); joueurVictorieux =2;
        }
        else{
            if(tableauJoueur[0].distance >tableauJoueur[1].distance){
                textprintf_ex(screen, font, 100, 320, makecol(255, 255, 255),-1, "VICTOIRE !"); joueurVictorieux =1;}
            else if(tableauJoueur[0].distance <tableauJoueur[1].distance){
                textprintf_ex(screen, font, 1000, 320, makecol(255, 255, 255),-1, "VICTOIRE !"); joueurVictorieux =2;}
            else{
                textprintf_ex(screen, font, 100, 320, makecol(255, 255, 255),-1, "EGALITE !"); joueurVictorieux =3;
                textprintf_ex(screen, font, 1000, 320, makecol(255, 255, 255),-1, "EGALITE !");
            }
            textprintf_ex(screen, font, 100, 350, makecol(255, 255, 255),-1, "Distance : %d", tableauJoueur[0].distance);
            textprintf_ex(screen, font, 1000, 350, makecol(255, 255, 255),-1, "Distance : %d", tableauJoueur[1].distance);
        }
    }

    if(joueurVictorieux ==3){
        textprintf_ex(screen, font, 510, 20, makecol(255, 255, 255),-1, "Pas de ticket pour les joueurs");}
    else{
        textprintf_ex(screen, font, 520, 20, makecol(255, 255, 255),-1, "Joueur %d gagne un ticket !", joueurVictorieux);}
    textprintf_ex(screen, font, 550, 620, makecol(255, 255, 255),-1, "Press UP to exit");

    while (!key[KEY_UP]) { rest(50); }

    //Gestion des tickets
    tabJoueurs[joueurVictorieux-1].tickets +=1;
    tabJoueurs[0].performance_CrossyRoad =0; tabJoueurs[1].performance_CrossyRoad = 0;
    tabJoueurs[joueurVictorieux-1].performance_CrossyRoad = tableauJoueur[joueurVictorieux-1].temps;

    return 1;
}