#include <stdio.h>
#include <stdlib.h> // atoi
#include <math.h> // ceil
#include <string.h>

#include "MiscellaneousFunctions.h"

// CONTROL PANEL
int numberOfIterations = 20;
double verticesPresentesMultiplier = 0.5;
int numberOfgenerations = 5;
double mutationChance = 0.1;
double recombinationChance = 0.8;
int populationSize = 10;
//char typeOfMutation[20] = "addVertice";
char typeOfMutation[20] = "normal";
//char typeOfMutation[20] = "twoPoint";
char typeOfCrossover[20] = "normal";
int verbose = 1;
char nameOfFile[20] = "../res/teste.txt";



// For genetic and hillclimb
int getRand(int lowerInclusive, int higherInclusive){ // (lowerInclusive <= returnValue <= higherInclusive)
    return rand()%(higherInclusive-lowerInclusive+1)+lowerInclusive;
}

void printArray(int* array, int size){
    printf("[");
    for(int i=0; i<size; i++){
        printf("%d ", array[i]);
    }
    printf("]");
    printf("\n");
}

void setGraph(int** graph,int vertices, int value){
    //printf("Vertices:%d Value:%d\n",vertices,value);

    for(int i=0;i<vertices;i++){
        for(int j=0;j<vertices;j++) {
            //printf("%d %d\n",i,j);
            graph[i][j] = value;
        }
    }
}

void printGraph(int** graph,int vertices){
    printf("\n\n");
    for(int i=1;i<vertices+1;i++) {
        if(i==1){
            printf("    %d ",i);
        }
        else{
            printf("%d ",i);
        }
    }
    printf("\n");
    for(int i=1;i<vertices+1;i++){
        if(i==1){
            printf("    | ");
        }
        else{
            printf("| ");
        }
    }
    printf("\n");
    for(int i=0;i<vertices;i++){
        for(int j=0;j<vertices;j++) {
            if(j==0){
                printf("%d---%d ",i+1, graph[i][j]);
            }
            else{
                printf("%d ", graph[i][j]);
            }
        }
        printf("\n");
    }
    printf("\n\n");
}

int** createGrid(int linhas, int colunas){
    int** grid;
    grid=(int**)malloc(sizeof(int*)*linhas);
    if(grid==NULL){ printf("Error with malloc:"); exit(1);}
    for(int i=0; i<linhas;i++){
        grid[i]= (int*)malloc(sizeof(int) * colunas);
        if(grid[i]==NULL){ printf("Error with malloc:"); exit(1);}
    }
    return grid;
}

int countConnections(int *solution, int** graph, int vertices, int extraVerbose){
    int links=0;
    int i,j;
    for(i=0;i<vertices;i++){ // run through all nodes in solution
        //printf("%d ", solution[i]);
        for(j=0;j<vertices;j++){
            if(solution[i]!=-1 && solution[j]!=-1 && i!=j){
                //printf("%d|%d\n", solution[i],solution[j]);
                if(graph[ solution[i]-1 ][ solution[j]-1 ]==1){
                    //printf("%d__%d\n",solution[i],solution[j]);
                    //printf("liked\n");
                    links++;
                }
            }
        }
    }
    if(verbose && extraVerbose){printf("links:%d\n",links/2);}
    return links/2; // because it counts the connection between node 5 and node 2 and between node 2 and node 5
}

void resetArray(int *array, int size, int value){
    for(int i=0;i<size;i++){
        array[i]=value; // initialize it
    }
}

void copyArray(int *arrCopy, int *arrPaste, int size){
    for(int i=0; i<size; i++){
        arrPaste[i]=arrCopy[i];
    }
}

int** readFile(int *num_vertices){
    int **graph;
    char str1[50], str2[50], str3[50], str4[50], lineBuffer[500];
    int vertice1, vertice2;
    FILE* fp = fopen(nameOfFile, "rw");
    if(fp==NULL){
        perror("Failed opening file: ");
        exit(1);
    }

    while(fgets(lineBuffer, 500, fp)!=NULL) {
        //printf("%colunas\n", lineBuffer);

        switch (lineBuffer[0]){
            case 'e':
                sscanf(lineBuffer, "%s %s %s", str1, str2, str3);
                if(verbose){printf("%s %d %d\n", str1, atoi(str2), atoi(str3));}
                vertice1 = atoi(str2);
                vertice2 = atoi(str3);

                graph[vertice1-1][vertice2-1]=1;
                // same beacause its simetrical unless  it is a directed graph (not the case)
                graph[vertice2-1][vertice1-1]=1;

                break;
            case 'c':
                continue;
            case 'p':
                sscanf(lineBuffer, "%s %s %s %s", str1, str2, str3, str4);
                if(verbose){printf("%s %s %d %d\n", str1, str2, atoi(str3), atoi(str4));}

                (*num_vertices) = atoi(str3);

                graph = (int**)malloc(sizeof(int*)*(*num_vertices));
                if(graph==NULL){ printf("Error with malloc:"); exit(1);}
                for(int i=0; i<(*num_vertices); i++){
                    graph[i] = (int*) malloc(sizeof(int)*(*num_vertices));
                    if(graph[i]==NULL){ printf("Error with malloc:"); exit(1);}
                }

                setGraph(graph,(*num_vertices),0);
                break;
        }
    }

    fclose(fp);

    return graph;
}


