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

// A8T5jouter l'arête entre les sommets s1 et s2 du graphe
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
    int ordre=0;
    int iteration=0;
    int sommetIt[iteration];
    int tmp[graphe->ordre];
    int station=1;
    int j=0;
    int tempsDeCycle=2;
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
    while(temps<tempsDeCycle && ordre<graphe->ordre){
        if (graphe->pSommet[ordre]->marquageFauxSommet == 0) {
            ordre++;
        }
        if(temps==0){
            printf("station n %d : contient les operations ", station);
        }
        if(graphe->pSommet[graphe->pSommet[ordre]->sommetOrdre]->secondPredecesseur==0 && sommetIt[j]!=graphe->pSommet[ordre]->sommetOrdre){
            temps=temps+tmp[graphe->pSommet[ordre]->sommetOrdre];
            printf("%d,",graphe->pSommet[ordre]->sommetOrdre);
            ordre++;
        }
        else if(graphe->pSommet[graphe->pSommet[ordre]->sommetOrdre]->secondPredecesseur!=0) {
            temps=temps+tmp[graphe->pSommet[graphe->pSommet[ordre]->sommetOrdre]->secondPredecesseur];
            printf("%d,", graphe->pSommet[graphe->pSommet[ordre]->sommetOrdre]->secondPredecesseur);
            ordre++;
            sommetIt[j]=graphe->pSommet[ordre]->sommetOrdre;
            j++;
        }
        if ((temps+tmp[graphe->pSommet[graphe->pSommet[ordre]->sommetOrdre]->sommetOrdre])>tempsDeCycle){
            temps=0;
            printf("qui prend un temps de %d.%d\n", graphe->pSommet[graphe->pSommet[ordre]->sommetOrdre]->tempsExecutionSeconde, graphe->pSommet[graphe->pSommet[ordre]->sommetOrdre]->tempsExecutionCentiSeconde);
            station++;
        }
    }
}

int FindSommet(struct Sommet operations[100],int val,int nmbOp)
{
    int sommetTrouve = -1;
    for(int i = 0 ; i < nmbOp ; i++)
    {
        if(operations[i].valeur == val)
            sommetTrouve = i;
    }

    return sommetTrouve;
}

void addExclusions(struct Sommet *S1 , struct Sommet *S2 )
{
    S1->exclusions[S1->nmbE] = S2->valeur;
    S1->nmbE++;

    S2->exclusions[S2->nmbE] = S1->valeur;
    S2->nmbE++;

}

void addPrececedent(struct Sommet *S1 , struct Sommet *S2)
{

    if(S2->premierPredecesseur == 0)
    {
        S2->premierPredecesseur = S1->valeur;
    }
    else if(S2->secondPredecesseur == 0)
        S2->secondPredecesseur = S1->valeur;
    else if(S2->troisPredecesseur == 0)
        S2->troisPredecesseur = S1->valeur;
}

