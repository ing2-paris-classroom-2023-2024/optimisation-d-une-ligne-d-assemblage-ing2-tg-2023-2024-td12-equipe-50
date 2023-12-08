//
// Created by Cheikh Ahmadou Bamba Mbacké Fall on 26/11/2023.
//

#include "Graphe.h"


int main()
{
    struct Sommet operations[100];
    int nmbOp = fichierTexte(operations);

    float tmpC= getTempscycle();

    printf("%d operations recupere\n\n",nmbOp);

    while(1)
    {



        printf("\n\n __  __ ______ _   _ _    _ \n");
        printf("|  \\/  |  ____| \\ | | |  | |\n");
        printf("| \\  / | |__  |  \\| | |  | |\n");
        printf("| |\\/| |  __| | . ` | |  | |\n");
        printf("| |  | | |____| |\\  | |__| |\n");
        printf("|_|  |_|______|_| \\_|\\____/\n\n");

        printf("1- Donnees des fichier texte\n");
        printf("2- Calcules des sations\n");
        printf("3- Quitter\n");

        int choix;
        scanf("%d",&choix);

        if(choix == 1)
        {
            data(operations ,  nmbOp);
        }

        if(choix == 2)
        {
            calculeDesStations(operations ,  nmbOp,tmpC);
        }

        if(choix == 3)
        {
            return 0;
        }

    }


    return 0;
}
