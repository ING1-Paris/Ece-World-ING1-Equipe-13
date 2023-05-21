#include "Header.h"

typedef struct canard {
    int x;
    int y;
    int dx;
    int dy;
    int color;
    int orientation;
    int attrape;
} t_canard;

void melangeCanards(int i, t_canard tabCanard[10]
) {
    tabCanard[i].attrape = 0;
    tabCanard[i].x = (rand() % 900) + 150;
    tabCanard[i].y = (rand() % 550) + 50;
    tabCanard[i].orientation=rand()%256;
}



void affichePanier(int compteur, BITMAP *buffer){
    BITMAP *paniers[8];
    paniers[0]= load_bitmap("Panier.bmp",NULL);
    paniers[1]= load_bitmap("Panier1.bmp",NULL);
    paniers[2]= load_bitmap("Panier2.bmp",NULL);
    paniers[3]= load_bitmap("Panier3.bmp",NULL);
    paniers[4]= load_bitmap("Panier4.bmp",NULL);
    paniers[5]= load_bitmap("Panier5.bmp",NULL);
    paniers[6]= load_bitmap("Panier6.bmp",NULL);
    paniers[7]= load_bitmap("Panier7.bmp",NULL);

    //circlefill(buffer,55,395,50, makecol(150,150,150));
    if(compteur==0){
        stretch_sprite(buffer,paniers[0],-90,230,300,300);
    }
    if (compteur==1){
        stretch_sprite(buffer,paniers[1],-100,267,300,300);
    }
    if (compteur==2){
        stretch_sprite(buffer,paniers[2],-100,267,300,300);
    }
    if (compteur==3){
        stretch_sprite(buffer,paniers[3],-100,267,300,300);
    }
    if (compteur==4){
        stretch_sprite(buffer,paniers[4],-100,267,300,300);
    }
    if (compteur==5){
        stretch_sprite(buffer,paniers[5],-100,267,300,300);
    }
    if (compteur==6){
        stretch_sprite(buffer,paniers[6],-100,267,300,300);
    }
    if (compteur>=7){
        stretch_sprite(buffer,paniers[7],-100,267,300,300);
    }
    textprintf(buffer,font,52,410, makecol(255,255,255),"%d",compteur);
}

