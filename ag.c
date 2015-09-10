/* OBS:
   - Pode-se gerar soh 8 numeros?
   - Pode ter numeros repetidos?
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define POPULATION_SIZE 50

int    randsingular (int *vet, int size, int edge);   /* gera individuo aleatorio, sem repeticao de inteiros */
int    contain (int* vet, int element, int size);     /* verifica se ha o elemeento no vetor */
void   copy (int* vet1, int* vet2, int size);
char** initEvalAuxVector (char* A, char* B, char* C); /* Esse eh aquele vetor de letras nao repetidas */
int    evalAplusBequalC (int *vet);                   /* func. de aval. A + B = C */
int    roulette(int** population, int populationsize, int col, int value);
void   imprimeVet(int* vet, int size);
int**  init_population(int individual_size);
void   crossover_simple(int** newpopulation, int it, int** originalpopulation, int index_pai1, int index_pai2);
void   permutation(int** newpopulation, int newpopulationsize, float mutationrate);

int main(int argc, char** argv) {

  int**  originalpopulation, **newpopulation;
  
  // CONSTANTES
  int    newpopulationsize, populationsize = 50;
  float  crossoverrate = .2, mutationrate = .02;
  int    numero_geracoes = 1;

  // Tentar fazer generico
  int    domain_size = 10;
  int    i, j, acc,it;
  /* char** map; */

  newpopulationsize = populationsize * crossoverrate;
  printf("\nnewPopSize = %d\n", newpopulationsize);
  srand(time(NULL));

  /* if (argc != 4) { */
  /*   printf("Error! Invalid number of args.\n"); */
  /*   exit(1); */
  /* } */

  /* GERA POPULACAO */
  originalpopulation = init_population(domain_size);
  newpopulation = init_population(domain_size);
	
  for(i = 0; i < populationsize; ++i) {
    randsingular(originalpopulation[i], domain_size, 10);
  }

  /* ATRIBUI NOTAS - Avalia populacao inicial */
  acc = 0;
  for (i = 0; i < populationsize; ++i) {
    acc += originalpopulation[i][10] = evalAplusBequalC (originalpopulation[i]);
    originalpopulation[i][11] = acc;
    //imprimeVet(originalpopulation[i], 12);
  }

  for (j = 0; j < numero_geracoes; ++j) {

    /* CROSSOVER */
    /* Roleta */

    for (it = 0; it < newpopulationsize; it++) {
      /* SELECAO */
      int index_pai1 = roulette(originalpopulation, populationsize, 11, acc);
      int index_pai2 = roulette(originalpopulation, populationsize, 11, acc);

      crossover_simple(newpopulation, it, originalpopulation, index_pai1, index_pai2);
      // imprimeVet(originalpopulation[index_pai1], 10);
      // imprimeVet(originalpopulation[index_pai2], 10);
      // imprimeVet(newpopulation[2*it], 10);
      // imprimeVet(newpopulation[2*it+1], 10);
      // putchar('\n');
    }
		
    /* MUTACAO */
    permutation(newpopulation, newpopulationsize, mutationrate);

  	/* ATRIBUI NOTAS - Avalia populacao de filhos */
  	acc = 0;
  	for (i = 0; i < newpopulationsize; ++i) {
    	acc += newpopulation[i][10] = evalAplusBequalC (newpopulation[i]);
    	newpopulation[i][11] = acc;
    	imprimeVet(newpopulation[i], 12);
  	}
  	
    int** temp = init_population(10);
    for(i = 0; i < 50; i++) {

      int valor_maior = -1, maior = 0,x,y, matrix = 0;

      for(x = 0; x < populationsize; ++x) {
        if(originalpopulation[x][10] > valor_maior) {
            maior = x;
            matrix = 0;
            valor_maior = originalpopulation[maior][10];
        }
      }

      for(y = 0; y < newpopulationsize; ++y) {
        if(newpopulation[y][10] > valor_maior){
            maior = y;
            matrix = 1;
            valor_maior = newpopulation[y][10];
        }
      }

      if(matrix)
        copy(temp[i], newpopulation[maior], 12);
      else
        copy(temp[i], originalpopulation[maior], 12);
    }

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

void copy (int* destino, int* origem, int size) {
  int i;
  for (i = 0; i < size; ++i) {
    destino[i] = origem[i];
  }

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

  return abs(C - A + B);
}

int roulette(int** population, int populationsize, int col, int value) {
  /* ### ROLETA ### 
   * INPUT:  recebe a matriz Populacao, seu tamanho, a coluna das avalicoes acumuladas e o valor total de fichas distribuidas;
   * OUTPUT: retorna o individuo da ficha que foi sorteada;  
   */
  int i, token;
  int random = rand() * 999;
  int rnd = random % value;

  for (i = 0; i < populationsize; ++i) {
    if (rnd < population[i][col]) {
      return i;
    }
  }
  return --i;
}

void imprimeVet(int* vet, int size) {
  int i;
  putchar('\n');
  for (i = 0; i < size; ++i) {
    printf ("%d ", vet[i]);
  }
  putchar('\n');
}

int** init_population(int individual_size) {
  int **temp;
  int i;

  temp = (int**) malloc (POPULATION_SIZE * sizeof(int *));
  for(i = 0; i < POPULATION_SIZE; ++i) {
    temp[i] = (int*) malloc ((individual_size + 2) * sizeof(int));
  }

  return temp;
}

void crossover_simple(int** newpopulation, int it, int** originalpopulation, int index_pai1, int index_pai2) {
	int i, corte = rand() % 10;
	it *= 2;
	for (i = 0; i < corte; ++i) {
		newpopulation[it][i] = originalpopulation[index_pai1][i];
		newpopulation[it + 1][i] = originalpopulation[index_pai2][i];
	}
	for (i = corte; i < 10; ++i) {
		newpopulation[it][i] = originalpopulation[index_pai2][i];
		newpopulation[it + 1][i] = originalpopulation[index_pai1][i];
	}
}

void permutation(int** newpopulation, int newpopulationsize, float mutationrate) {
	int i, aux, qtd = (int) newpopulationsize * mutationrate;
	int rnd, rnd1, rnd2;

	for (i = 0; i < qtd; ++i) {
		rnd  = rand() % newpopulationsize;
		rnd1 = rand() % 10;
		rnd2 = rand() % 10;

		aux = newpopulation[rnd][rnd1];
		newpopulation[rnd][rnd1] = newpopulation[rnd][rnd2];
		newpopulation[rnd][rnd2] = aux;
	}

}
/* RASCUNHO:
   map = initEvalAuxVector (argv[1], argv[2], argv[3]); 
*/
