//
// Created by cleme on 01/12/2023.
//
#include "Graphe.h"

//affichage des successeurs du sommet num
void afficherSuccesseurs(pSommet * sommet, int numero, int numerobis,int valeur) {
    printf(" sommet %d : %d\n", numerobis + 1, valeur);
    pArc arc = sommet[numero]->arc;
    while (arc != NULL) {
        if (sommet[arc->sommet - 1]->marquageFauxSommet == 0) {
            arc->sommet++;
        }
        printf("%d ", arc->sommet);
        arc = arc->arc_suivant;
    }
}

// Ajouter l'arête entre les sommets s1 et s2 du graphe
pSommet* CreerArete(pSommet* sommet,int s1,int s2,int decallage)
{

    if(sommet[s1-decallage]->arc==NULL)
    {
        pArc Newarc=(pArc)malloc(sizeof(struct Arc));
        Newarc->sommet=s2;
        Newarc->arc_suivant=NULL;
        sommet[s1-decallage]->arc=Newarc;
        return sommet;
    }

    else
    {
        pArc temp=sommet[s1-decallage]->arc;
        while( !(temp->arc_suivant==NULL))
        {
            temp=temp->arc_suivant;
        }
        pArc Newarc=(pArc)malloc(sizeof(struct Arc));
        Newarc->sommet=s2;
        Newarc->arc_suivant=NULL;

        if(temp->sommet>s2)
        {
            Newarc->arc_suivant=temp->arc_suivant;
            Newarc->sommet=temp->sommet;
            temp->sommet=s2;
            temp->arc_suivant=Newarc;
            return sommet;
        }

        temp->arc_suivant=Newarc;
        return sommet;
    }
}

// créer le graphe
t_Graphe* CreerGraphe(int ordre,int decallage)
{
    t_Graphe * Newgraphe=(t_Graphe*)malloc(sizeof(t_Graphe));
    Newgraphe->pSommet = (pSommet*)malloc(ordre*sizeof(pSommet));

    for(int i=0; i<ordre; i++)
    {
        Newgraphe->pSommet[i]=(pSommet)malloc(sizeof(struct Sommet));
        Newgraphe->pSommet[i]->valeur=i+decallage;
        Newgraphe->pSommet[i]->arc=NULL;
    }
    return Newgraphe;
}

t_Graphe* analyse_fichier(FILE* fic, int taille)
{
    int min,max,decalage,ordre,s1,s2;
    t_Graphe* graph;

    fscanf(fic,"%d%d",&s1,&s2);
    if (s1<s2)
    {
        min=s1;
        max=s2;
    }
    else
    {
        min=s2;
        max=s1;
    }
    // créer les arêtes du graphe
    for(int i=1; i<taille; i++)
    {
        fscanf(fic,"%d%d",&s1,&s2);
        if (s1<min)
        {
            min=s1;
        }
        if (s1>max)
        {
            max=s1;
        }
        if (s2<min)
        {
            min=s2;
        }
        if (s2>max)
        {
            max=s2;
        }
    }
    ordre = max;
    decalage=max-min;
    if (decalage==ordre-1)
    {
        decalage=min;
    }
    else {
        if (decalage < ordre - 1) {
            decalage = 0;
            max=(min+ordre)-1;
        } else {
            printf("erreur contenu fichier .txt");
            fclose(fic);
            exit(EXIT_FAILURE);
        }
    }
    graph=CreerGraphe(ordre,decalage); // correction simple bug de CreerArete()
    graph->min=min;
    graph->max=max;
    graph->decalage=decalage;
    graph->ordre=ordre;
    graph->taille=taille;
    return graph;
}



