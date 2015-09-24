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
int    roulette(int** population, int populationsize, int col, int base, int value);
int    tournament (int** population, int populationsize, int tour);
void   imprimeVet(int* vet, int size);
int**  init_population(int individual_size, int population_size);
void   crossover_simple (int** newpopulation, int it, int** originalpopulation, int index_pai1, int index_pai2);
void   pmx (int** parent, int pai1, int pai2, int vector_size, int** son, int it);
void   permutation (int** newpopulation, int newpopulationsize, float mutationrate);
void conflito(int** pop, int size, int domain,int it);
void conflitolin(int* pop, int domain, int it);

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

  int** temp;
  int base;
  int index_pai1, index_pai2;
  int x, y, valor_maior, maior, matrix;

  newpopulationsize = populationsize * crossoverrate;

  /* srand( (unsigned int) (semente < 0 ? time(NULL) : semente) ); */
  srand(time(NULL));

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
  base = originalpopulation[0][11];
  //getchar();
  // LASSO
  
  for (j = 0; (j < numero_geracoes) && (optimal < 0); ++j) {
    
    for (it = 0; it < newpopulationsize / 2; it++) {

      /* printf("\nTOPO it\n"); */
      /* conflito(originalpopulation , populationsize, 10, it); */
      //*************************** SELECAO ******************************
      if (S == 1) {
	// Roleta 
	index_pai1 = roulette (originalpopulation, populationsize, domain_size + 1, base, acc);
	index_pai2 = roulette (originalpopulation, populationsize, domain_size + 1, base, acc);
      }
      else if (S == 2) {
	// Torneio Simples
	index_pai1 = tournament (originalpopulation, populationsize, tour);
	imprimeVet(originalpopulation[index_pai1],10);
	index_pai2 = tournament (originalpopulation, populationsize, tour);
	imprimeVet(originalpopulation[index_pai2],10);
      }

      //******************************************************************
      printf("\n%d",index_pai1);
      printf("\n%d",index_pai2);
      /*************************** CROSSOVER ******************************/

      //Ciclico
      if (C == 1) {
	crossover_simple(newpopulation, it, originalpopulation, index_pai1, index_pai2);
      }
      if (C == 2) {
	pmx(originalpopulation, index_pai1, index_pai2, domain_size, newpopulation, it);
      }
      /********************************************************************/

      /* imprimeVet(originalpopulation[index_pai1], 10); */
      /* imprimeVet(originalpopulation[index_pai2], 10); */
      /* putchar('\n'); */
      /* imprimeVet(newpopulation[2*it], 10); */
      /* imprimeVet(newpopulation[2*it+1], 10); */
      /* putchar('\n'); */
      /* getchar(); */
    }
		
    /*************************** MUTACAO ******************************/
    permutation(newpopulation, newpopulationsize, mutationrate);
    /******************************************************************/
    /* printf("\nPERMUT j\n"); */
    /* conflito(originalpopulation , populationsize, 10, j); */

    /***** AVALIA os novos indiv. apos Crossover e Mutacao ************/
    acc = 0;
    for (i = 0; i < newpopulationsize; ++i) {
      acc += newpopulation[i][10] = evalAplusBequalC (newpopulation[i]);
      newpopulation[i][11] = acc;
      /* imprimeVet(newpopulation[i], 12); */
    }
    /******************************************************************/

    /* printf("\nEVAL j\n"); */
    /* conflito(originalpopulation , populationsize, 10, j); */


    /****** SELECIONA os melhores indiv. dentre os pais e filhos ******/

    temp = init_population(domain_size, populationsize);
    //init_population(10);

    valor_maior = -1;
    maior = 0, matrix = 0;
    for(i = 0; i < populationsize; i++) {

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
	newpopulation[maior][10] = -1;
	valor_maior = -1;
	if (temp[i][10] == EVAL_REFERENCE) {
	  optimal = i;
	}

      }
      else {
        copy(temp[i], originalpopulation[maior], 12);
	originalpopulation[maior][10] = -1;
	valor_maior = -1;
	if (temp[i][10] == EVAL_REFERENCE) {
	  optimal = i;
	}

      }
      printf("\nSEL2 i\n");
      conflitolin(temp[i] , 10, i);

    }
    /****************************************************************/
    for (i = 0; i < domain_size; ++i) {
      free(originalpopulation[i]);
    }
    free(originalpopulation);
    originalpopulation = temp;

    /* printf("\nBOT j\n"); */
    /* conflito(originalpopulation , populationsize, 10, j); */


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

int roulette (int** population, int populationsize, int col, int base, int value) {
  /* ### ROLETA ### 
   * INPUT:  recebe a matriz Populacao, seu tamanho, a coluna das avalicoes acumuladas e o valor total de fichas distribuidas;
   * OUTPUT: retorna o individuo da ficha que foi sorteada;  
   */
  int i, token;
  int rnd = (rand() + base ) % value;
  /* (rnd < 0) ? rnd *= -1 : rnd; */

  /* printf("\ncol = %d",col); */
  /* printf("\nbase = %d",base); */
  /* printf("\nacc = %d",value); */
  /* printf("\nrnd = %d",rnd); */
  for (i = 0; i < populationsize; ++i) {
    if (rnd < population[i][col]) {
      return i;
    }
  }
  /* printf("\ni = %d",--i); */
  /* getchar(); */
  return --i;
}

