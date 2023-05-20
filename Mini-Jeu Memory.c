#include "Header.h"

typedef struct Cartes {
    int x;
    int y;
    int figure;
    int piochee;
    int gagnee;
} carte;

void melangerCartes(int* tableau, int taille) {
    for (int i = taille - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int temp = tableau[i];
        tableau[i] = tableau[j];
        tableau[j] = temp;
    }
}

int partieMemory(BITMAP* buffer) {
    BITMAP *avantcarte= load_bitmap("Avantcarte.bmp",NULL);
    BITMAP *doscarte= load_bitmap("Doscarte.bmp",NULL);
    int debut = clock();
    int score = 0;
    int cartePiochee1 = -1;
    int cartePiochee2 = -1;
    carte tabCartes[24];

    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 4; j++) {
            tabCartes[i * 4 + j].x = i * 125 + 300;
            tabCartes[i * 4 + j].y = j * 156 + 25;
        }
    }

    int tabChiffres[] = {0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7, 8, 8, 9, 9, 10, 10, 11, 11};
    melangerCartes(tabChiffres, 24);

    //On initialise les valeurs
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 4; j++) {
            tabCartes[i * 4 + j].x = i * 125 + 300;
            tabCartes[i * 4 + j].y = j * 156 + 25;
            tabCartes[i * 4 + j].figure = tabChiffres[i * 4 + j];
            tabCartes[i * 4 + j].piochee = 0;
            tabCartes[i*4+j].gagnee=0;
        }
    }
    //on rentre dans la boucle de jeu qui s'arrête si on clique sur espace où lorsque le memory est résolu;
    while (!key[KEY_ESC] && score != 12) {
        //on affiche l'ensemble des cartes face cachées
        for (int i = 0; i < 24; i++) {
            //rectfill(buffer, tabCartes[i].x, tabCartes[i].y, tabCartes[i].x + 100, tabCartes[i].y + 100, makecol(255, 255, 255));
            draw_sprite(buffer,doscarte,tabCartes[i].x, tabCartes[i].y);
        }
        //on regarde si une carte est piochée (clic)
        for (int i = 0; i < 24; i++) {
            if (mouse_x >= tabCartes[i].x && mouse_x <= tabCartes[i].x + 100 && mouse_y >= tabCartes[i].y && mouse_y <= tabCartes[i].y + 100) {
                if (mouse_b == 1) {
                    //si la premiere carte n'a aps été piochée :elle devient alors la premiere carte piochée et on marque qu'elle est piochée
                    if (cartePiochee1 == -1) {
                        cartePiochee1 = i;
                        tabCartes[cartePiochee1].piochee = 1;
                        //si la deuxieme carte n'a pas été piochée et que ce n'est pas la premiere carte: elle devient alors la deuxieme carte piochee et on marque qu'elle est piochée
                    } else if (cartePiochee2 == -1 && cartePiochee1 != i) {
                        cartePiochee2 = i;
                        tabCartes[cartePiochee2].piochee = 1;
                    }
                }
            }
        }

        //on regarde carte si elle a été piochée ou gagnée, si oui on l'affiche de face sur le plateau
        for (int i = 0; i < 24; i++) {
            if (tabCartes[i].piochee == 1 || tabCartes[i].gagnee == 1) {
                draw_sprite(buffer,avantcarte,tabCartes[i].x,tabCartes[i].y);
                textprintf_ex(buffer, font, tabCartes[i].x + 50, tabCartes[i].y + 50, 0, makecol(255, 255, 255), "%d", tabCartes[i].figure);
                //rest(500);
            }
        }

        //si les deux cartes ont été piochées
        if (cartePiochee1 != -1 && cartePiochee2 != -1) {
            if (tabCartes[cartePiochee1].figure == tabCartes[cartePiochee2].figure) {
                score++;
                tabCartes[cartePiochee1].gagnee = 1;
                tabCartes[cartePiochee2].gagnee = 1;
                cartePiochee1 = -1;
                cartePiochee2 = -1;
            } else {
                tabCartes[cartePiochee1].piochee = 0;
                tabCartes[cartePiochee2].piochee = 0;
                cartePiochee1 = -1;
                cartePiochee2 = -1;
            }
        }

        textprintf_ex(buffer, font, 0, 0, makecol(255, 255, 255), 0, "temps: %ld", clock() - debut);
        textprintf_ex(buffer, font, 0, 20, makecol(255, 255, 255), 0, "score: %d", score);
        show_mouse(buffer);
        blit(buffer, screen, 0, 0, 0, 0, LONG, LARG);
        clear(buffer);
    }

    return clock() - debut;
}

int jeuMemory( BITMAP *buffer, joueur *tabJoueurs) {
    int temps1 = partieMemory(buffer);
    temps1=tabJoueurs[0].performance_memory;
    clear(screen);
    while(!key[KEY_SPACE]){
        textprintf_ex(screen,font,LONG/2,LARG/2,makecol(255,255,255),0,"%s a fait un temps de %d",tabJoueurs[0].name,temps1);
        textprintf_ex(screen,font,LONG/2,LARG/2+20,makecol(255,255,255),0,"C'est au tour de %s. Tapez Espace pour commencer",tabJoueurs[1].name);

    }
    int temps2= partieMemory(buffer);
    temps2=tabJoueurs[1].performance_memory;
    textprintf_ex(screen,font,LONG/2,LARG/2,makecol(255,255,255),0,"%s a fait un temps de %d",tabJoueurs[1].name,temps2);
    rest(5000);
}


