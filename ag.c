/* OBS:
 - Pode-se gerar soh 8 numeros?
 - Pode ter numeros repetidos?
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int    randsingular (int *vet, int size, int edge);   /* gera individuo aleatorio, sem repeticao de inteiros */
int    contain (int* vet, int element, int size);     /* verifica se ha o elemeento no vetor */
char** initEvalAuxVector (char* A, char* B, char* C); /* Esse eh aquele vetor de letras nao repetidas */
int    evalAplusBequalC (int *vet);                   /* func. de aval. A + B = C */
void   imprimeVet(int* vet, int size);

int main(int argc, char** argv) {

  int**  originalpopulation, **newpopulation;
  int    newpopulationsize, populationsize = 50;
  int    numero_geracoes = 100;
  int    domain_size = 10;
  float  crossoverrate = .6, mutationrate = .02;
  int    i, j, acc,it;
  /* char** map; */

  newpopulationsize = populationsize * crossoverrate;

  srand(time(NULL));

  /* if (argc != 4) { */
  /*   printf("Error! Invalid number of args.\n"); */
  /*   exit(1); */
  /* } */


  /* GERA POPULACAO */
  originalpopulation = (int**) malloc (populationsize * sizeof(int *));
  newpopulation = (int**) malloc (populationsize * sizeof(int *));
  for(i = 0; i < populationsize; ++i) {
    newpopulation[i] = NULL;
    originalpopulation[i] = (int*) malloc ((domain_size + 2) * sizeof(int));
    randsingular(originalpopulation[i], domain_size, 10);
  }

  /* ATRIBUI NOTAS - Avalia populacao inicial */
  acc = 0;
  for (i = 0; i < populationsize; ++i) {
    acc += originalpopulation[i][10] = evalAplusBequalC (originalpopulation[i]);
    originalpopulation[i][11] = acc;
    imprimeVet(originalpopulation[i], 12);
  }

  for (j = 0; j < numero_geracoes; ++j) {

  
    /* CROSSOVER */
    /* Roleta */

    for (it = 0; it < crossoverrate * populationsize; it++) {
      
    }
    
    /* MUTACAO */
  }
  return 0;
}

int randsingular(int *vet, int size, int edge) {
/* Recebe um vetor e preeche com inteiros aleatorios sem repeticao */
  int i, r, size2 = 0;
  for (i = 0; i < size; ++i) {
    do {
      r = rand()%10;
    } while (contain(vet, r, size2));
    vet[i] = r;
    ++size2;
  }
}

int contain (int* vet, int element, int size) {
  int i = 0;
  while (i < size) {
    if (vet[i] == element) {
      return 1;
    }
    ++i;
  }
  return 0;
}

char** initEvalAuxVector (char* A, char* B, char* C) {
  /* Esse eh aquele vetor de letras nao repetidas */

  int    size = (int) (sizeof(A) + sizeof(B) + sizeof(C));
  int    singularSize; 		/* tamanho sem repeticao */
  char** map  = (char**) malloc (2 * sizeof (char*));
  char*  temp = (char*) malloc (size * sizeof(char));

  strcpy(temp, A);
  strcat(temp, B);
  strcat(temp, C);

  map[0] = (char*) malloc (size * sizeof(char));
  map[1] = (char*) malloc (size * sizeof(char));

  free(temp);
}

int evalAplusBequalC (int *vet) {
  /* *********** */
  /* IMPROVISADA */
  /* *********** */
  int A, B, C;
  A =  vet[0] * 1000 + vet[1] * 100 + vet[2] * 10 + vet[3];
  B =  vet[4] * 1000 + vet[5] * 100 + vet[6] * 10 + vet[1];
  C =  vet[4] * 10000 + vet[5] * 1000 + vet[2] * 100 + vet[1] * 10 + vet[7];

  if (A + B == C) {
    printf("\n\n!!! ENCONTROU !!!\nSend + More = Money\n\n");
  }

  return 100000 - abs(C - A + B);
}

void imprimeVet(int* vet, int size) {
  int i;
  putchar('\n');
  for (i = 0; i < size; ++i) {
    printf ("%d ", vet[i]);
  }
  putchar('\n');
}

/* RASCUNHO:
   map = initEvalAuxVector (argv[1], argv[2], argv[3]); 
 */
