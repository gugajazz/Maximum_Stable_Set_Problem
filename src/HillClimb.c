#include <stdio.h>
#include <stdlib.h> // atoi
#include <time.h>
#include "MiscellaneousFunctions.h"


int* hillClimb(int** graph,int vertices, int iterations){
    float avgSizeBestSol=0;
    int globalBestSolution=0;
    int globalBestSolutionArr[vertices];
    resetArray(globalBestSolutionArr,vertices,0);

    for(int r=1; r<=iterations;r++) {
        int localBestSolution;
        int localBestSolutionArr[vertices];
        resetArray(localBestSolutionArr,vertices,0);

        int tempForTies[vertices];
        resetArray(tempForTies, vertices, -1);

        // starts from initial state
        int curSolution[vertices];
        for (int i = 0; i < vertices; i++) {
            curSolution[i] = i + 1; // make curSolution as {1 2 3 4 5 6 .. n} n being the number of vertices
        }

        // generates the sucessor states
        int linhas = 0, colunas = vertices, k = 0, deletedNumber = 0;
        int **grid;
        int minConnections = 9999, indexOfMinConnections = -1, temp = -1;
        grid = createGrid(linhas, colunas);

        while (minConnections) {
            localBestSolution = 0;
            for (int deleteIndice = 0; deleteIndice < vertices; deleteIndice++) {
                grid = aumentarGrid(grid, &linhas, colunas); // adds an extra line/linha to the graph
                for (int i = 0; i < vertices; i++) { // fill every row of the grid with the current solution
                    if (grid[deleteIndice][i] == -1) {
                        // maintains previous deleted nodes
                        continue;
                    }
                    grid[deleteIndice][i] = i + 1;
                    if (grid[deleteIndice][i] == deletedNumber)
                        grid[deleteIndice][i] = -1; // deletes the node that was deleted in the previous iteration from each row
                }
                grid[deleteIndice][deleteIndice] = -1; // delete a different node from each row
            }
            if (verbose) { printGrid(grid, vertices, colunas); }


            // evaluates them
            for (int i = 0; i < vertices; i++) {
                temp = countConnections(grid[i], graph, vertices, 1);
                if (temp <= minConnections) {
                    minConnections = temp;
                    indexOfMinConnections = i;
                }
            }

            // to run probabilities on the ties
            resetArray(tempForTies, vertices, -1);
            for (int i = 0; i < vertices; i++) {
                temp = countConnections(grid[i], graph, vertices, 0);
                if (temp == minConnections) {
                    tempForTies[i] = i; // going to look like [-1 -1 2 -1 4] if the ties are at indexes 2 and 4
                }
            }
            while (1) {
                temp = getRand(0, vertices - 1);
                if (tempForTies[temp] != -1) {
                    indexOfMinConnections = temp;
                    minConnections = countConnections(grid[indexOfMinConnections], graph, vertices, 0);
                    break;
                }
            }


            if (verbose) {
                printf("minConnections=%d AND indexOfMinConnections=%d\n", minConnections, indexOfMinConnections);
            }
            deletedNumber = indexOfMinConnections + 1;

            // for debugging (chose any index to be used as the best)
            /*printf("index to use: ");
            scanf("%d",&indexOfMinConnections);*/

            // sets current state to the best
            for (int i = 0; i < vertices; i++) {
                curSolution[i] = grid[indexOfMinConnections][i];
            }

            for (int i = 0; i < vertices; i++) { // prints the solution including -1 values
                if (verbose) { printf("%d ", grid[indexOfMinConnections][i]); }
                copyArray(grid[indexOfMinConnections],localBestSolutionArr,vertices);
                if (grid[indexOfMinConnections][i] != -1) { // count number of nodes in solution
                    localBestSolution++;
                }
            }
            if (verbose) { printf("\n\n"); }
        }
        if(localBestSolution>globalBestSolution){
            globalBestSolution=localBestSolution;
            copyArray(localBestSolutionArr,globalBestSolutionArr,vertices);
        }
        // count iterations if not verbose
        if (!verbose) { printf("Iteration:%d/%d (Solution=%d)\n",r,iterations,localBestSolution); }
        avgSizeBestSol+=(float)localBestSolution;
    }
    printf("Final Best Solution: %d\n",globalBestSolution);
    printArray(globalBestSolutionArr,vertices);
    avgSizeBestSol=avgSizeBestSol/(float)numberOfIterations;
    printf("Avg size best solution: %f",avgSizeBestSol);
}

int main() {

    int **graph;
    int num_vertices;

    srand(time(0));

    graph = readFile(&num_vertices);

    if(verbose){printGraph(graph,num_vertices);}

    hillClimb(graph,num_vertices,numberOfIterations);

    return 0;

}
