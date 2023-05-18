//
// Created by elsab on 10/05/2023.
//

#include "Header.h"


void initialisationAllegro();
int collide_point_cercle(int point_x, int point_y, int cercle_x, int cercle_y, int rayon);
int determinerGagnant(long tempsJoueur1, long tempsJoueur2);

long tempsJoueur1 = 0;
long tempsJoueur2 = 0;


int partie(BITMAP *buffer, long *tempsJoueur1, long *tempsJoueur2,int joueur){
    //declaration de variables
    BITMAP *numeroBallons[MAX_IMAGES_BALLONS];
    numeroBallons[0] = load_bitmap("ballon 1.bmp", NULL);
    numeroBallons[1] = load_bitmap("ballon 2.bmp", NULL);
    numeroBallons[2] = load_bitmap("ballon 3.bmp", NULL);
    numeroBallons[3] = load_bitmap("ballon 4.bmp", NULL);
    numeroBallons[4] = load_bitmap("ballon 5.bmp", NULL);
    numeroBallons[5] = load_bitmap("ballon 6.bmp", NULL);
    int ballonClique = -1; //au début aucun ballon n'ést cliqué
    int tempsDebut = clock(); // temps de début du jeu pour le joueur courant
    int joueurActuel = joueur;
    int compteur=0;
    long tempsJoueur;


    // Charger un fond
    BITMAP *fond = load_bitmap("fond jeu.bmp", NULL);
    if (!fond) {
        allegro_message("prb chargement image");
        allegro_exit();
        exit(EXIT_FAILURE);
    }
    // Charger l'image de la cible
    BITMAP *cible = load_bitmap("cible.bmp", NULL);

    if (!numeroBallons[5]) {
        allegro_message("probleme chargement image");
        allegro_exit();
        exit(EXIT_FAILURE);
    }

    Ballon ballons[MAX_IMAGES_BALLONS];  //initialisation
    for(int i=0; i<MAX_IMAGES_BALLONS; i++){
        ballons[i].x = rand() % (SCREEN_W - 300);
        ballons[i].y =rand() % (SCREEN_H - 300);
        ballons[i].image = numeroBallons[i];
        ballons[i].dx = rand() % 2 + 1;
        ballons[i].dy = rand() % 3 + 1;
        ballons[i].eclate = 0;
    }

    // Boucle de jeu
    while (!key[KEY_ESC] && compteur!=6/*&& tousBallonsEclates(ballons)==1*/) {
        //AFFICHER LE FOND
        stretch_sprite(buffer, fond, 0, 0, SCREEN_W, SCREEN_H);
        //AFFICHER LES BALLONS SUR L'ECRAN
        if (mouse_b & 1) {
            for (int i = 0; i < MAX_IMAGES_BALLONS; i++) {
                if (!ballons[i].eclate && collide_point_cercle(mouse_x, mouse_y, ballons[i].x + 100, ballons[i].y + 100, 100)) {
                    ballons[i].eclate = 1; // marquer le ballon comme étant éclaté
                    compteur++;
                    ballonClique = i; // enregistrer l'indice du ballon cliqué
                    /*if (joueurActuel == 1 && tousBallonsEclates(ballons)==1) {

                                            }
                    else if(joueurActuel == 2 && tousBallonsEclates(ballons)==1){
                        //ouvrir ecran score

                    }*/
                }
            }
        }

        for(int i=0; i<MAX_IMAGES_BALLONS; i++){
            if (!ballons[i].eclate){
                if (i != ballonClique) { // afficher le ballon si ce n'est pas celui qui a été cliqué
                    stretch_sprite(buffer, ballons[i].image, ballons[i].x, ballons[i].y, 200, 200);
                }

                ballons[i].x += ballons[i].dx;
                ballons[i].y += ballons[i].dy;
                if (ballons[i].x <0 || ballons[i].x> SCREEN_W-100) {
                    ballons[i].dx *= -1;
                }
                if (ballons[i].y>SCREEN_H-140 || ballons[i].y<0){
                    ballons[i].dy *= -1;
                }

            }
        }

        stretch_sprite(buffer, cible, mouse_x-50, mouse_y-50, 100, 100);

        //AFFICHER LES TEMPS DES JOUEURS SUR L'ECRAN
        tempsJoueur=clock()-tempsDebut;
        if(joueur==1){
            textprintf_ex(buffer, font, SCREEN_W - 150, 10, makecol(255, 255, 255), -1, "Joueur 1: %ld", *tempsJoueur1);
            textprintf_ex(buffer, font, SCREEN_W - 150, 30, makecol(255, 255, 255), -1, "Joueur 2: %ld", *tempsJoueur2);
        }

        else if(joueur==2){
            textprintf_ex(buffer, font, SCREEN_W - 150, 10, makecol(255, 255, 255), -1, "Joueur 1: %ld", *tempsJoueur1);
            textprintf_ex(buffer, font, SCREEN_W - 150, 30, makecol(255, 255, 255), -1, "Joueur 2: %ld", *tempsJoueur2);
        }
        show_mouse(buffer);
        blit(buffer,screen,0,0,0,0,SCREEN_W,SCREEN_H);
        clear(buffer);
    }
    if(joueur==1){
        *tempsJoueur1 = tempsJoueur;
    }
    else if(joueur==2){
        *tempsJoueur2 = tempsJoueur;
    }
}