int partie(BITMAP *buffer, t_canard tabCanard[10]){

    int done = 0;
    int canne = 0;
    int index_canard = -1; // index du canard attrapé
    for (int i=0;i<10;i++){
        melangeCanards(i ,tabCanard);
    }
    float start=clock();
    int compteur=0;
    SAMPLE *coin,*plouf,*fall,*win ;
    BITMAP *fond= load_bitmap("fond.bmp",NULL);
    BITMAP *canard= load_bitmap("Canard.bmp",NULL);
    BITMAP *mare= load_bitmap("Mare.bmp",NULL);
    BITMAP *cannes[3];
    cannes[0]= load_bitmap("Canne1.bmp",NULL);
    cannes[1]= load_bitmap("Canne2.bmp",NULL);
    cannes[2]= load_bitmap("Canne3.bmp",NULL);
    coin= load_sample("SF-coin.wav");
    plouf= load_sample("plouf.wav");
    fall= load_sample("fall.wav");
    win= load_sample("win.wav");
    while (done == 0&& clock()-start<=60000) {
        draw_sprite(buffer,fond,0,0);
        textprintf_ex(buffer,font,0,0, makecol(255,255,255),0,"temps : %.2f s",(clock()-start)/1000);
        if (key[KEY_ESC]) {
            done = 1;
        }
        affichePanier(compteur,buffer);
        rectfill(buffer, 150, 50, 1050, 600, 255);
        draw_sprite(buffer,mare,150,50);
        for (int i = 0; i < 10; i++) {
            if (canne == 0 && index_canard == -1 &&
                (mouse_x >= tabCanard[i].x - 30 && mouse_x <= tabCanard[i].x + 30) &&
                (mouse_y >= tabCanard[i].y - 30 && mouse_y <= tabCanard[i].y + 30)) {
                tabCanard[i].color = makecol(255, 200, 0);
                if (mouse_b == 1) {
                    canne = 1;
                    play_sample(coin,255,100,1000,0);
                    tabCanard[i].color = makecol(255, 150, 0);
                    tabCanard[i].x = mouse_x;
                    tabCanard[i].y = mouse_y;
                    tabCanard[i].attrape = 1;
                    index_canard = i;
                }
                else {
                    canne = 0;
                }
            }
            else {
                canne = 0;
                tabCanard[i].color = makecol(255, 255, 0);
            }
            rotate_sprite(buffer,canard,tabCanard[i].x-45,tabCanard[i].y-45, itofix(tabCanard[i].orientation));
        }
        if (index_canard != -1) {
            tabCanard[index_canard].x = mouse_x;
            tabCanard[index_canard].y = mouse_y;
            rest(10);
            if (mouse_b == 0) {
                if(mouse_x>=150&&mouse_x<=1050&&mouse_y>=50&&mouse_y<=600){
                    play_sample(plouf,255, 128, 1000, 0);
                }
                else if(mouse_x>=0&&mouse_x<=100&&mouse_y>=350&&mouse_y<=450){
                    play_sample(win,255, 128, 1000, 0);
                    melangeCanards(index_canard,tabCanard);
                    compteur=compteur+1;
                }
                else{
                    melangeCanards(index_canard, tabCanard);
                    play_sample(fall,255, 128, 1000, 0);
                }
                tabCanard[index_canard].attrape = 0;
                index_canard = -1;
            }
        }
        stretch_sprite(buffer,cannes[0],mouse_x-200,mouse_y-100,300,500);
        show_mouse(NULL);
        blit(buffer, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
        clear(buffer);


    }
    return compteur;
}
void jeu(BITMAP *buffer, joueur *tabJoueurs){
    t_canard tabCanard[10];
    //Le premier joueur commence à jouer et on stocke son score
    int score1=partie(buffer,tabCanard);
    clear(screen);
    while(!key[KEY_SPACE]){
        textprintf_ex(screen,font,SCREEN_W/4,SCREEN_H/2, makecol(255,255,255), makecol(0,0,0),"%s a réalisé un score de %d", tabJoueurs[0].name, score1);
        textprintf_ex(screen,font,SCREEN_W/4,SCREEN_H/2+20, makecol(255,255,255), makecol(0,0,0),"C'est au tour de %s, cliquez sur espace pour commencer votre partie", tabJoueurs[1].name);

    }
    int score2= partie(buffer,tabCanard);
    clear(screen);
    textprintf_ex(screen,font,SCREEN_W/4,SCREEN_H/2, makecol(255,255,255), makecol(0,0,0),"%s a réalisé un score de %d", tabJoueurs[1].name, score2);
    rest(2000);
    clear(screen);
    if (score1 >score2){
        textprintf_ex(screen,font,SCREEN_W/4,SCREEN_H/2,makecol(255,255,255), makecol(0,0,0),"%s a gagné avec un score de %d contre %d !",tabJoueurs[0].name, score1,score2);
        tabJoueurs[0].tickets++;
        rest(2000);
    }
    if (score1 == score2){
        textprintf_ex(screen,font,SCREEN_W/4,SCREEN_H/2,makecol(255,255,255), makecol(0,0,0),"Match nul avec un score de %d partout!",score2);
        rest(2000);
    }
    if (score1 <score2){

        textprintf_ex(screen,font,SCREEN_W/4,SCREEN_H/2,makecol(255,255,255), makecol(0,0,0),"%s a gagné avec un score de %d contre %d !",tabJoueurs[1].name,score2,score1);
        tabJoueurs[1].tickets++;
        rest(2000);
    }
}
void regles(BITMAP *buffer){
    while(!key[KEY_ESC]){
        clear(buffer);
        textprintf_ex(buffer,font,SCREEN_W/2-10,0,makecol(255,255,255),0,"REGLES");
        textprintf_ex(buffer,font,20,30,makecol(255,255,255),0,"Le but du jeu est pour chaque joueur d'attraper et placer dans le panier le plus de canards en 1 minute");
        textprintf_ex(buffer,font,20,50,makecol(255,255,255),0,"Le curseur de la souris joue le rôle de canne à pêche.");
        textprintf_ex(buffer,font,20,70,makecol(255,255,255),0,"Le joueur ayant le plus grand score remporte un ticket.");
        textprintf_ex(buffer,font,20,90,makecol(255,255,255),0,"Vous pouvez quitter le jeu à tout moment en cliquant sur Echap.");
        show_mouse(buffer);
        blit(buffer,screen,0,0,0,0,SCREEN_W,SCREEN_H);
    }
    rest(800);
}

void menu(BITMAP *buffer,joueur *tabJoueurs){
    BITMAP  *imageMenu= load_bitmap("Menu.bmp",NULL);
    BITMAP * bulles =load_sample("bulles.wav");
    while(!key[KEY_ESC]){
        draw_sprite(buffer,imageMenu,0,0);
        if(mouse_x>=530 && mouse_x<=710 && mouse_y>=300 && mouse_y<=375 && mouse_b==1){
            play_sample(bulles,255,100,1000,0);
            rest(1000);
            jeu(buffer, tabJoueurs);
            break;
        }
        if(mouse_x>=530 && mouse_x<=710 && mouse_y>=390 && mouse_y<=450 && mouse_b==1){
            play_sample(bulles,255,100,1000,0);
            rest(1000);
            regles(buffer);
        }
        show_mouse(buffer);
        blit(buffer,screen,0,0,0,0,SCREEN_W,SCREEN_H);
        clear(buffer);
    }
}
void jeu_canards(BITMAP *buffer,joueur *tabJoueurs) {
    menu(buffer,tabJoueurs);
}

