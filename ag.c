/* OBS:
   - Pode-se gerar soh 8 numeros?
   - Pode ter numeros repetidos?
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define EVAL_REFERENCE 100000

int    randsingular (int *vet, int size, int edge);   /* gera individuo aleatorio, sem repeticao de inteiros */
int    contain (int* vet, int element, int size);     /* verifica se ha o elemeento no vetor */
void   copy (int* destino, int* origem, int size);
char** initEvalAuxVector (char* A, char* B, char* C); /* Esse eh aquele vetor de letras nao repetidas */
int    evalAplusBequalC (int *vet);                   /* func. de aval. A + B = C */
int    roulette(int** population, int populationsize, int col, int value);
int    tournament (int** population, int populationsize, int tour);
void   imprimeVet(int* vet, int size);
int**  init_population(int individual_size, int population_size);
void   crossover_simple(int** newpopulation, int it, int** originalpopulation, int index_pai1, int index_pai2);
void   permutation(int** newpopulation, int newpopulationsize, float mutationrate);
void   pmx(int* parent1, int* parent2, int* son1, int* son2, int vector_size, int p1, int p2);


//(tamanho_da_populacao, nro_de_geracoes, pcross, pmut, S, C, M, semente)
int ag(int populationsize, int numero_geracoes, float crossoverrate, float mutationrate, int S, int C, int M, int semente) {

  int**  originalpopulation, **newpopulation;
  
  // CONSTANTES
  int    newpopulationsize;

  // Tentar fazer generico
  int    domain_size = 10;
  int    tour = 3;
  int    i, j, acc,it;
  int    optimal = -1;
  /* char** map; */

  newpopulationsize = populationsize * crossoverrate;

  srand( (unsigned int) (semente < 0 ? time(NULL) : semente) );

  /* if (argc != 4) { */
  /*   printf("Error! Invalid number of args.\n"); */
  /*   exit(1); */
  /* } */

  /* GERA POPULACAO */
  originalpopulation = init_population(domain_size, populationsize);
  newpopulation = init_population(domain_size, populationsize);
	
  for(i = 0; i < populationsize; ++i) {
    randsingular(originalpopulation[i], domain_size, domain_size);
  }

  /* ATRIBUI NOTAS - Avalia populacao inicial */
  acc = 0;
  for (i = 0; i < populationsize; ++i) {
    acc += originalpopulation[i][domain_size] = evalAplusBequalC (originalpopulation[i]);

    if (originalpopulation[i][domain_size] == EVAL_REFERENCE) {
      optimal = i;
    }

    originalpopulation[i][11] = acc;
    //imprimeVet(originalpopulation[i], 12);
  }

  // LASSO
  int index_pai1, index_pai2;
  for (j = 0; (j < numero_geracoes) && (optimal < 0); ++j) {
    
    for (it = 0; it < newpopulationsize; it++) {

      //*************************** SELECAO ******************************
      if (S == 1) {
	// Roleta 
	index_pai1 = roulette (originalpopulation, populationsize, domain_size - 1, acc);
	index_pai2 = roulette (originalpopulation, populationsize, domain_size - 1, acc);
      }
      else if (S == 2) {
	// Torneio Simples
	index_pai1 = tournament (originalpopulation, populationsize, tour);
	index_pai2 = tournament (originalpopulation, populationsize, tour);
      }
      //******************************************************************

      /*************************** CROSSOVER ******************************/
      crossover_simple(newpopulation, it, originalpopulation, index_pai1, index_pai2);
      /********************************************************************/

      /* imprimeVet(originalpopulation[index_pai1], 10); */
      /* imprimeVet(originalpopulation[index_pai2], 10); */
      /* imprimeVet(newpopulation[2*it], 10); */
      /* imprimeVet(newpopulation[2*it+1], 10); */
      /* putchar('\n'); */
    }
		
    /*************************** MUTACAO ******************************/
    permutation(newpopulation, newpopulationsize, mutationrate);
    /******************************************************************/

    /***** AVALIA os novos indiv. apos Crossover e Mutacao ************/
    acc = 0;
    for (i = 0; i < newpopulationsize; ++i) {
      acc += newpopulation[i][10] = evalAplusBequalC (newpopulation[i]);
      newpopulation[i][11] = acc;
      /* imprimeVet(newpopulation[i], 12); */
    }
    /******************************************************************/

    /****** SELECIONA os melhores indiv. dentre os pais e filhos ******/

    int** temp = init_population(domain_size, populationsize);
    //init_population(10);
    for(i = 0; i < 50; i++) {

      int x, y, valor_maior = -1, maior = 0, matrix = 0;

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

      if (matrix) {
        copy(temp[i], newpopulation[maior], 12);
	newpopulation[maior][10] = 0;

	if (temp[i][10] == EVAL_REFERENCE) {
	  optimal = i;
	}

      }
      else {
        copy(temp[i], originalpopulation[maior], 12);
	originalpopulation[maior][10] = 0;

	if (temp[i][10] == EVAL_REFERENCE) {
	  optimal = i;
	}

      }
    }
    /****************************************************************/
    for (i = 0; i < domain_size; ++i) {
      free(originalpopulation[i]);
    }
    free(originalpopulation);
    originalpopulation = temp;
  }

  for (i = 0; i < populationsize; ++i) {
    imprimeVet(originalpopulation[i], 12);
  }


  if (optimal >= 0) {
    printf("\nSucesso - Optimal = %d\n", optimal);
  }


  return 0;
}