int main() {
    //initialisation allegro mis dans un sous-programme
    initialisationAllegro();
    install_keyboard();
    install_mouse();
    srand(time(NULL));
    BITMAP * buffer= create_bitmap(SCREEN_W,SCREEN_H);
    Joueur joueurs[1];
    int joueur = 1;
    partie(buffer, &tempsJoueur1, &tempsJoueur2, joueur);
    joueur=2;
    clear(buffer);
    partie(buffer, &tempsJoueur1, &tempsJoueur2, joueur);

    textprintf_ex(buffer, font, SCREEN_W / 2 -50, 10, makecol(255, 255, 255), -1, "Joueur 1: %ld s", tempsJoueur1);
    textprintf_ex(buffer, font, SCREEN_W /2 -50, 30, makecol(255, 255, 255), -1, "Joueur 2: %ld s", tempsJoueur2);

    int gagnant = determinerGagnant(tempsJoueur1, tempsJoueur2);
    if (gagnant == 1)
        textprintf_ex(buffer, font, SCREEN_W - 650, 50, makecol(255, 255, 255), -1, "Le Joueur 1 a gagné, félicitations vous remportez un ticket !!");
    else if (gagnant == 2)
        textprintf_ex(buffer, font, SCREEN_W -650, 50, makecol(255, 255, 255), -1, "Le Joueur 2 a gagné, félicitations vous remportez un ticket !!");
    else
        textprintf_ex(buffer, font, SCREEN_W - 650, 50, makecol(255, 255, 255), -1, "Match Nul !!");
    blit(buffer, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
    readkey();
    return 0;
}END_OF_MAIN();

void initialisationAllegro() {
    allegro_init();
    set_color_depth(desktop_color_depth());
    if ((set_gfx_mode(GFX_AUTODETECT_WINDOWED, 800, 600, 0, 0)) != 0) {
        allegro_message("Pb de mode graphique");
        allegro_exit();
        exit(EXIT_FAILURE);
    }
}

int collide_point_cercle(int point_x, int point_y, int cercle_x, int cercle_y, int rayon) {
    int dist_x = point_x - cercle_x;
    int dist_y = point_y - cercle_y;
    int dist_au_carre = dist_x * dist_x + dist_y * dist_y;
    return (dist_au_carre <= rayon * rayon);
}

int determinerGagnant(long tempsJoueur1, long tempsJoueur2){
    if (tempsJoueur1 < tempsJoueur2){
        return 1;
    }
    else if (tempsJoueur2 < tempsJoueur1){
        return 2;
    }
    else{
        return 0;
    }
}