#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define notaMaxima 100000

/* Auxiliares */
int    aleatorioSemRep (int *vet, int size, int edge);   /* gera individuo aleatorio, sem repeticao de inteiros */
int    funcAvaliacao (int *vet);                  
void   show (int* vet, int size);
int    contido (int* vet, int element, int size);  
void   copy (int* destino, int* origem, int size);
void   norep (int** pop, int size, int domain,int it);
void   noreplin (int* pop, int domain, int it);
char  *initMap (char* A, char* B, char* C); 
void   insereMapa (char* strconcat, int** pop, int linha);
char  *compact(char *str);

/* AG */
int  **geraPop(int individual_size, int populacao_size);
int    roleta(int** populacao, int popSize, int col, int base, int value);
int    torneio (int** populacao, int popSize, int tour);
void   ptoSimples (int** newpopulacao, int it, int** originalpopulacao, int pai1, int pai2);
void   ciclico(int** pais, int pai1, int pai2, int vector_size, int** filhos, int it);
void   pmx (int** pais, int pai1, int pai2, int vector_size, int** filhos, int it);
void   pmx2(int** pais, int pai1, int pai2, int vector_size, int** filhos, int it);
void   mutation (int** newpopulacao, int newpopSize, float mutationrate);


int alggen(int popSize, int numero_geracoes, float crossoverrate, float mutationrate, int S, int C, int M, int semente);


int
main() 
{

  int   popSize = 50, geracoes = 100;
  float pcross = .6, pmut = .1;;
  int   sucesso, semente = -1;
  int   S, C, R;
  float convergencia[8];

  char* mapa;
  char  fraseA[] = "send";
  char  fraseB[] = "more";
  char  fraseC[] = "money";

  int i, j;

  j = sucesso = 0;

  alggen(popSize, geracoes, pcross, pmut, 3, 2, 1, semente);
  /* for (R = 1; R <= 2; M++) { */
  /*   for (C = 1; C <= 2; C++) { */
  /*     for (S = 1; S <= 2; S++) { */
  /* 	for (i = 0; i < 1000; ++i) { */
  /* 	  if ( ag(popSize, geracoes, pcross, pmut, S, C, R, semente) ) { */
  /* 	    ++sucesso; */
  /* 	  } */
  /* 	} */
  /* 	convergencia[j++] = ((float) sucesso) / nro_de_execs; */
  /* 	sucesso = 0; */
  /*     } */
  /*   } */
  /* } */

  return 0;
}