/* La construction du réseau peut se faire à partir d'un fichier dont le nom est passé en paramètre
Le fichier contient : ordre, taille,orientation (0 ou 1)et liste des arcs */
t_Graphe * lire_graphe(char * nomFichier,int conx)
{
    t_Graphe* graphe;
    FILE *fptr1, *fptr2;
    char filename2[] = "operations.txt";
    int taille=0;
    int s1, s2, sommet, temps, tempsbis;
    char point;

    fptr1 = fopen(nomFichier,"r");
    fptr2 = fopen(filename2, "r");
    int ch;

    while ((ch = fgetc(fptr1)) != EOF) {
        if (ch == '\n') {
            taille++;
        }
    }
    taille++;
    fclose(fptr1);

    fptr1 = fopen(nomFichier, "r");

    if (fptr1 == NULL || fptr2 == NULL) {
        printf("Error!");
        exit(-1);
    }
    if (!fptr1)
    {
        printf("Erreur de lecture fichier\n");
        exit(-1);
    }
    graphe=analyse_fichier(fptr1, taille);
    fclose(fptr1);

    fptr1 = fopen(nomFichier,"r");

    int trouverSommet[graphe->ordre];
    int trouverSommetBis[graphe->ordre];
    // créer les arêtes du graphe
    for (int i=0; i<taille; ++i)
    {
        fscanf(fptr1,"%d%d",&s1,&s2);
        graphe->pSommet=CreerArete(graphe->pSommet, s1, s2,graphe->decalage);
        trouverSommet[i]=s1;
        trouverSommetBis[i]=s2;
    }
    fclose(fptr1);
    for(int i=0; i<graphe->ordre;i++){
        graphe->pSommet[i]->marquageFauxSommet = 0;
    }
    for(int i=0; i<graphe->ordre; i++){
        for(int j=0; j<taille; j++){
            if(i+1==trouverSommet[j]){
                graphe->pSommet[i]->marquageFauxSommet = 1;
            }
            if(i+1==trouverSommetBis[j]){
                graphe->pSommet[i]->marquageFauxSommet = 1;
            }
        }
    }
    for (int i = 0; i < graphe->ordre; i++) {
        if (graphe->pSommet[i]->marquageFauxSommet == 0) {
            i++;
        }
        fscanf(fptr2, "%d%d%c%d", &sommet, &temps, &point, &tempsbis);
        graphe->pSommet[i]->tempsExecutionSeconde = temps;
        graphe->pSommet[i]->tempsExecutionCentiSeconde = tempsbis;
        if(graphe->pSommet[i]->tempsExecutionCentiSeconde < 10){
            printf("%d pour %d.0%d\n", i+1, graphe->pSommet[i]->tempsExecutionSeconde, graphe->pSommet[i]->tempsExecutionCentiSeconde);
        }
        else {
            printf("%d pour %d.%d\n", i+1, graphe->pSommet[i]->tempsExecutionSeconde, graphe->pSommet[i]->tempsExecutionCentiSeconde);
        }
    }
    fclose(fptr2);
    for(int i=0; i<graphe->ordre; i++){
        if(graphe->pSommet[i]->marquageFauxSommet==0) {
            i++;
        }
        graphe->pSommet[i]->premierPredecesseur=0;
        graphe->pSommet[i]->secondPredecesseur=0;
    }
    for(int i=0; i<graphe->ordre; i++) {
        for (int j = 0; j < taille; j++) {
            if (graphe->pSommet[i]->marquageFauxSommet == 0) {
                i++;
            }
            if(i+1==trouverSommetBis[j]) {
                if (graphe->pSommet[i]->premierPredecesseur != 0) {
                    graphe->pSommet[i]->secondPredecesseur = trouverSommet[j];
                } else {
                    graphe->pSommet[i]->premierPredecesseur = trouverSommet[j];
                }
            }
        }
    }
    return graphe;
}
/*affichage du graphe avec les successeurs de chaque sommet */
void graphe_afficher(t_Graphe* graphe)
{
    int itteration=0;
    printf("graphe\n");

    printf("listes d'adjacence :\n");

    for (int i=0; i<graphe->ordre; i++)
    {
        if(graphe->pSommet[i]->marquageFauxSommet==0){
            i++;
            itteration--;
        }
        afficherSuccesseurs(graphe->pSommet, i, i+itteration,graphe->min+i);
        printf("\n");
    }

    printf("ce graphe a pour ordre = %d\n\n",graphe->ordre+itteration);

}

t_file* init_file ()
{
    t_file *file=(t_file*)malloc(sizeof(t_file));
    file->premier = NULL;
    file->dernier =NULL;
    file->nb_maillon=0;
    return file;
}

