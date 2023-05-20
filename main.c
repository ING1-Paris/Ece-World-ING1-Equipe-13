#include "Header.h"

int main(void) {
    srand(time(NULL));

    allegro_init();
    install_mouse();
    install_keyboard();
    install_mouse();
    set_color_depth(desktop_color_depth());
    install_sound(DIGI_AUTODETECT, MIDI_AUTODETECT, NULL);
    if ((set_gfx_mode(GFX_AUTODETECT_WINDOWED, (LONG), (LARG), 0, 0)) != 0) {
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

    //menu();

    joueur tabJoueur[2];
    menu_map(tabJoueur);
    allegro_exit();
    return 0;
}

END_OF_MAIN()

void nomJoueur(char *name, BITMAP* buffer,int i)
{
    textprintf_ex(buffer, font, LARG / 2, LONG / 2 - text_height(font), makecol(255, 255, 255), 0, "Entrez le nom du joueur %d",i);
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

        textprintf_ex(buffer, font, LARG / 2, LONG / 2 - text_height(font), makecol(255, 255, 255), 0, "Entrez le nom du joueur %d",i);
        textprintf_ex(buffer, font, LARG / 2, LONG / 2 + text_height(font), makecol(255, 255, 255), 0, "%s",name);

        blit(buffer, screen, 0, 0, 0, 0, LARG, LONG);

        rest(10);
    
    }
}

int menu_map(joueur *tabJoueur) {
    int memory=0;
    int crossyRoad=0;
    int peche=0;
    int tirBallons=0;
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
    int i = 0;
    while (!key[KEY_ESC]) {
        draw_sprite(buffer,fondMap,0,0);

        show_mouse(buffer);
        while (i < 2) {
            tabJoueur[0].x = 38*16;
            tabJoueur[0].y = 37*16;
            tabJoueur[1].x = 40*16;
            tabJoueur[1].y = 37*16;
            tabJoueur[0].tickets=0;
            tabJoueur[1].tickets=0;

            show_mouse(buffer);
            textprintf_ex(buffer, font, 540, 50, makecol(255, 255, 255), 0, "CHOIX PERSONNAGE");
            textprintf_ex(buffer, font, 355, 70, makecol(255, 255, 255), 0, "Choisissez le personnage que vous allez jouer et saisissez votre nom.");

            if (mouse_x >= SCREEN_W / 4 - 75 && mouse_x <= SCREEN_W / 4 - 75 + 150 && mouse_y >= SCREEN_H / 4 &&
                mouse_y <= SCREEN_H / 4 + 200) {
                stretch_sprite(buffer, sprite1, SCREEN_W / 4 - 75, SCREEN_H / 4, 180, 240);
                stretch_sprite(buffer, sprite2, SCREEN_W * 6 / 8 - 75, SCREEN_H / 4, 150, 200);
                if (mouse_b == 1) {
                    tabJoueur[i].sprite = sprite1;
                    nomJoueur(tabJoueur[i].name,buffer,i);
                    rest(500);
                    i++;
                }
            } else if (mouse_x >= SCREEN_W * 6 / 8 - 75 && mouse_x <= SCREEN_W * 6 / 8 - 75 + 150 &&
                       mouse_y >= SCREEN_H / 4 && mouse_y <= SCREEN_H / 4 + 200) {
                stretch_sprite(buffer, sprite2, SCREEN_W * 6 / 8 - 75, SCREEN_H / 4, 180, 240);
                stretch_sprite(buffer, sprite1, SCREEN_W / 4 - 75, SCREEN_H / 4, 150, 200);
                if (mouse_b == 1) {
                    tabJoueur[i].sprite = sprite2;
                    nomJoueur(tabJoueur[i].name,buffer,i);
                    rest(500);
                    i++;

                }
            } else {
                stretch_sprite(buffer, sprite1, SCREEN_W / 4 - 75, SCREEN_H / 4, 150, 200);
                stretch_sprite(buffer, sprite2, SCREEN_W * 6 / 8 - 75, SCREEN_H / 4, 150, 200);

            }
            blit(buffer, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
            clear(buffer);
        }
        //porte memory
        rectfill(buffer, 29*16, 11*16, 31*16, 13*16, makecol(155, 0, 250));
        //porte des canards
        rectfill(buffer, 30*16-10, 28*16, 32*16-10, 30*16, makecol(0, 0, 0));
        //porte crossy road
        //rectfill(buffer, 9*16, 16*16, 11*16, 18*16, makecol(155, 0, 250));
        //porte tir aux ballons
        //rectfill(buffer, 47*16-5, 15*16, 47*16+43, 16*16+32, makecol(155, 0, 250));

        //affichage des joueurs en fonction de leur position
        stretch_sprite(buffer, tabJoueur[0].sprite, tabJoueur[0].x, tabJoueur[0].y, 30, 40);
        stretch_sprite(buffer, tabJoueur[1].sprite, tabJoueur[1].x, tabJoueur[1].y, 30, 40);

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
        if(tabJoueur[0].x>=30*16-10 &&tabJoueur[0].x<=32*16+5 && tabJoueur[0].y>=28*16-5 &&tabJoueur[0].y<=30*16 && peche==0 ){
            rest(300);
            jeu_canards(buffer,tabJoueur);
            peche=1;
        }
        if(tabJoueur[0].x>=29*16 &&tabJoueur[0].x<=29*16+32 && tabJoueur[0].y>=11*16 &&tabJoueur[0].y<=13*16 && memory==0){
            rest(100);
            jeuMemory(buffer, tabJoueur);
            memory=1;
        }
        if(tabJoueur[0].x>=9*16-10 &&tabJoueur[0].x<=11*16 && tabJoueur[0].y>=16*16 &&tabJoueur[0].y<=18*16 && crossyRoad==0){
            rest(100);
            crossy_road();
            crossyRoad=1;
        }
        if(tabJoueur[0].x>=47*16-5 &&tabJoueur[0].x<=47*16+43 && tabJoueur[0].y>=15*16 &&tabJoueur[0].y<=16*16+32 && tirBallons==0){
            rest(100);
            tir_aux_ballons();
            tirBallons=1;
        }
        if(tabJoueur[0].x>=38*16-10 &&tabJoueur[0].x<=42*16+5 && tabJoueur[0].y>=38*16){
            return 0;
        }
        if(tabJoueur[1].x>=38*16-10 &&tabJoueur[1].x<=42*16+5 && tabJoueur[1].y>=38*16){
            return 0;
        }
        blit(buffer, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
        clear(buffer);
    }
    return 0;
}