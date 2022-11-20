#ifndef MISCELLANEOUSFUNCTIONS_H
#define MISCELLANEOUSFUNCTIONS_H

// CONTROL PANEL
extern int verbose;
extern int numberOfIterations;
extern double verticesPresentesMultiplier;
extern int numberOfgenerations;
extern double mutationChance;
extern double recombinationChance;
extern int populationSize;
extern char typeOfMutation[20];
extern char nameOfFile[20];

// For genetic and hillclimb
int getRand(int lowerInclusive, int higherInclusive);
void printArray(int* array, int size);
void setGraph(int** graph,int vertices, int value);
void printGraph(int** graph,int vertices);
int** createGrid(int linhas, int colunas);
int countConnections(int *solution, int** graph, int vertices, int extraVerbose);
void resetArray(int *array, int size, int value);
void copyArray(int *arrCopy, int *arrPaste, int size);
int** readFile(int *num_vertices);

// hillclimb only
int** aumentarGrid(int** grid, int* linhas, int colunas);
void printGrid(int** grid, int linhas, int colunas);

// genetic only
float evaluateH(int *h1, int vertices, int **graph, int *ligacoes, int *verticesPresentes);
void printPopulation(int p, int vertices, int **population, int **graph, float *globalAvgRnk);
void crossover(int *aParent, int *bParent, int *aChild, int *bChild, int vertices);
void crossoverTwoPointRandom(int *aParent, int *bParent, int *aChild, int *bChild, int vertices);
int mutate(int *subject, int vertices);
int mutateAddVertice(int *subject, int vertices);
int** createNextGen(int p, double r, double m, int vertices, int **initPop, int **graph);

#endif //MISCELLANEOUSFUNCTIONS_H
