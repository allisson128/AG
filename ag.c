#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int    randsingular (int *vet, int size, int edge);     /* gera individuo aleatorio, sem repeticao de inteiros */
int contain (int* vet, int element, int size);
char** initEvalAuxVector (char* A, char* B, char* C); 	/* Esse eh aquele vetor de letras nao repetidas */
int    evalAplusBequalC (int *vet);                   /* func. de aval. A + B = C */
void imprimeVet(int* vet, int size);

int main(int argc, char** argv) {
  int**  population;
  int    population_size = 50;
  int    domain_size = 10;
  int    i;
  char** map;

  srand(time(NULL));

  if (argc != 4) {
    printf("Error! Invalid number of args.\n");
    exit(1);
  }

  /* map = initEvalAuxVector (argv[1], argv[2], argv[3]); */

  population = (int**) malloc (population_size * sizeof(int *));
  for(i = 0; i < population_size; ++i) {
    population[i] = (int*) malloc ((domain_size + 2) * sizeof(int));
    randsingular(population[i], domain_size, 10);
    imprimeVet(population[i], 10);
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

void imprimeVet(int* vet, int size) {
  int i;
  putchar('\n');
  for (i = 0; i < size; ++i) {
    printf ("%d ", vet[i]);
  }
  putchar('\n');
}
