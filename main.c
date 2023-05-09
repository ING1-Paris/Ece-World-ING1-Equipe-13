#include "Header.h"

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
    //À mettre dans le menu : joueur Joueur[NB_JOUEUR]; //Création de la structure joueur

    allegro_exit();
    return 0;
}
END_OF_MAIN()