// hillclimb only
int** aumentarGrid(int** grid, int* linhas, int colunas){
    (*linhas)++;
    grid=(int**)realloc(grid, sizeof(int*)*(*linhas));
    grid[(*linhas)-1]= (int*)malloc(sizeof(int)*colunas);
    if(grid[*linhas-1]==NULL){ printf("Error with malloc:"); exit(1);}

    return grid;
}

void printGrid(int** grid, int linhas, int colunas){
    for(int i=0; i<linhas; i++){
        for(int j=0; j<colunas; j++){
            printf("%d ",grid[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}


// genetic only
float evaluateH(int *h1, int vertices, int **graph, int *ligacoes, int *verticesPresentes){
    *ligacoes = 0; *verticesPresentes=0;
    *ligacoes = countConnections(h1, graph, vertices, 0);
    for(int i=0; i<vertices; i++){
        if(h1[i]!=-1){
            (*verticesPresentes)++;
        }
    }
    return (float)*verticesPresentes*(float)verticesPresentesMultiplier/((float)*ligacoes+(float)1);
}

void printPopulation(int p, int vertices, int **population, int **graph, float *globalAvgRnk){
    float avgRnk=0;
    for(int i=0; i < p; i++){
        for(int j=0; j < vertices; j++){
            if(verbose){ printf("%d ",population[i][j]); }
        }

        int ligacoes, verticesPresentes;
        float response = evaluateH(population[i],vertices,graph, &ligacoes, &verticesPresentes);
        if(verbose){printf("(LI%d VP%d RNK %f)\n", ligacoes, verticesPresentes, response);}
        avgRnk+=response;
    }
    avgRnk=avgRnk/(float)p;
    (*globalAvgRnk)+=avgRnk;
    // maybe leave on
    if(verbose) {
        printf("AVG RNK %f\n", avgRnk);
        printf("\n");
    }
}

void crossover(int *aParent, int *bParent, int *aChild, int *bChild, int vertices){
    int crossoverIndex; // its inclusive
    crossoverIndex=vertices/2;

    if(verbose){ printf("CrossoverPoint=%d\n",crossoverIndex); }


    resetArray(aChild,vertices,-9);
    resetArray(bChild,vertices,-9);

    // aChild
    for(int i=0; i<crossoverIndex; i++){
        aChild[i]=aParent[i];
    }
    for(int i=crossoverIndex; i<vertices; i++){
        aChild[i]=bParent[i];
    }

    // bChild
    for(int i=0; i<crossoverIndex; i++){
        bChild[i]=bParent[i];
    }
    for(int i=crossoverIndex; i<vertices; i++){
        bChild[i]=aParent[i];
    }
}

void crossoverTwoPointRandom(int *aParent, int *bParent, int *aChild, int *bChild, int vertices){
    int crossoverIndex1, crossoverIndex2; // its inclusive
    do{
        crossoverIndex1 = getRand(0,vertices-1);
        crossoverIndex2 = getRand(0,vertices-1);
    }while(crossoverIndex1==crossoverIndex2 || crossoverIndex1>crossoverIndex2);

    if(verbose){ printf("CrossoverPoint1=%d CrossoverPoint2=%d\n",crossoverIndex1, crossoverIndex2); }

    resetArray(aChild,vertices,-9);
    resetArray(bChild,vertices,-9);

    // aChild
    for(int i=0; i<crossoverIndex1; i++){
        aChild[i]=aParent[i];
    }
    for(int i=crossoverIndex1; i<crossoverIndex2; i++){
        aChild[i]=bParent[i];
    }
    for(int i=crossoverIndex2; i<vertices; i++){
        aChild[i]=aParent[i];
    }

    // bChild
    for(int i=0; i<crossoverIndex1; i++){
        bChild[i]=bParent[i];
    }
    for(int i=crossoverIndex1; i<crossoverIndex2; i++){
        bChild[i]=aParent[i];
    }
    for(int i=crossoverIndex2; i<vertices; i++){
        bChild[i]=bParent[i];
    }
}

int mutate(int *subject, int vertices){
    int temp = getRand(0, vertices-1);
    if(subject[temp]==-1){
        subject[temp]=temp+1;
    }
    else{
        subject[temp]=-1;
    }
    return temp;
}

int mutateAddVertice(int *subject, int vertices){
    int foundMinusOne=0;
    for(int i=0; i<vertices; i++){
        if(subject[i]==-1){
            foundMinusOne=1;
            break;
        }
    }
    if(foundMinusOne==0){
        if(verbose){ printf("Nothing to mutate\n"); }
        return -1;
    }
    int temp; // counter is used to prevent infinite loop if the array has no -1
    do {
        temp = getRand(0, vertices - 1);
    }while(subject[temp]!=-1);
    subject[temp]=temp+1;
    return temp;
}

int** createNextGen(int p, double r, double m, int vertices, int **initPop, int **graph){
    int **nextGen = createGrid(p, vertices);
    // RESET NEXT GEN
    for(int o=0; o<p; o++){
        resetArray(nextGen[o],vertices,0);
    }

    int couplesToCross, toPass;
    couplesToCross=ceil( (r*(double)p)/(double)2 );
    toPass=p-(couplesToCross*2);
    if(verbose){printf("ToPas=%d couplesToCross=%d\n", toPass, couplesToCross);}


    float probOfSelection[p];
    float total=0;
    int trashLigacoes, trashVerticesPresentes;
    // SETTING THE PROBABILITIES BASED ON THEIR RNK
    for(int i=0; i<p; i++){
        total += evaluateH(initPop[i],vertices, graph, &trashLigacoes, &trashVerticesPresentes);
    }
    for(int i=0; i<p; i++){
        // GET A # BETWEEN 0 AND 100
        probOfSelection[i] =
                100*(evaluateH(initPop[i],vertices, graph, &trashLigacoes, &trashVerticesPresentes)/total);
    }

    // PASS UNALTERED
    int index=0, temp, temp2, equal;
    for(int i=1; i<=toPass; i++){
        do{
            temp = getRand(0, p-1);
        }while(getRand(0, 100)>(int)probOfSelection[temp]);
        // temp passed the probability selection and will be passed unaltered
        nextGen[index] = initPop[temp];
        if(verbose){ printf("Index [%d](NEXTGEN) was passed unaltered from index [%d](INITPOP).\n", index, temp); }
        index++;
    }


    // CROSS
    for(int i=1; i<=couplesToCross; i++){
        do{ // SELECT PARENTS
            equal=0;

            do{
                temp = getRand(0, p-1);
            }while(getRand(0, 100)>(int)probOfSelection[temp]);
            // temp passed the probability selection and will be a parent

            do{
                temp2 = getRand(0, p-1);
            }while(getRand(0, 100)>(int)probOfSelection[temp2]);
            // temp2 passed the probability selection and will be a parent

            if(temp==temp2){
                equal=1;
            }
        }while(equal);

        int aChild[vertices], bChild[vertices];
        resetArray(aChild,vertices,0);
        resetArray(bChild,vertices,0);


/*if(verbose){ printf("temp=%d temp2=%d\n",temp,temp2); }*/


        if(strcmp(typeOfCrossover,"normal")==0){
            crossover(initPop[temp], initPop[temp2], aChild, bChild, vertices);
        }
        else if(strcmp(typeOfCrossover,"twoPoint")==0){
            crossoverTwoPointRandom(initPop[temp], initPop[temp2], aChild, bChild, vertices);
        }
        else{
            printf("Wrong crossover value defined\n");
            exit(0);
        }

/*if(verbose){ printf("index=%d\n",index); }*/

        copyArray(aChild,nextGen[index],vertices);
        if(verbose){ printf("Index [%d](NEXTGEN) is a child from index [%d](INITPOP) & [%d](INITPOP).\n", index, temp, temp2); }
        index++;



/*if(verbose){ printf("index=%d\n",index); }*/

        copyArray(bChild,nextGen[index],vertices);
        if(verbose){ printf("Index [%d](NEXTGEN) is a child from index [%d](INITPOP) & [%d](INITPOP).\n", index, temp, temp2); }
        index++;
    }

    // MUTATE
    for(int i=0; i<p; i++){
        if(getRand(1, 100) <= m*100){
            // mutate this hypothesis
            int genome;
            if(strcmp(typeOfMutation, "normal")==0){
                genome = mutate(nextGen[i],vertices);
            }
            else if(strcmp(typeOfMutation, "addVertice")==0){
                genome = mutateAddVertice(nextGen[i],vertices);
            }
            if(verbose){ printf("Index [%d](NEXTGEN) was mutated at genome of index [%d].\n", i, genome); }

        }
    }

    return nextGen;
}