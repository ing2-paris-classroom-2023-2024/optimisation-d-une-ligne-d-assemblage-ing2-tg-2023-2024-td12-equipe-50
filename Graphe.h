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
    int couleur;
    float tempsExecutionSeconde;
    int tempsExecutionCentiSeconde;
    int marquageFauxSommet;
    int premierPredecesseur;
    int secondPredecesseur;
    int troisPredecesseur;
    int sommetOrdre;
    int exclusions[20];
    int nmbE;

};



/* Alias de pointeur sur un Sommet */
typedef struct Sommet* pSommet;

struct Station
{
    int nmbOp;
    int Op[100];
    float temps;

};

struct Sommet creerSommet(int _val)
{
    struct Sommet newSommet;
    newSommet.couleur = 0;
    newSommet.valeur = _val;
    newSommet.premierPredecesseur =0;
    newSommet.secondPredecesseur = 0;
    newSommet.troisPredecesseur = 0;
    newSommet.sommetOrdre = 0;
    newSommet.tempsExecutionSeconde = 0;
    newSommet.nmbE = 0;

    return newSommet;
}


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

struct Sommet creerSommet(int _val);

void afficherSuccesseurs(pSommet * sommet, int numero, int numerobis,int valeur);

pSommet* CreerArete(pSommet* sommet,int s1,int s2,int decallage);

t_Graphe* CreerGraphe(int ordre,int decallage);
t_Graphe* analyse_fichier(FILE* fic, int taille);

/* La construction du réseau peut se faire à partir d'un fichier dont le nom est passé en paramètre
Le fichier contient : ordre, taille,orientation (0 ou 1)et liste des arcs */
t_Graphe * lire_graphe(char * nomFichier,int conx);

void graphe_afficher(t_Graphe* graphe);

t_file* init_file ();
int file_vide (t_file *file);
t_file* enfiler (t_file *file,pSommet s);
int defiler (t_file *file);
void bfs(t_Graphe *graphe, int sommet);

void recupSommetDFS(t_Graphe *graphe, int valeur);
void dfs (t_Graphe *graph,pSommet premier);
void recherche_connexe(t_Graphe *graphe);
void tempsDeCycle(t_Graphe*graphe);
int FindSommet(struct Sommet operations[100],int val,int nmbOp);
void addExclusions(struct Sommet *S1 , struct Sommet *S2 );
void addPrececedent(struct Sommet *S1 , struct Sommet *S2);
int fichierTexte(struct Sommet operations[100]);
void afficheStation(int nmb , struct Station assemblage[15]);
float getTempscycle();
int isPlaced(int i , struct Sommet operations[100] , int nmb , int placed[100]);
void CreerStation(struct Sommet operations[100] , int nmb, int contraintes , int order[100] , float tempsC);
void Algo2(struct Sommet operations[100] , int nmb, float tempsC);
void Algo4(struct Sommet operations[100] , int nmb,float tempsC);
void Algo5(struct Sommet operations[100] , int nmb,float tempsC);
void calculeDesStations(struct Sommet operations[100] , int nmb ,float tmpC);
void data(struct Sommet operations[100] , int nmb);


#endif // GRAPHE_H_INCLUDED
