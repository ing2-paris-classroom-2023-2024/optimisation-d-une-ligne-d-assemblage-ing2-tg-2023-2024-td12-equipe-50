//
// Created by Cheikh Ahmadou Bamba Mbacké Fall on 26/11/2023.
//

#include "Graphe.h"

int main()
{
    t_Graphe * g, *g2;
    int first,i;
    char nom_fichier[50];

    printf("entrer le nom du fichier du graphe:");
    gets(nom_fichier);
    g = lire_graphe(nom_fichier,0);


    /// afficher le graphe
    graphe_afficher(g);

    /*Reinitialisation */
    // ordre +2 pour correction bug CreerArete
    for(i=0;i<g->ordre;i++)
    {
        if(g->pSommet[i]->marquageFauxSommet==0){
            i++;
        }
        g->pSommet[i]->couleur=i;
    }

    // Recherche composantes connexes
    printf("\nListe composantes connexes\n");

    // reconstruction du graphe si jamais oriente
    g2 = lire_graphe(nom_fichier,1);

    /*Reinitialisation */
    // ordre +2 pour correction bug CreerArete
    for(i=0;i<g2->ordre;i++)
    {
        g2->pSommet[i]->couleur='B';
    }
    recherche_connexe(g2);

    tempsDeCycle(g2);

    return 0;
}