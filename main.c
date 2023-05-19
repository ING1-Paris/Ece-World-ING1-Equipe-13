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
    //crossy_road();
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
    BITMAP *buffer = create_bitmap(SCREEN_W, SCREEN_H);
    BITMAP *sprite1 = load_bitmap("bonhomme.bmp", NULL);
    BITMAP *maisonCanard = load_bitmap("maisonCanard.bmp", NULL);
    BITMAP  *CrossyRoad= load_bitmap("MaisonRoad.bmp",NULL);
    BITMAP *arche= load_bitmap("Arche.bmp",NULL);
    if (!sprite1) {
        allegro_message("pas pu trouver sprite1.bmp");
        exit(EXIT_FAILURE);
    }
    BITMAP *sprite2 = load_bitmap("bonhomme2.bmp", NULL);
    if (!sprite2) {
        allegro_message("pas pu trouver sprite2.bmp");
        exit(EXIT_FAILURE);
    }
    int i = 0;
    while (!key[KEY_ESC]) {
        show_mouse(buffer);
        while (i < 2) {
            tabJoueur[0].x = 0;
            tabJoueur[0].y = 0;
            tabJoueur[1].x = 35;
            tabJoueur[1].y = 0;
            show_mouse(buffer);
            textprintf_ex(buffer, font, 340, 50, makecol(255, 255, 255), 0, "CHOIX PERSONNAGE");
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
        stretch_sprite(buffer, maisonCanard, 260, 34, 100, 100);
        stretch_sprite(buffer, CrossyRoad, 100, 159, 150, 150);
        //porte memory
        rectfill(buffer, 400, 190, 430, 230, makecol(155, 0, 250));
        //porte des canards
        rectfill(buffer, 300, 90, 330, 130, makecol(155, 0, 250));
        //porte crossy road
        rectfill(buffer, 500, 290, 530, 330, makecol(155, 0, 250));

        stretch_sprite(buffer, tabJoueur[0].sprite, tabJoueur[0].x, tabJoueur[0].y, 30, 40);
        stretch_sprite(buffer, tabJoueur[1].sprite, tabJoueur[1].x, tabJoueur[1].y, 30, 40);
        stretch_sprite(buffer, arche, 260, 300, 200, 200);
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
        if(tabJoueur[0].x>=300 &&tabJoueur[0].x<=330 && tabJoueur[0].y>=90 &&tabJoueur[0].y<=130 && peche==0 ){
            rest(100);
            jeu_canards(buffer,tabJoueur);
            peche=1;
        }
        if(tabJoueur[0].x>=400 &&tabJoueur[0].x<=430 && tabJoueur[0].y>=190 &&tabJoueur[0].y<=230 && memory==0){
            rest(100);
            jeuMemory();
            memory=1;
        }
        if(tabJoueur[0].x>=500 &&tabJoueur[0].x<=530 && tabJoueur[0].y>=290 &&tabJoueur[0].y<=330 && crossyRoad==0){
            rest(100);
            crossy_road();
            crossyRoad=1;
        }
        blit(buffer, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
        clear(buffer);
    }
}