int fichierTexte(struct Sommet operations[100])
{

    int nmbOp = 0;

    FILE* fichier = fopen("operations.txt", "r");

    if (!fichier)
        fprintf(stderr, "Impossible d'ouvrir operatons.txt");


    int sommet;
    float temps;

    while (fscanf(fichier, "%d %f", &sommet, &temps) != EOF)
    {
        int act = FindSommet(operations,sommet,nmbOp);
        if(act == -1)
        {
            operations[nmbOp] = creerSommet(sommet);
            operations[nmbOp].tempsExecutionSeconde = temps;
            nmbOp ++;
        }
        else
        {
            operations[act].tempsExecutionSeconde = temps;
        }
    }


    fclose(fichier);

    fichier = fopen("exclusions.txt", "r");

    if (!fichier)
        fprintf(stderr, "Impossible d'ouvrir exclusions.txt");


    int sommet1;
    int sommet2;

    while (fscanf(fichier, "%d %d", &sommet1, &sommet2) != EOF)
    {
        int act1 = FindSommet(operations,sommet1,nmbOp);
        int act2 = FindSommet(operations,sommet2,nmbOp);





        if(act1 == -1)
        {
            operations[nmbOp] = creerSommet(sommet1);
            act1 = nmbOp;
            nmbOp ++;
            printf("\naddTime: %d",sommet1);
        }
        if(act2 == -1)
        {
            operations[nmbOp] = creerSommet(sommet2);
            act2 = nmbOp;
            nmbOp ++;
        }

        addExclusions(&operations[act1],&operations[act2]);

    }


    fclose(fichier);

    fichier = fopen("precedences.txt", "r");

    if (!fichier)
        fprintf(stderr, "Impossible d'ouvrir prec.txt");


    while (fscanf(fichier, "%d %d", &sommet1, &sommet2) != EOF)
    {
        int act1 = FindSommet(operations,sommet1,nmbOp);
        int act2 = FindSommet(operations,sommet2,nmbOp);

        if(act1 == -1)
        {
            operations[nmbOp] = creerSommet(sommet1);
            act1 = nmbOp;
            nmbOp ++;
        }
        if(act2 == -1)
        {
            operations[nmbOp] = creerSommet(sommet2);
            act2 = nmbOp;
            nmbOp ++;
        }

        addPrececedent(&operations[act1],&operations[act2]);

    }


    fclose(fichier);

    return nmbOp;

}

void afficheStation(int nmb , struct Station assemblage[15])
{
    for(int i = 0 ; i < nmb ; i++)
    {
        printf("\n\nStation %d : ",i);
        for(int j = 0 ; j < assemblage[i].nmbOp ; j++)
        {
            printf("Op%d ",assemblage[i].Op[j]);
        }
        printf("\ntemps total: %f",assemblage[i].temps);
    }
}

float getTempscycle()
{
    const char *nomFichier = "temps_cycle.txt";
    FILE *fichier = fopen(nomFichier, "r");

    if (fichier == NULL)
    {
        fprintf(stderr, "Impossible d'ouvrir le fichier %s.\n", nomFichier);
        exit(EXIT_FAILURE);
    }

    float nombre;
    fscanf(fichier, "%f", &nombre);


    fclose(fichier);
    return nombre;
}
int isPlaced(int i , struct Sommet operations[100] , int nmb , int placed[100])
{
    int sortie = 0;

    int nmb2;
    nmb2 = 0;
    int cmp;
    cmp = 0;



    int p1 = operations[i].premierPredecesseur;
    if(p1 != 0)
    {
        nmb2++;
        int k = FindSommet(operations,p1,nmb);
        if(placed[k] == 1)
            cmp++;
    }

    int p2 = operations[i].secondPredecesseur;
    if(p2 != 0)
    {
        nmb2++;
        int k = FindSommet(operations,p2,nmb);
        if(placed[k] == 1)
            cmp++;
    }

    int p3 = operations[i].troisPredecesseur;
    if(p3 != 0)
    {
        nmb2++;
        int k = FindSommet(operations,p3,nmb);
        if(placed[k] == 1)
            cmp++;
    }



    if(nmb2 == cmp )
        return 1;
    else
        return 0;


}


