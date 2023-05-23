#include "Header.h"

int main(void) {
    srand(time(NULL));

    allegro_init();
    install_mouse();
    install_keyboard();
    install_mouse();
    set_color_depth(desktop_color_depth());
    install_sound(DIGI_AUTODETECT, MIDI_AUTODETECT, NULL);
    if ((set_gfx_mode(GFX_AUTODETECT_WINDOWED, (largeur), (hauteur), 0, 0)) != 0) {
        allegro_message("Pb de mode graphique");
        allegro_exit();
        exit(EXIT_FAILURE);
    }

    /* Menu → Permet de sélectionner son pseudo et son joueur, le tout est stocké dans une structure Joueur
     * Le menu lance ensuite la Map principale, cette map doit afficher le numéro du joueur en cours et son
     * nombre de tickets (rappel à 0 fin de partie) elle lance aussi les mini-jeux → chaque mini jeu est
     * un sous programme indépendant (appelle les Bitmaps au début et les libère à la fin) → prend en
     * paramètres la structure joueur pour les tickets
     * La Map principale doit bien gérer la sortie du sous programme de mini jeu avec le passage au joueur
     * suivant.
     * Enfin il faut s'occuper de la victoire et quitter allegro (ça vide la mémoire automatiquement). */

    joueur tabJoueur[2];
    initialisation_joueur(tabJoueur);
    menu_map(tabJoueur);

    allegro_exit();
    return 0;
}
END_OF_MAIN()


void initialisation_joueur(joueur *tabJoueur){
    for(int i=0; i<2; i++){
        tabJoueur[i].performance_CrossyRoad =0;
        tabJoueur[i].performance_Memory =0;
        tabJoueur[i].performance_Canards =0;
        tabJoueur[i].performance_TirAuxBallons =0;
    }
}

void ecran_score(BITMAP *buffer, joueur *tabJoueur){
    clear(buffer);
    clear(screen);
    BITMAP *scores =load_bitmap("SCORES.bmp",NULL);
    clear_to_color(buffer,makecol(255,255,255));
    int bestScore =0;
    while(!key[KEY_SPACE]){
        draw_sprite(buffer,scores,300,15);
        textprintf_ex(buffer, font, 350, 200, makecol(0, 0, 0),-1, "JOUEUR 1");
        textprintf_ex(buffer, font, 900, 200, makecol(0, 0, 0),-1, "JOUEUR 2");

        textprintf_ex(buffer, font, 10, 250, makecol(0, 0, 0),-1, "Score a la peche aux canards :");
        textprintf_ex(buffer, font, 350, 250, makecol(0, 0, 0),-1, "%d", tabJoueur[0].performance_Canards);
        textprintf_ex(buffer, font, 900, 250, makecol(0, 0, 0),-1, "%d", tabJoueur[1].performance_Canards);

        textprintf_ex(buffer, font, 10, 300, makecol(0, 0, 0),-1, "Score au tir aux ballons :");
        textprintf_ex(buffer, font, 350, 300, makecol(0, 0, 0),-1, "%d", tabJoueur[0].performance_TirAuxBallons);
        textprintf_ex(buffer, font, 900, 300, makecol(0, 0, 0),-1, "%d", tabJoueur[1].performance_TirAuxBallons);

        textprintf_ex(buffer, font, 10, 350, makecol(0, 0, 0),-1, "Score au crossy road :");
        textprintf_ex(buffer, font, 350, 350, makecol(0, 0, 0),-1, "%.2f", tabJoueur[0].performance_CrossyRoad);
        textprintf_ex(buffer, font, 900, 350, makecol(0, 0, 0),-1, "%.2f", tabJoueur[1].performance_CrossyRoad);

        textprintf_ex(buffer, font, 10, 400, makecol(0, 0, 0),-1, "Score au memory :");
        textprintf_ex(buffer, font, 350, 400, makecol(0, 0, 0),-1, "%d", tabJoueur[0].performance_Memory);
        textprintf_ex(buffer, font, 900, 400, makecol(0, 0, 0),-1, "%d", tabJoueur[1].performance_Memory);

        textprintf_ex(buffer, font, 250, 600, makecol(0, 0, 0),-1, "Appuyez sur Espace pour quitter ou Tab pour les meilleurs scores de tous les temps");
        blit(buffer, screen, 0, 0, 0, 0, largeur, hauteur);
        if(key[KEY_TAB]){
            ///AFFICHAGE MEILLEURS SCORES
            FILE *pointeur = NULL;
            pointeur = fopen("meilleursScores.txt", "w+");
            if (pointeur == NULL) { printf("Erreur d'ouverture de fichier."); }
            clear_to_color(buffer, makecol(255, 255, 255));
            draw_sprite(buffer, scores, 300, 15);
            textprintf_ex(buffer, font, 10, 250, makecol(0, 0, 0), -1, "Meilleur Score a la peche aux canards :");
            textprintf_ex(buffer, font, 10, 300, makecol(0, 0, 0), -1, "Meilleur Score au tir aux ballons :");
            textprintf_ex(buffer, font, 10, 350, makecol(0, 0, 0), -1, "Meilleur Score au crossy road :");
            textprintf_ex(buffer, font, 10, 400, makecol(0, 0, 0), -1, "Meilleur Score au memory :");

            textprintf_ex(buffer, font, 400, 250, makecol(0, 0, 0), -1, "14 / 12 / 12");
            textprintf_ex(buffer, font, 400, 300, makecol(0, 0, 0), -1, "0 / 0 / 0");
            textprintf_ex(buffer, font, 400, 350, makecol(0, 0, 0), -1, "17.28 / 19.55 / 20.80");
            textprintf_ex(buffer, font, 400, 400, makecol(0, 0, 0), -1, "0 / 0 / 0");

            textprintf_ex(buffer, font, 250, 600, makecol(0, 0, 0), -1, "Appuyez sur Espace pour quitter");
            blit(buffer, screen, 0, 0, 0, 0, largeur, hauteur);
            while (!key[KEY_SPACE]) { rest(10); }
        }
    }
    clear(buffer);
    for(int i=0; i<2; i++){ //Reset de la position des joueurs
        tabJoueur[i].x =(38 +i*2)*16;
        tabJoueur[i].y =37*16;
    }
}