int randsingular(int *vet, int size, int edge) {
  /* Recebe um vetor e preeche com inteiros aleatorios sem repeticao */
  int i, r, size2 = 0;
  for (i = 0; i < size; ++i) {
    do {
      r = rand()%edge;
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
  /* printf("\nc = %d, a = %d, b = %d", C, A, B); */
  /* printf("\nc - a + b = %d", abs(C - (A + B))); */
  /* printf("\n100000 - abs(c - a + b) = %d\n", EVAL_REFERENCE - abs(C - (A + B))); */
  return EVAL_REFERENCE - abs(C - (A + B));
}

int roulette (int** population, int populationsize, int col, int value) {
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

int tournament (int** population, int populationsize, int tour) {
  
  int* candidates = (int *) malloc (tour * sizeof (int));
  int i, winner = 0, grade = 0, topgrade = 0;

  for (i = 0; i < tour; ++i) {
    candidates[i] = rand() % populationsize;
    grade = evalAplusBequalC (population[candidates[i]]);
    if (grade > topgrade) {
      winner = candidates[i];
    }
  }
  return winner;
}

void imprimeVet(int* vet, int size) {
  int i;
  putchar('\n');
  for (i = 0; i < size; ++i) {
    printf ("%d ", vet[i]);
  }
  putchar('\n');
}

int** init_population(int individual_size, int population_size) {
  int **temp;
  int i;

  temp = (int**) malloc (population_size * sizeof(int *));
  for(i = 0; i < population_size; ++i) {
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

int main(int argc, char** argv) {

  char stringA[] = "send";
  char stringB[] = "more";
  char stringC[] = "money";

  int   tamanho_da_populacao = 50;
  int   nro_de_geracoes = 100;
  float pcross = .6;
  float pmut = .1;
  int   semente = -1;
  int   S, C, M;
  int   nro_de_sucesso;
  int   nro_de_execs = 1000;
  float convergencia[8];

  int i, j;


  nro_de_sucesso = 0;
  j = 0;

  for (M = 1; M <= 2; M++) {
    for (C = 1; C <= 2; C++) {
      for (S = 1; S <= 2; S++) {
	for (i = 0; i < nro_de_execs; ++i) {
	  if ( ag(tamanho_da_populacao, nro_de_geracoes, pcross, pmut, S, C, M, semente) ) {
	    ++nro_de_sucesso;
	  }
	}
	convergencia[j++] = ((float) nro_de_sucesso) / nro_de_execs;
	nro_de_sucesso = 0;
      }
    }
  }

  return 0;
}

void pmx(int* parent1, int* parent2, int* son1, int* son2, int vector_size, int p1, int p2) {

  int exchange_aux, i, j, p1j, p2i, p2j;
  int conflict[vector_size];
  int found, found2;

  for(i = 0; i < vector_size; i++) {

    son1[i] = parent1[i];
    son2[i] = parent2[i];
  }

  for(i = p1, j = 0; i <= p2; i ++) {
    son1[i] = parent2[i];
    son2[i] = parent1[i];

    if(son1[i] != son2[i]) {
      conflict[j] = i;
      j++;
    }
  }

  for(i = 0; i < j; i++) {
    
    found = found2 = 0;

    for(p1j = 0; p1j < p1; p1j++) {
      if ( son1[p1j] == son1[ conflict[i] ] ) {
        son1[p1j] = son2[ conflict[i] ];
        found = 1;
        break;
      }

      if ( son2[p1j] == son2[ conflict[i] ] ) {
        son2[p1j] = son1[ conflict[i] ];
        found2 = 1;
        break;
      }
    }

    for (p1j = p2; p1j < vector_size && (!found || !found2); p1j++) {
      if ( son1[p1j] == son1[ conflict[i] ] ) {
        son1[p1j] = son2[ conflict[i] ];
        break;
      }

      if ( son2[p1j] == son2[ conflict[i] ] ) {
        son2[p1j] = son1[ conflict[i] ];
        break;
      }
    }
  }
}