int tournament (int** population, int populationsize, int tour) {
  
  int* candidates = (int *) malloc (tour * sizeof (int));
  int i, winner = 0, grade = 0, topgrade = 0;

  for (i = 0; i < tour; ++i) {
    candidates[i] = rand() % populationsize;
    imprimeVet(population[candidates[i]],10);
    grade = evalAplusBequalC (population[candidates[i]]);
    printf("i = %d", i);
    printf("\ncandidates[i] = %d", candidates[i]);
    printf("\ngrade = %d", grade);
    if (grade > topgrade) {
      topgrade = grade;
      winner = candidates[i];
    }
  }
  printf("\nVencedor: ");
  imprimeVet(population[winner],10);
  printf("\nwinner = %d\n", winner);
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
  int i, aux, qtd = (int) (newpopulationsize * mutationrate);
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

void pmx(int** parent, int pai1, int pai2, int vector_size, int** son, int it) {

  int count1, count2, aux, i, j;
  int conflict[vector_size][2];
  
  int p1 = rand () % vector_size;
  int p2 = rand () % vector_size;

  if (p1 > p2) {
    aux = p1;
    p1 = p2;
    p2 = aux;
  }

  it *= 2;

  for (i = 0; i < vector_size; i++) {
    if (i >= p1 && i <= p2) {
      son[it][i] = parent[pai2][i];
      son[it + 1][i] = parent[pai1][i];
    }
    else {
      son[it][i] = parent[pai1][i];
      son[it + 1][i] = parent[pai2][i];
    }
  }

  count1 = count2 = 0;

  for (i = 0; i < p1; i++) {
    if (contain (&son[it][p1], son[it][i],p2-p1+1)) {
      conflict [0][count1++] = i;
    }

    if (contain (&son[it + 1][p1], son[it + 1][i],p2-p1+1)) {
      conflict [1][count2++] = i;
    }
  }

  for (i = p2+1; i < vector_size; i++) {
    if (contain (&son[it][p1], son[it][i],p2-p1+1)) {
      conflict [0][count1++] = i;
    }

    if (contain (&son[it + 1][p1], son[it + 1][i],p2-p1+1)) {
      conflict [1][count2++] = i;
    }
  }

  for(i = 0; i < count1; i++) {
    aux = son[it][conflict[0][i]];
    son[it][conflict[0][i]] = son[it + 1][conflict[1][i]];
    son[it + 1][conflict[1][i]] = aux;
  }

  /*
  imprimeVet(parent[pai1], vector_size);
  imprimeVet(parent[pai2], vector_size);
  printf("\np1 = %d", p1);
  printf("\np2 = %d", p2);
  imprimeVet(conflict[0], count1);
  imprimeVet(conflict[1], count2);
  imprimeVet(son[it], vector_size);
  imprimeVet(son[it+1], vector_size);
  getchar();
  */

}

void conflito(int** pop, int size, int domain, int it) {
  int j,i;
  for (i = 0; i < size; ++i) {
    for (j = 0; j < domain - 1; ++j) {
      if(contain(&pop[i][1+j], pop[i][j],domain - j - 1)) {
	printf("\nit = %d", it);
	imprimeVet(pop[i], 10);
	getchar();
      }
    }
  }
}

void conflitolin(int* pop, int domain, int it) {
  int j,i;
  for (j = 0; j < domain - 1; ++j) {
    if(contain(&pop[1+j], pop[j],domain - j - 1)) {
      printf("\nit = %d", it);
      imprimeVet(pop, 10);
      getchar();
    }
  }

}

int main(int argc, char** argv) {

  char stringA[] = "send";
  char stringB[] = "more";
  char stringC[] = "money";

  int   tamanho_da_populacao = 50;
  int   nro_de_geracoes = 100;
  float pcross = .6;
  float pmut = .1;
  int   semente = -1;
  int   S, C, R;
  int   nro_de_sucesso;
  int   nro_de_execs = 1000;
  float convergencia[8];

  int i, j;


  nro_de_sucesso = 0;
  j = 0;
  ag(tamanho_da_populacao, nro_de_geracoes, pcross, pmut, 1, 2, 1, semente);
  /* for (R = 1; R <= 2; M++) { */
  /*   for (C = 1; C <= 2; C++) { */
  /*     for (S = 1; S <= 2; S++) { */
  /* 	for (i = 0; i < nro_de_execs; ++i) { */
  /* 	  if ( ag(tamanho_da_populacao, nro_de_geracoes, pcross, pmut, S, C, R, semente) ) { */
  /* 	    ++nro_de_sucesso; */
  /* 	  } */
  /* 	} */
  /* 	convergencia[j++] = ((float) nro_de_sucesso) / nro_de_execs; */
  /* 	nro_de_sucesso = 0; */
  /*     } */
  /*   } */
  /* } */

  return 0;
}