int 
alggen(int popSize, int numero_geracoes, float crossoverrate, float mutationrate, int S, int C, int M, int semente) 
{

  int **originalpopulacao, **newpopulacao, newpopSize;
  int   cromossomos = 10;
  int   tour;
  int   i, j, acc,it;
  int    optimal = -1;
  int **temp;
  int base;
  int pai1, pai2;
  int x, y, valor_maior, maior, matrix;
  char *mapa;

  newpopSize = (popSize -1) * crossoverrate;
  popSize = popSize + 1; //ultima linha serah o mapa

  /* srand( (unsigned int) (semente < 0 ? time(NULL) : semente) ); */
  srand(time(NULL));


  // Gera Pop
  originalpopulacao = geraPop(cromossomos, popSize);
  newpopulacao = geraPop(cromossomos, popSize);
	
  // Preenche Pop
  for(i = 0; i < popSize; ++i) {
    aleatorioSemRep(originalpopulacao[i], cromossomos, cromossomos);
  }

  // Insere Mapa na ultima linha
  /* mapa = initMap (strA, strB, strC); */
  /* insereMapa (mapa, originalpopulacao, popSize); */

  // Avalia Pop
  acc = 0;
  for (i = 0; i < popSize; ++i) {
    acc += originalpopulacao[i][cromossomos] = funcAvaliacao (originalpopulacao[i]);

    if (originalpopulacao[i][cromossomos] == notaMaxima) {
      optimal = i;
    }

    originalpopulacao[i][11] = acc;
    //show(originalpopulacao[i], 12);
  }
  base = originalpopulacao[0][11];
  //getchar();
  
  for (j = 0; (j < numero_geracoes) && (optimal < 0); ++j) {
    
    for (it = 0; it < newpopSize / 2; it++) {

      /* printf("\nTOPO it\n"); */
      /* norep(originalpopulacao , popSize, 10, it); */
      //*************************** SELECAO ******************************
      if (S == 1) {
	// Roleta 
	pai1 = roleta (originalpopulacao, popSize, cromossomos + 1, base, acc);
	pai2 = roleta (originalpopulacao, popSize, cromossomos + 1, base, acc);
      }
      else if (S == 2 || S == 3) {
	// Torneio Simples
	tour = (S == 2) ? 3 : 2;

	pai1 = torneio (originalpopulacao, popSize, tour);
	/* show(originalpopulacao[pai1],10); */
	pai2 = torneio (originalpopulacao, popSize, tour);
	/* show(originalpopulacao[pai2],10); */
      }
      //******************************************************************
      printf("\n%d",pai1);
      printf("\n%d",pai2);

      /*************************** CROSSOVER ******************************/
      //Ciclico
      if (C == 1) {
	/* ptoSimples(newpopulacao, it, originalpopulacao, pai1, pai2); */
	ciclico(originalpopulacao, pai1, pai2, cromossomos, newpopulacao, it);
      }
      if (C == 2) {
	pmx2(originalpopulacao, pai1, pai2, cromossomos, newpopulacao, it);
      }
      printf("\npai\n");
      noreplin(originalpopulacao[pai1], 10, it);
      noreplin(originalpopulacao[pai2] , 10, it);
      printf("\nCross i\n");
      noreplin(newpopulacao[2*it], 10, it);
      noreplin(newpopulacao[2*it + 1] , 10, it);
      /********************************************************************/

      /* show(originalpopulacao[pai1], 10); */
      /* show(originalpopulacao[pai2], 10); */
      /* putchar('\n'); */
      /* show(newpopulacao[2*it], 10); */
      /* show(newpopulacao[2*it+1], 10); */
      /* putchar('\n'); */
      /* getchar(); */
    }
		
    /*************************** MUTACAO ******************************/
    mutation(newpopulacao, newpopSize, mutationrate);
    /******************************************************************/
    printf("\nPERMUT j\n");
    norep(newpopulacao , newpopSize, 10, j);

    /***** AVALIA os novos indiv. apos Crossover e Mutacao ************/
    acc = 0;
    for (i = 0; i < newpopSize; ++i) {
      acc += newpopulacao[i][10] = funcAvaliacao (newpopulacao[i]);
      newpopulacao[i][11] = acc;
      /* show(newpopulacao[i], 12); */
    }
    /******************************************************************/

    printf("\nEVAL j\n");
    norep(newpopulacao , newpopSize, 10, j);


    /****** SELECIONA os melhores indiv. dentre os pais e filhos ******/

    temp = geraPop(cromossomos, popSize);
    //geraPop(10);

    valor_maior = -1;
    maior = 0, matrix = 0;
    for(i = 0; i < popSize; i++) {

      for(x = 0; x < popSize; ++x) {
        if(originalpopulacao[x][10] > valor_maior) {
	  maior = x;
	  matrix = 0;
	  valor_maior = originalpopulacao[maior][10];
        }
      }

      for(y = 0; y < newpopSize; ++y) {
        if(newpopulacao[y][10] > valor_maior){
	  maior = y;
	  matrix = 1;
	  valor_maior = newpopulacao[y][10];
        }
      }

      if (matrix) {
	printf("\nnewpop\n");
	noreplin(newpopulacao[maior], 10, i);
        copy(temp[i], newpopulacao[maior], 12);
	newpopulacao[maior][10] = -1;
	valor_maior = -1;
	if (temp[i][10] == notaMaxima) {
	  optimal = i;
	}

      }
      else {
	printf("\norgpop\n");
	noreplin(originalpopulacao[maior], 10, i);
        copy(temp[i], originalpopulacao[maior], 12);
	originalpopulacao[maior][10] = -1;
	valor_maior = -1;
	if (temp[i][10] == notaMaxima) {
	  optimal = i;
	}

      }
      printf("\nSEL2 i\n");
      noreplin(temp[i] , 10, i);

    }
    /****************************************************************/
    for (i = 0; i < cromossomos; ++i) {
      free(originalpopulacao[i]);
    }
    free(originalpopulacao);
    originalpopulacao = temp;

    /* printf("\nBOT j\n"); */
    /* norep(originalpopulacao , popSize, 10, j); */


  }

  for (i = 0; i < popSize; ++i) {
    show(originalpopulacao[i], 12);
  }


  if (optimal >= 0) {
    printf("\nSucesso - Optimal = %d\n", optimal);
  }


  return 0;
}

int
aleatorioSemRep(int *vet, int size, int edge) 
{
  /* Recebe um vetor e preeche com inteiros aleatorios sem repeticao */
  int i, r, size2 = 0;
  for (i = 0; i < size; ++i) {
    do {
      r = rand()%edge;
    } while (contido(vet, r, size2));
    vet[i] = r;
    ++size2;
  }
}

