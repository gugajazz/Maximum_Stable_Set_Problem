#include <stdio.h>
#include <stdlib.h> // atoi
#include <time.h>
#include "MiscellaneousFunctions.h"


void genetic(int generations, int p, double r, double m, int vertices, int **graph, int **initPopInput){

/*p = numero de hipóteses da população
    r = fração da população a ser recombinada
    m = mutation rate*/


    float globalAvgRnk=0;
    int bestSolution[vertices];
    int verticesInBestSol=0;
    int **initPop = createGrid(p, vertices);
    // INITIALIZE initPop
    for(int i=0; i<p ; i++){
        for(int j=0; j<vertices; j++){
            initPop[i][j]=0;
        }
    }


    int temp, numEmpty;
    if(initPopInput==NULL){
        // create the initial population by setting random values for the indexes that are empty and filling the rest
        for(int i=0; i<p ; i++){
            numEmpty = getRand(0, vertices-1);
            for(int l=0; l<=numEmpty; l++){
                do{
                    temp = getRand(0, vertices-1);
                }while(initPop[i][temp] == -1);
                initPop[i][temp] = -1;
            }
        }
        for(int i=0; i<p ; i++){
            for(int j=0; j<vertices; j++){
                if(initPop[i][j]!=-1){
                    initPop[i][j]=j+1;
                }
            }
        }
    }
    else{
        for(int i=0; i<p; i++){
            copyArray(initPopInput[i],initPop[i],vertices);
        }
    }

    // generations loop
    int numVertices;
    while(generations>0){
        //check initPop for a good solution
        for(int i=0;i<p;i++){
            numVertices=0;
            if(countConnections(initPop[i],graph,vertices,0)==0){
                for(int y = 0; y < vertices; y++) {
                    if (initPop[i][y] != -1) { // count number of vertices in solution
                        numVertices++;
                    }
                }
                if(numVertices>verticesInBestSol){
                    verticesInBestSol=numVertices;
                    copyArray(initPop[i],bestSolution,vertices);
                    if(verbose){
                        printf("New best solution with %d vertices\n", verticesInBestSol);
                        printArray(bestSolution,vertices);
                    }
                }
            };
        }

        if(verbose){
            printf("\nINIT POP\n");
            printPopulation(p,vertices,initPop,graph, &globalAvgRnk);
        }

        // NEXT GENERATION
        int **nextGen = createNextGen(p,r,m,vertices,initPop, graph);

        if(verbose){
            printf("\nNEXT GEN\n");
            printPopulation(p,vertices,nextGen,graph, &globalAvgRnk);
        }

        // COPY NEXT GEN TO INIT POP
        for(int i=0; i<p; i++){
            copyArray(nextGen[i], initPop[i], vertices);
        }
        generations--;
        printf("Generations left: %d\n",generations);
    }

    if(verticesInBestSol==0){
        printf("\n\n!!! No solution found !!!\n");
    }
    else{
        printf("\n\n!!! Best solution found had %d vertices !!!\n", verticesInBestSol);
        printArray(bestSolution,vertices);
        // MAYBE LEAVE ON
        printf("GLOBAL AVG RNK = %f\n",globalAvgRnk/(float)numberOfgenerations);
    }
}

int main() {

    int **graph;
    int num_vertices;

    srand(time(0));

    graph = readFile(&num_vertices);

    if(verbose){printGraph(graph,num_vertices);}

    genetic(numberOfgenerations,populationSize,recombinationChance,mutationChance,num_vertices, graph, NULL);



    return 0;
}

