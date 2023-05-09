#include "Header.h"

typedef struct Joueur{
    char nom[100];
    int x;
    int y;
    int tickets;
    BITMAP* sprite;
}joueur;

int choixJoueurs(joueur *tabJoueur){
    BITMAP *buffer= create_bitmap(SCREEN_W,SCREEN_H);
    BITMAP *sprite1= load_bitmap("bonhomme.bmp",NULL);
    BITMAP *maisonCanard= load_bitmap("maisonCanard.bmp",NULL);
    if (!sprite1){
        allegro_message("pas pu trouver sprite1.bmp");
        exit(EXIT_FAILURE);}
    BITMAP *sprite2= load_bitmap("bonhomme2.bmp",NULL);
    if (!sprite2){
        allegro_message("pas pu trouver sprite2.bmp");
        exit(EXIT_FAILURE);}
    int i=0;
    while(!key[KEY_ESC]){
        show_mouse(buffer);
        while (i<2) {
            tabJoueur[0].x=0;
            tabJoueur[0].y=0;
            tabJoueur[1].x=35;
            tabJoueur[1].y=0;
            show_mouse(buffer);
            textprintf_ex(buffer, font, 340, 50, makecol(255, 255, 255), 0, "CHOIX PERSONNAGE");
            if (mouse_x >= SCREEN_W / 4 - 75 && mouse_x <= SCREEN_W / 4 - 75 + 150 && mouse_y >= SCREEN_H / 4 &&
                mouse_y <= SCREEN_H / 4 + 200) {
                stretch_sprite(buffer, sprite1, SCREEN_W / 4 - 75, SCREEN_H / 4, 180, 240);
                stretch_sprite(buffer, sprite2, SCREEN_W * 6 / 8 - 75, SCREEN_H / 4, 150, 200);
                if (mouse_b == 1) {
                    tabJoueur[i].sprite = sprite1;
                    rest(500);
                    i++;
                }
            } else if (mouse_x >= SCREEN_W * 6 / 8 - 75 && mouse_x <= SCREEN_W * 6 / 8 - 75 + 150 &&
                       mouse_y >= SCREEN_H / 4 && mouse_y <= SCREEN_H / 4 + 200) {
                stretch_sprite(buffer, sprite2, SCREEN_W * 6 / 8 - 75, SCREEN_H / 4, 180, 240);
                stretch_sprite(buffer, sprite1, SCREEN_W / 4 - 75, SCREEN_H / 4, 150, 200);
                if (mouse_b == 1) {
                    tabJoueur[i].sprite = sprite2;
                    rest(500);
                    i++;
                }
            } else {
                stretch_sprite(buffer, sprite1, SCREEN_W / 4 - 75, SCREEN_H / 4, 150, 200);
                stretch_sprite(buffer, sprite2, SCREEN_W * 6 / 8 - 75, SCREEN_H / 4, 150, 200);
            }
            blit(buffer,screen,0,0,0,0,SCREEN_W,SCREEN_H);
            clear(buffer);
        }
        stretch_sprite(buffer,maisonCanard,260,34,100,100);
        rectfill(buffer,300,90,320,114,makecol(0,0,0));
        stretch_sprite(buffer,tabJoueur[0].sprite,tabJoueur[0].x,tabJoueur[0].y,30,40);
        stretch_sprite(buffer,tabJoueur[1].sprite,tabJoueur[1].x,tabJoueur[1].y,30,40);
        if(key[KEY_UP]&& tabJoueur[1].y>0){
            tabJoueur[1].y=tabJoueur[1].y-10;
            rest(50);
        }
        if (key[KEY_DOWN]&&tabJoueur[1].y<SCREEN_H-40){
            tabJoueur[1].y=tabJoueur[1].y+10;
            rest(50);
        }
        if(key[KEY_LEFT]&& tabJoueur[1].x>0){
            tabJoueur[1].x=tabJoueur[1].x-10;
            rest(50);
        }
        if (key[KEY_RIGHT]&&tabJoueur[1].x<SCREEN_W-30){
            tabJoueur[1].x=tabJoueur[1].x+10;
            rest(50);
        }
        if(key[KEY_W]&& tabJoueur[0].y>=0){
            tabJoueur[0].y=tabJoueur[0].y-10;
            rest(50);
        }
        if (key[KEY_S]&&tabJoueur[0].y<SCREEN_H-40){
            tabJoueur[0].y=tabJoueur[0].y+10;
            rest(50);
        }
        if(key[KEY_A]&& tabJoueur[0].x>=0){
            tabJoueur[0].x=tabJoueur[0].x-10;
            rest(50);
        }
        if (key[KEY_D]&&tabJoueur[0].x<SCREEN_W-30){
            tabJoueur[0].x=tabJoueur[0].x+10;
            rest(50);
        }
            blit(buffer,screen,0,0,0,0,SCREEN_W,SCREEN_H);
            clear(buffer);
        }
}

int main(void) {
    allegro_init();
    install_keyboard();
    set_color_depth(desktop_color_depth());
    if((set_gfx_mode(GFX_AUTODETECT_WINDOWED,(LONG),(LARG),0,0))!=0){
        allegro_message("Pb de mode graphique");
        allegro_exit();
        exit(EXIT_FAILURE);}

    /* Menu → Permet de sélectionner son pseudo et son joueur, le tout est stocké dans une structure Joueur
     * Le menu lance ensuite la Map principale, cette map doit afficher le numéro du joueur en cours et son
     * nombre de tickets (rappel à 0 fin de partie) elle lance aussi les mini-jeux → chaque mini jeu est
     * un sous programme indépendant (appelle les Bitmaps au début et les libère à la fin) → prend en
     * paramètres la structure joueur pour les tickets
     * La Map principale doit bien gérer la sortie du sous programme de mini jeu avec le passage au joueur
     * suivant.
     * Enfin il faut s'occuper de la victoire et quitter allegro (ça vide la mémoire automatiquement). */

    //menu();
    joueur *tabJoueur[2];
    choixJoueurs(tabJoueur);
    allegro_exit();
    return 0;
}
END_OF_MAIN()