int
contido (int* vet, int element, int size) 
{
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

char* 
initMap (char* A, char* B, char* C) 
{
  int    size = (int) (strlen(A) + strlen(B) + strlen(C));
  char  *map = (char*) malloc (size * sizeof(char));

  strcpy (map, A);
  strcat (map, B);
  strcat (map, C);
  map = compact (map);

  return map;
}

int
funcAvaliacao (int* vet) 
//funcAvaliacao (int* vet, int** population, int mapline) 
{
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
  /* printf("\n100000 - abs(c - a + b) = %d\n", notaMaxima - abs(C - (A + B))); */
  return notaMaxima - abs(C - (A + B));
}

int
roleta (int** populacao, int popSize, int col, int base, int value) 
{
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
  for (i = 0; i < popSize; ++i) {
    if (rnd < populacao[i][col]) {
      return i;
    }
  }
  /* printf("\ni = %d",--i); */
  /* getchar(); */
  return --i;
}

int
torneio (int** populacao, int popSize, int tour) 
{
  
  int* candidates = (int *) malloc (tour * sizeof (int));
  int i, winner = 0, grade = 0, topgrade = 0;

  for (i = 0; i < tour; ++i) {
    candidates[i] = rand() % popSize;
    show(populacao[candidates[i]],10);
    grade = funcAvaliacao (populacao[candidates[i]]);
    printf("i = %d", i);
    printf("\ncandidates[i] = %d", candidates[i]);
    printf("\ngrade = %d", grade);
    if (grade > topgrade) {
      topgrade = grade;
      winner = candidates[i];
    }
  }
  printf("\nVencedor: ");
  show(populacao[winner],10);
  printf("\nwinner = %d\n", winner);
  return winner;
}

void
show(int* vet, int size) 
{
  int i;
  putchar('\n');
  for (i = 0; i < size; ++i) {
    printf ("%d ", vet[i]);
  }
  putchar('\n');
}

int** 
geraPop(int individual_size, int populacao_size) 
{
  int **temp;
  int i;

  temp = (int**) malloc (populacao_size * sizeof(int *));
  for(i = 0; i < populacao_size; ++i) {
    temp[i] = (int*) malloc ((individual_size + 2) * sizeof(int));
  }

  return temp;
}

void
ptoSimples(int** newpopulacao, int it, int** originalpopulacao, int pai1, int pai2) 
{
  int i, corte = rand() % 10;
  it *= 2;
  for (i = 0; i < corte; ++i) {
    newpopulacao[it][i] = originalpopulacao[pai1][i];
    newpopulacao[it + 1][i] = originalpopulacao[pai2][i];
  }
  for (i = corte; i < 10; ++i) {
    newpopulacao[it][i] = originalpopulacao[pai2][i];
    newpopulacao[it + 1][i] = originalpopulacao[pai1][i];
  }
}

void
mutation(int** newpopulacao, int newpopSize, float mutationrate) 
{
  int i, aux, qtd = (int) (newpopSize * mutationrate);
  int rnd, rnd1, rnd2;

  for (i = 0; i < qtd; ++i) {
    rnd  = rand() % newpopSize;
    rnd1 = rand() % 10;
    rnd2 = rand() % 10;

    aux = newpopulacao[rnd][rnd1];
    newpopulacao[rnd][rnd1] = newpopulacao[rnd][rnd2];
    newpopulacao[rnd][rnd2] = aux;
  }

}

void
pmx2(int** pais, int pai1, int pai2, int vector_size, int** filhos, int it) {

  int r1 = rand () % vector_size;
  int r2 = rand () % vector_size;
  int i,j, aux;
  int conf1[vector_size], size1 = 0;
  int conf2[vector_size], size2 = 0;

  if (r1 > r2)
    {
      aux = r1;
      r1 = r2;
      r2 = aux;
    }

  it *= 2;

  copy(filhos[it], pais[pai1], 12);
  copy(filhos[it + 1], pais[pai2], 12);

  for (i = r1; i <= r2; ++i)
    {
      aux = filhos[it][i];
      filhos[it][i] = filhos[it + 1][i];
      filhos[it + 1][i] = aux;
    }

  // Saporra acha os conflitos do filho 1
  for (i = r1; i != r2; ++i)
    {
      for (j = 0; j < vector_size; ++j)
	{
	  if (j >= r1 && j <= r2) 
	    {
	      continue;
	    }
	  else if (filhos[it][i] == filhos[it][j]) 
	    {
	      conf1[size1++] = filhos[it][i];
	    }
	}
    }
  // Saporra acha os conflitos do filho 2
  for (i = r1; i != r2; ++i)
    {
      for (j = 0; j < vector_size; ++j)
	{
	  if (j >= r1 && j <= r2) 
	    {
	      continue;
	    }
	  else if (filhos[it + 1][i] == filhos[it + 1][j]) 
	    {
	      conf2[size2++] = filhos[it + 1][i];
	    }
	}
    }

  // Verifica erro do PMX
  if (size1 != size2)
    {
      printf ("\nERRO NO PMX2- Tamanhos de conflitos diferentes\n");
      getchar ();
    }

  // Troca os conflitos de filho
  for (i = 0; i < size1; ++i)
    {
      aux = filhos[it][ conf1[i] ];
      filhos[it][ conf1[i] ] = filhos[it + 1][ conf2[i] ];
      filhos[it + 1][ conf2[i] ] = aux;
    }

}

void
pmx(int** pais, int pai1, int pai2, int vector_size, int** filhos, int it) 
{

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
      filhos[it][i] = pais[pai2][i];
      filhos[it + 1][i] = pais[pai1][i];
    }
    else {
      filhos[it][i] = pais[pai1][i];
      filhos[it + 1][i] = pais[pai2][i];
    }
  }

  count1 = count2 = 0;

  for (i = 0; i < p1; i++) {
    if (contido (&filhos[it][p1], filhos[it][i],p2-p1+1)) {
      conflict [0][count1++] = i;
    }

    if (contido (&filhos[it + 1][p1], filhos[it + 1][i],p2-p1+1)) {
      conflict [1][count2++] = i;
    }
  }

  for (i = p2+1; i < vector_size; i++) {
    if (contido (&filhos[it][p1], filhos[it][i],p2-p1+1)) {
      conflict [0][count1++] = i;
    }

    if (contido (&filhos[it + 1][p1], filhos[it + 1][i],p2-p1+1)) {
      conflict [1][count2++] = i;
    }
  }

  for(i = 0; i < count1; i++) {
    aux = filhos[it][conflict[0][i]];
    filhos[it][conflict[0][i]] = filhos[it + 1][conflict[1][i]];
    filhos[it + 1][conflict[1][i]] = aux;
  }

  
  /* show(pais[pai1], vector_size); */
  /* show(pais[pai2], vector_size); */
  /* printf("\np1 = %d", p1); */
  /* printf("\np2 = %d", p2); */
  /* show(conflict[0], count1); */
  /* show(conflict[1], count2); */
  /* show(filhos[it], vector_size); */
  /* show(filhos[it+1], vector_size); */
  /* getchar(); */

}