void CreerStation(struct Sommet operations[100] , int nmb, int contraintes , int order[100] , float tempsC)
{

    struct Station assemblage[15];
    for(int i = 0 ;  i < 15  ; i++)
    {
        assemblage[i].temps = 0;
        assemblage[i].nmbOp = 0;
    }

    int placed[100];
    for(int i = 0 ; i < 100 ; i++)
        placed[i] = 0;
    int nmbStation = 0;
    int cmp = 0;


    while(cmp < nmb)
    {
        for(int i = 0 ; i < nmb ; i++)
        {
            int act = i;
            if(contraintes == 2)
                act = order[i];

            if(placed[act] == 0)
            {
                int good = 1;
                if(contraintes == 1 || contraintes == 3 || contraintes == 4  || contraintes == 5)
                {
                    if(assemblage[nmbStation].nmbOp > 0)
                    {
                        int t = FindSommet(operations,assemblage[nmbStation].Op[0],nmb);
                        if(operations[t].couleur != operations[act].couleur)
                        {
                            good = 0;
                        }
                    }
                }
                if(contraintes == 2  || contraintes == 4  || contraintes == 5)
                {
                    if(isPlaced(act,operations,nmb,placed) == 0)
                        good = 0;
                }

                if(contraintes == 2  || contraintes == 3 || contraintes == 5)
                {
                    if(assemblage[nmbStation].temps + operations[act].tempsExecutionSeconde > tempsC)
                        good = 0;
                }

                if(good == 1)
                {
                    assemblage[nmbStation].Op[assemblage[nmbStation].nmbOp] = operations[act].valeur;
                    assemblage[nmbStation].temps += operations[act].tempsExecutionSeconde;
                    assemblage[nmbStation].nmbOp++;
                    cmp++;
                    placed[act] = 1;

                }
            }
        }
        nmbStation++;
    }

    afficheStation(nmbStation,assemblage);

}

void Algo2(struct Sommet operations[100] , int nmb, float tempsC)
{
    int order[100];

    for(int i = 0 ; i < nmb ; i++)
        order[i] = 0;

    int placed[100];

    for(int i = 0 ; i < 100 ; i++)
        placed[i] = 0;

    int cmp = 0;


    while( cmp != nmb)
    {
        for(int i = 0 ; i < nmb ; i++)
        {
            if(placed[i] == 0)
            {
                //printf("\ncmp: %d",cmp);

                int c = isPlaced(i,operations,nmb,placed);
                if(c == 1 )
                {
                    placed[i] = 1;
                    order[cmp] = i;
                    cmp++;
                }
            }
        }
    }

    CreerStation(operations,nmb,2,order,tempsC);
}

void Algo3(struct Sommet operations[100] , int nmb,float tempsC)
{
    int cmp = 0;
    int couleur = 1;
    int order[100];

    for(int i = 0 ; i< nmb ; i++)
        operations[i].couleur = 0;

    while(cmp != nmb)
    {
        for(int i = 0 ; i < nmb ; i++)
        {
            if(operations[i].couleur == 0 )
            {
                int memeCouleur = 0;

                for(int j = 0 ; j < operations[i].nmbE ; j++)
                {
                    int t = FindSommet(operations,operations[i].exclusions[j],nmb);
                    if(operations[t].couleur == couleur)
                        memeCouleur = 10;
                }
                if(memeCouleur == 0)
                {
                    operations[i].couleur = couleur;
                    cmp++;
                }
            }
        }
        couleur = couleur +1;
    }


    CreerStation(operations,nmb,3,order,tempsC);

}


void Algo4(struct Sommet operations[100] , int nmb,float tempsC)
{
    int cmp = 0;
    int couleur = 1;
    int order[100];

    for(int i = 0 ; i< nmb ; i++)
        operations[i].couleur = 0;

    while(cmp != nmb)
    {
        for(int i = 0 ; i < nmb ; i++)
        {
            if(operations[i].couleur == 0 )
            {
                int memeCouleur = 0;

                for(int j = 0 ; j < operations[i].nmbE ; j++)
                {
                    int t = FindSommet(operations,operations[i].exclusions[j],nmb);
                    if(operations[t].couleur == couleur)
                        memeCouleur = 10;
                }
                if(memeCouleur == 0)
                {
                    operations[i].couleur = couleur;
                    cmp++;
                }
            }
        }
        couleur = couleur +1;
    }

    for(int i = 0 ; i < nmb ; i++)
        order[i] = 0;

    int placed[100];

    for(int i = 0 ; i < 100 ; i++)
        placed[i] = 0;

    cmp = 0;


    while( cmp != nmb)
    {
        for(int i = 0 ; i < nmb ; i++)
        {
            if(placed[i] == 0)
            {
                //printf("\ncmp: %d",cmp);

                int c = isPlaced(i,operations,nmb,placed);
                if(c == 1 )
                {
                    placed[i] = 1;
                    order[cmp] = i;
                    cmp++;
                }
            }
        }
    }


    CreerStation(operations,nmb,4,order,tempsC);

}