/* verification si file vide */
int file_vide (t_file *file)
{
    if (file->nb_maillon ==0)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

/* ajout dans file */
t_file* enfiler (t_file *file,pSommet s)
{
    t_maillon * temp;
    temp= malloc(sizeof (t_maillon*));
    if (temp==NULL)
    {
        printf("problème d'allocation mémoire\n");
        exit(EXIT_FAILURE);
    }

    temp->numero_sommet =s->valeur;

    if (file_vide(file)==0)
    {
        file->premier=temp;
        file->dernier=temp;
    }
    else
    {
        file->dernier->suivant=temp;
        file->dernier=temp;
    }
    file->nb_maillon = file->nb_maillon+1;
    return file;
}


/* retire un element de la file*/

int defiler (t_file *file) {
    t_maillon *temp;
    int temp2;
    temp = file->premier;
    temp2 =temp->numero_sommet;
    if (file_vide(file) == 0) {
        printf("rien à défiler\n");
        exit(1);
    } else {
        if (file->premier == file->dernier) {
            file->premier = NULL;
            file->dernier = NULL;
        } else {
            file->premier = file->premier->suivant;
        }
        file->nb_maillon = file->nb_maillon - 1;
        free(temp);
        return temp2;
    }
}



/* BFS */
void bfs(t_Graphe *graphe, int sommet) {
    // couleur blanc=-1 gris= 0 Noir = 1
    t_file *file;
    pSommet premier;
    pSommet temp2;
    pArc temp3;
    int temp = 0, i;

    /* Initialisation */
    // ordre + 2 pour correction bug CreerArete
    for (i = 0; i < graphe->ordre; i++) {
        if (graphe->pSommet[i]->marquageFauxSommet == 0) {
            i++;
        }
        graphe->pSommet[i]->couleur = 'B';
    }

    int iteration = 0;
    premier = graphe->pSommet[sommet - graphe->decalage];
    file = init_file();
    file = enfiler(file, premier);
    printf("[%d]", sommet);
    premier->couleur = 'N';

    while (file_vide(file) != 0 && iteration<16) {
        temp = defiler(file);
        if (graphe->pSommet[temp - graphe->decalage]->couleur == 'B') {
            printf("->[%d]", temp);
            graphe->pSommet[temp - graphe->decalage]->couleur = 'N';
            iteration++;
        }
        temp3 = graphe->pSommet[temp - graphe->decalage]->arc;
        while (temp3 != NULL) {
            temp2 = graphe->pSommet[temp3->sommet - graphe->decalage];
            if (temp2->couleur == 'B') {
                file = enfiler(file, temp2);
            }
            temp3 = temp3->arc_suivant;
        }
    }
    printf("\n\n");
}

void recupSommetDFS(t_Graphe *graphe, int valeur){
    if(graphe->pSommet[graphe->debut]->marquageFauxSommet==0){
        graphe->debut++;
    }
    graphe->pSommet[graphe->debut]->sommetOrdre=valeur;
    graphe->debut++;
}

/*DFS*/
void dfs (t_Graphe *graph,pSommet premier)
{
    /*initialisation */
    /* programme*/
    pSommet tempSommet;
    pArc tempArc;
    if(premier->couleur=='B') {
        tempArc = premier->arc;
        printf("[%d]", premier->valeur);
        recupSommetDFS(graph, premier->valeur);
        premier->couleur = 'N';
        while (tempArc != NULL) {
            tempSommet = graph->pSommet[tempArc->sommet-graph->decalage];
            if(tempSommet->couleur=='B') {
                printf("->");
            }
            dfs(graph, tempSommet);
            tempArc = tempArc->arc_suivant;
        }
    }
}

void recherche_connexe(t_Graphe *graphe)
{
    int i,j,test,index;
    test =0;
    j=1;
    graphe->debut=0;
    while(test==0)
    {
        test=1;
        for(i=0;i<graphe->ordre;i++)
        {
            if(graphe->pSommet[i]->marquageFauxSommet==0){
                i++;
            }
            if(graphe->pSommet[i]->couleur=='B')
            {
                test=0;
                index=i;
                i=graphe->ordre;
            }
        }
        if(test==0)
        {
            printf("composante connexe %d :",j);
            dfs(graphe, graphe->pSommet[index]);
            printf("\n");
            j=j+1;
        }
    }
}

void tempsDeCycle(t_Graphe*graphe){
    int temps=0;
    int tempsbis=0;
    int iteration=0;
    int sommetIt[iteration];
    int tmp[graphe->ordre];
    int station=1;
    int j=0;
    int it=0;
    int tempsDeCycle=200;
    int tempsDeCycleSeconde=2;
    for(int i=0; i<graphe->ordre; i++) {
        if (graphe->pSommet[i]->marquageFauxSommet == 0) {
            i++;
            iteration++;
        }
        if(graphe->pSommet[i]->tempsExecutionSeconde==0){
            tmp[i]=graphe->pSommet[i]->tempsExecutionCentiSeconde;
        }
        else{
            tmp[i]=100+graphe->pSommet[i]->tempsExecutionCentiSeconde;
        }
        printf("%d ", tmp[i]);
    }
    printf("\n");
    for(int i=0; i<graphe->ordre;i++){
        if (graphe->pSommet[i]->marquageFauxSommet == 0) {
            i++;
        }
        if(temps==0){
            printf("station n %d : contient les operations ", station);
        }
        tempsbis=temps+tmp[graphe->pSommet[i]->sommetOrdre-1-j];
        if (tempsbis>tempsDeCycle){
            temps = temps-100;
            printf("qui prend un temps de %d.%d\n", tempsDeCycleSeconde-1,temps);
            station++;
            temps=0;
        }
        if(graphe->pSommet[graphe->pSommet[i]->sommetOrdre-1]->secondPredecesseur==0 && sommetIt[j]!=graphe->pSommet[i]->sommetOrdre && tempsbis<tempsDeCycle){
            temps=temps+tmp[graphe->pSommet[i]->sommetOrdre-1];
            printf("%d,",graphe->pSommet[i]->sommetOrdre);
        }
        else if(graphe->pSommet[graphe->pSommet[i]->sommetOrdre-1]->secondPredecesseur!=0 && it==0) {
            if(temps==0 && it==0){
                printf("station n %d : contient les operations ", station);
            }
            temps=temps+tmp[graphe->pSommet[graphe->pSommet[i]->sommetOrdre-1]->secondPredecesseur-1-j];
            printf("%d,", graphe->pSommet[graphe->pSommet[i]->sommetOrdre-1]->secondPredecesseur);
            temps=temps+tmp[graphe->pSommet[i]->sommetOrdre-1];
            printf("%d,",graphe->pSommet[i]->sommetOrdre);
            sommetIt[j]=graphe->pSommet[i]->sommetOrdre;
            j++;
            it=1;
        }
        tempsbis=0;
    }
}