void
norep(int** pop, int size, int domain, int it) 
{
  int j,i;
  for (i = 0; i < size; ++i) {
    for (j = 0; j < domain - 1; ++j) {
      if(contido(&pop[i][1+j], pop[i][j],domain - j - 1)) {
	printf("\nit = %d", it);
	show(pop[i], 10);
	getchar();
      }
    }
  }
}

void
noreplin(int* pop, int domain, int it) 
{
  int j,i;
  for (j = 0; j < domain - 1; ++j) {
    if(contido(&pop[1+j], pop[j],domain - j - 1)) {
      printf("\nit = %d", it);
      show(pop, 10);
      getchar();
    }
  }

}

void
ciclico(int** pais, int pai1, int pai2, int vector_size, int** filhos, int it) 
{

  int r = rand() % vector_size;
  int ref, i, j, k, aux,prox;
  int trocas[vector_size];
  int trocas_size = 0;
  it *= 2;

  copy(filhos[it], pais[pai1], 12);
  copy(filhos[it + 1], pais[pai2], 12);

  prox = ref = filhos[it][0];

  do {
    for (i = 0; i < vector_size; ++i) {
      if (filhos[it][i] == prox) {

	prox = filhos[it + 1][i];

	aux = filhos[it][i];
	filhos[it][i] = filhos[it + 1][i];
	filhos[it + 1][i] = aux;

	break;
      }
    }
  } while (prox != ref); 

}

void
insereMapa (char* strconcat, int** pop, int linha) 
{
  int i, size = strlen (strconcat);
  printf("\nSize = %d", size);
  for (i = 0; i < size; ++i)
    {
      pop[linha][i] = (int)strconcat[i];
    }

}


char * 
compact(char *str)
{
  int  len = strlen(str);
  int  alphabet[26];
  int  find, i=0, j=0;
  char temp[len];
  char *temp2;

  while(str[i] != '\0')
    {
      find = str[i] - 'a';

      if(alphabet[find] != 1)
	{
	  alphabet[find] = 1;
	  temp[j] = str[i];
	  j++;
	}

      i++;
    }

  temp2 = (char *)malloc(sizeof(char) * j);

  for(i = 0; i < j; i++)
    {
      temp2[i] = temp[i];
    }

  temp2[j] = '\0';

  return temp2;
}