void nomJoueur(char *name, BITMAP* buffer,int i)
{
    textprintf_ex(buffer, font, hauteur / 2, largeur / 2 - text_height(font), makecol(255, 255, 255), 0, "Entrez le nom du joueur %d", i);
    int pos = 0;
    while (!key[KEY_ENTER])
    {
        if (keypressed())
        {
            int key = readkey() & 0xFF;
            if (key == '\b' && pos > 0)
            {
                name[--pos] = '\0';
            }
            else if (key >= ' ' && key <= '~' && pos < 255)
            {
                name[pos++] = key;
                name[pos] = '\0';
            }
        }

        textprintf_ex(buffer, font, hauteur / 2, largeur / 2 - text_height(font), makecol(255, 255, 255), 0, "Entrez le nom du joueur %d", i);
        textprintf_ex(buffer, font, hauteur / 2, largeur / 2 + text_height(font), makecol(255, 255, 255), 0, "%s", name);

        blit(buffer, screen, 0, 0, 0, 0, hauteur, largeur);

        rest(10);
    
    }
}

int menu_map(joueur *tabJoueur) {
    BITMAP *buffer = create_bitmap(SCREEN_W, SCREEN_H);
    BITMAP *sprite1 = load_bitmap("bonhomme.bmp", NULL);

    if (!sprite1) {
        allegro_message("pas pu trouver sprite1.bmp");
        exit(EXIT_FAILURE);
    }
    BITMAP *sprite2 = load_bitmap("bonhomme2.bmp", NULL);
    if (!sprite2) {
        allegro_message("pas pu trouver sprite2.bmp");
        exit(EXIT_FAILURE);
    }
    BITMAP *fondMap= load_bitmap("map.bmp",NULL);
    BITMAP *porte= load_bitmap("porte.bmp",NULL);
    BITMAP *ticket= load_bitmap("goldenTicket.bmp",NULL);
    int iter = 0;
    while (!key[KEY_CAPSLOCK]) {
        draw_sprite(buffer,fondMap,0,0);

        while (iter < 2) {
            ///INITIALISATION DES VARIABLES JOUEUR
            tabJoueur[0].x = 38*16;
            tabJoueur[0].y = 37*16;
            tabJoueur[1].x = 40*16;
            tabJoueur[1].y = 37*16;
            tabJoueur[0].tickets=5;
            tabJoueur[1].tickets=5;

            ///LANCEMENT DU CHOIX DU PERSO
            clear_to_color(buffer, makecol(3, 34, 76));
            textprintf_ex(buffer, font, 540, 50, makecol(255, 255, 255), 0, "CHOIX PERSONNAGE");
            textprintf_ex(buffer, font, 355, 70, makecol(255, 255, 255), 0, "Choisissez le personnage que vous allez jouer et saisissez votre nom.");

            if (mouse_x >= SCREEN_W / 4 - 75 && mouse_x <= SCREEN_W / 4 - 75 + 150 && mouse_y >= SCREEN_H / 4 &&
                mouse_y <= SCREEN_H / 4 + 200) {
                stretch_sprite(buffer, sprite1, SCREEN_W / 4 - 75, SCREEN_H / 4, 180, 240);
                stretch_sprite(buffer, sprite2, SCREEN_W * 6 / 8 - 75, SCREEN_H / 4, 150, 200);
                if (mouse_b == 1) {
                    tabJoueur[iter].sprite = sprite1;
                    nomJoueur(tabJoueur[iter].name,buffer,iter);
                    rest(500);
                    iter++;
                }
            } else if (mouse_x >= SCREEN_W * 6 / 8 - 75 && mouse_x <= SCREEN_W * 6 / 8 - 75 + 150 &&
                       mouse_y >= SCREEN_H / 4 && mouse_y <= SCREEN_H / 4 + 200) {
                stretch_sprite(buffer, sprite2, SCREEN_W * 6 / 8 - 75, SCREEN_H / 4, 180, 240);
                stretch_sprite(buffer, sprite1, SCREEN_W / 4 - 75, SCREEN_H / 4, 150, 200);
                if (mouse_b == 1) {
                    tabJoueur[iter].sprite = sprite2;
                    nomJoueur(tabJoueur[iter].name,buffer,iter);
                    rest(500);
                    iter++;

                }
            } else {
                stretch_sprite(buffer, sprite1, SCREEN_W / 4 - 75, SCREEN_H / 4, 150, 200);
                stretch_sprite(buffer, sprite2, SCREEN_W * 6 / 8 - 75, SCREEN_H / 4, 150, 200);

            }
            show_mouse(buffer);
            blit(buffer, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
            clear(buffer);
        }//Fin choix du perso

        //porte memory
        stretch_sprite(buffer,porte,29*16-10,9*16,3*16,4*16);
        //rectfill(buffer, 29*16, 11*16, 31*16, 13*16, makecol(155, 0, 250));
        //porte des canards
        stretch_sprite(buffer,porte,29*16,27*16+3,2*16+8,3*16);
        //rectfill(buffer, 30*16-10, 28*16, 32*16-10, 30*16, makecol(0, 0, 0));
        //porte crossy road
        //rectfill(buffer, 9*16, 16*16, 11*16, 18*16, makecol(155, 0, 250));
        //porte tir aux ballons
        //rectfill(buffer, 47*16-5, 15*16, 47*16+43, 16*16+32, makecol(155, 0, 250));


        /// Affichage des joueurs en fonction de leur position
        stretch_sprite(buffer, tabJoueur[0].sprite, tabJoueur[0].x, tabJoueur[0].y, 30, 40);
        stretch_sprite(buffer, tabJoueur[1].sprite, tabJoueur[1].x, tabJoueur[1].y, 30, 40);

        ///GESTION DES TOUCHES
        if (key[KEY_UP] && tabJoueur[1].y > 0) {
            tabJoueur[1].y = tabJoueur[1].y - 10;
            rest(50);
        }
        if (key[KEY_DOWN] && tabJoueur[1].y < SCREEN_H - 40) {
            tabJoueur[1].y = tabJoueur[1].y + 10;
            rest(50);
        }
        if (key[KEY_LEFT] && tabJoueur[1].x > 0) {
            tabJoueur[1].x = tabJoueur[1].x - 10;
            rest(50);
        }
        if (key[KEY_RIGHT] && tabJoueur[1].x < SCREEN_W - 30) {
            tabJoueur[1].x = tabJoueur[1].x + 10;
            rest(50);
        }
        if (key[KEY_W] && tabJoueur[0].y >= 0) {
            tabJoueur[0].y = tabJoueur[0].y - 10;
            rest(50);
        }
        if (key[KEY_S] && tabJoueur[0].y < SCREEN_H - 40) {
            tabJoueur[0].y = tabJoueur[0].y + 10;
            rest(50);
        }
        if (key[KEY_A] && tabJoueur[0].x >= 0) {
            tabJoueur[0].x = tabJoueur[0].x - 10;
            rest(50);
        }
        if (key[KEY_D] && tabJoueur[0].x < SCREEN_W - 30) {
            tabJoueur[0].x = tabJoueur[0].x + 10;
            rest(50);
        }
        ///################# LANCEMENT DES MINI-JEUX ####################
        for(int i=0; i<2; i++) { //canards
            if (tabJoueur[i].x >= 30*16-30 &&tabJoueur[i].x <=32*16+5 &&tabJoueur[i].y>=28*16-5 &&tabJoueur[i].y<=30*16){
                rest(100);
                tabJoueur[0].tickets--; tabJoueur[1].tickets--;
                jeu_canards(buffer, tabJoueur);
                tabJoueur[i].x =(38 +i*2)*16; tabJoueur[i].y =37*16; //Réinitialisation de la position des joueurs
            }
        }
        for(int i=0; i<2; i++) { //memory
            if (tabJoueur[i].x >= 29*16 &&tabJoueur[i].x <=29*16+32 &&tabJoueur[i].y >=11*16 &&tabJoueur[i].y <=13*16){
                rest(100);
                tabJoueur[0].tickets--; tabJoueur[1].tickets--;
                jeuMemory(buffer, tabJoueur);
                tabJoueur[i].x =(38 +i*2)*16; tabJoueur[i].y =37*16; //Réinitialisation de la position des joueurs
            }
        }
        for(int i=0; i<2; i++) { //crossy road
            if (tabJoueur[i].x >=9*16-10 &&tabJoueur[i].x <=11*16 &&tabJoueur[i].y >=16*16 &&tabJoueur[i].y <=18*16) {
                rest(100);
                tabJoueur[0].tickets--; tabJoueur[1].tickets--;
                crossy_road(tabJoueur);
                tabJoueur[i].x = (38 + i * 2) * 16; tabJoueur[i].y = 37 * 16; //Réinitialisation de la position des joueurs
            }
        }
        for(int i=0; i<2; i++) { //tir aux ballons
            if(tabJoueur[i].x>=47*16-5 &&tabJoueur[i].x<=47*16+43 && tabJoueur[i].y>=15*16 &&tabJoueur[i].y<=16*16+32) {
                rest(100);
                tabJoueur[0].tickets--; tabJoueur[1].tickets--;
                tir_aux_ballons();
                tabJoueur[i].x = (38 + i * 2) * 16; tabJoueur[i].y = 37 * 16; //Réinitialisation de la position des joueurs
            }
        }

        ///################# AFFICHAGE DES SCORES ####################
        for(int i=0; i<2; i++){
            if(tabJoueur[i].x>=38*16-10 &&tabJoueur[i].x<=42*16+5 && tabJoueur[i].y>=38*16){
                ecran_score(buffer, tabJoueur);
            }
        }

        ///Affichage tickets
        draw_sprite(buffer, ticket, 1020, 15);
        textprintf_ex(buffer, font, 1130, 30, makecol(120, 51, 0), -1, "J1: %d", tabJoueur[0].tickets);
        textprintf_ex(buffer, font, 1130, 50, makecol(120, 51, 0), -1, "J2: %d", tabJoueur[1].tickets);

        ///Affichage buffer / écran
        blit(buffer, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
        clear(buffer);

        ///Detection de la victoire
        if(tabJoueur[0].tickets <= 0 || tabJoueur[1].tickets <= 0) {
            rest(1500);
            BITMAP *coupe = load_bitmap("CR_Coupe.bmp", NULL);
            clear_bitmap(buffer); clear_bitmap(screen);
            clear_to_color(screen, makecol(3, 34, 76));
            draw_sprite(screen, coupe, 400, 100);
            if (tabJoueur[1].tickets == 0) {
                masked_stretch_blit(tabJoueur[0].sprite, screen, 0, 0, 35, 50, 560, 200, 70, 100);
                textprintf_ex(screen, font, 500, 20, makecol(255, 255, 255), -1, "VICTOIRE DE %s !", tabJoueur[0].name);
            }
            else {
                masked_stretch_blit(tabJoueur[1].sprite, screen, 0, 0, 35, 50, 560, 200, 70, 100);
                textprintf_ex(screen, font, 500, 20, makecol(255, 255, 255), -1, "VICTOIRE DE %s !", tabJoueur[1].name);
            }
            textprintf_ex(screen, font, 550, 620, makecol(255, 255, 255),-1, "Press ESC to exit");

            while (!key[KEY_ESC]) { rest(50); }
            return 0;
        }
    }

    return 0;
}