void Algo5(struct Sommet operations[100] , int nmb,float tempsC)
{
    int cmp = 0;
    int couleur = 1;
    int order[100];

    for(int i = 0 ; i< nmb ; i++)
        operations[i].couleur = 0;

    while(cmp != nmb)
    {
        for(int i = 0 ; i < nmb ; i++)
        {
            if(operations[i].couleur == 0 )
            {
                int memeCouleur = 0;

                for(int j = 0 ; j < operations[i].nmbE ; j++)
                {
                    int t = FindSommet(operations,operations[i].exclusions[j],nmb);
                    if(operations[t].couleur == couleur)
                        memeCouleur = 10;
                }
                if(memeCouleur == 0)
                {
                    operations[i].couleur = couleur;
                    cmp++;
                }
            }
        }
        couleur = couleur +1;
    }

    for(int i = 0 ; i < nmb ; i++)
        order[i] = 0;

    int placed[100];

    for(int i = 0 ; i < 100 ; i++)
        placed[i] = 0;

    cmp = 0;


    while( cmp != nmb)
    {
        for(int i = 0 ; i < nmb ; i++)
        {
            if(placed[i] == 0)
            {
                //printf("\ncmp: %d",cmp);

                int c = isPlaced(i,operations,nmb,placed);
                if(c == 1 )
                {
                    placed[i] = 1;
                    order[cmp] = i;
                    cmp++;
                }
            }
        }
    }


    CreerStation(operations,nmb,5,order,tempsC);

}

void calculeDesStations(struct Sommet operations[100] , int nmb ,float tmpC)
{
    printf("\n\n1-Algo 1 ( exclusions )\n2-Algo 2 ( temps et precedences ) \n3-Algo ( temps et exclusions ) 3\n4-Algo ( precedences et exclusions ) 4\n5-Algo 5 ( precedences exclusions et temps)\n");
    int choix ;
    scanf("%d",&choix);

    if(choix == 1)
    {
        //Algo1(operations , nmb);
    }

    if(choix == 2)
    {
        Algo2(operations,nmb,tmpC);
    }

    if(choix == 3)
    {
        Algo3(operations,nmb,tmpC);
    }

    if(choix == 4)
    {
        Algo4(operations,nmb,tmpC);
    }

    if(choix == 5)
    {
        Algo5(operations,nmb,tmpC);
    }
}

void data(struct Sommet operations[100] , int nmb)
{
    printf("\n\n1-Exclusions\n2-Precedences\n3-Temps\n");
    int choix ;
    scanf("%d",&choix);

    if(choix == 1)
    {
        for(int i = 0 ; i < nmb ; i++)
        {
            printf("\nOperation n%d  exclu : ",operations[i].valeur);
            for(int j = 0 ; j < operations[i].nmbE ; j++)
            {
                printf("[%d] ",operations[i].exclusions[j]);
            }
        }
    }
    if(choix == 2)
    {
        for(int i = 0 ; i < nmb ; i++)
        {
            printf("\nOperation n%d  exclu : ",operations[i].valeur);

            if(operations[i].premierPredecesseur != 0)
            {
                printf("[%d] ",operations[i].premierPredecesseur);
            }
            if(operations[i].secondPredecesseur != 0)
                printf("[%d] ",operations[i].secondPredecesseur);
            if(operations[i].troisPredecesseur != 0)
                printf("[%d] ",operations[i].troisPredecesseur);



        }
    }
    if(choix == 3)
    {
        for(int i = 0 ; i < nmb ; i++)
        {
            printf("\nOperation n%d  temps : %.2f sec ",operations[i].valeur,operations[i].tempsExecutionSeconde);
        }
    }
}



