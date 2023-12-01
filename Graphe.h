//
// Created by cleme on 01/12/2023.
//
#ifndef GRAPHE_H_INCLUDED
#define GRAPHE_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>


/* Structure d'un arc*/
struct Arc
{
    int sommet; // numéro de sommet d'un arc adjacent au sommet initial
    int valeur;
    struct Arc* arc_suivant;
};

/* Alias de pointeur sur un Arc */
typedef struct Arc* pArc;

/* Structure d'un sommet*/
struct Sommet
{
    struct Arc* arc;
    int valeur;
    char couleur;
    int tempsExecutionSeconde;
    int tempsExecutionCentiSeconde;
    int marquageFauxSommet;
    int premierPredecesseur;
    int secondPredecesseur;
    int sommetOrdre;
};

/* Alias de pointeur sur un Sommet */
typedef struct Sommet* pSommet;

/* Alias d'un Graphe */
typedef struct Graphe
{
    int taille;
    int ordre;
    int min;
    int max;
    int decalage;
    int debut;
    pSommet* pSommet;
} t_Graphe;

typedef struct maillon
{
    int numero_sommet;
    struct maillon* suivant;
} t_maillon;

typedef struct file
{
    int nb_maillon;
    t_maillon* premier;
    t_maillon* dernier;
} t_file;

/* affichage des successeurs du sommet num*/
void afficherSuccesseurs(pSommet * sommet, int numero, int numerobis,int valeur);

// créer le graphe
t_Graphe* CreerGraphe(int ordre,int decallage);

/* La construction du réseau peut se faire à partir d'un fichier dont le nom est passé en paramètre
Le fichier contient : ordre, taille,orientation (0 ou 1)et liste des arcs */
t_Graphe * lire_graphe(char * nomFichier, int conx);

// Ajouter l'arête entre les sommets s1 et s2 du graphe
pSommet* CreerArete(pSommet* sommet,int s1,int s2,int decallage);

/*affichage du graphe avec les successeurs de chaque sommet */
void graphe_afficher(t_Graphe* graphe);
t_Graphe* analyse_fichier(FILE* fic, int taille);

t_file* init_file ();
int file_vide (t_file *file);
t_file* enfiler (t_file *file,pSommet s);
int defiler (t_file *file);
void bfs (t_Graphe *graphe,int sommet);
void recupSommetDFS(t_Graphe *graphe, int valeur);
void dfs (t_Graphe *graphe,pSommet premier);
void recherche_connexe(t_Graphe *graphe);
void tempsDeCycle(t_Graphe*graphe);

#endif // GRAPHE_H_INCLUDED
