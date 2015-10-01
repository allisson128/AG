#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

#define notaMaxima 100000

// Auxiliares 
int   aleatorioSemRep (int *vet, int size, int edge);   
int   aval (int *vet, char *mapa, char *strA, char *strB, char *strC);
void  show (int *vet, int size);
int   contido (int *vet, int element, int size);  
void  copy (int *destino, int *origem, int size);
void  norep (int **pop, int size, int domain,int it);
void  noreplin (int *pop, int domain, int it);
char *initMap (char *A, char *B, char *C); 
void  insereMapa (char* strconcat, int **pop, int linha);
char *compact (char *str);

// AG
int  **geraPop (int individual_size, int populacao_size);
int  roleta (int **populacao, int popSize, int col, int base, int value);
int  torneio (int **populacao, int popSize, int tour, char *mapa, char *A, 
	      char *B, char *C);
void ptoSimples (int **newpopulacao, int **originalpopulacao, int it, 
		 int pai1, int pai2);
void ciclico (int **pais, int pai1, int pai2, int vector_size, int **filhos, 
	      int it);
void pmx (int **pais, int pai1, int pai2, int vector_size, int **filhos, int it);
void mutation (int **newpopulacao, int newpopSize, float mutationrate);
int  **reinsercao_ordenada (int **originalpopulacao, int popSize, 
			    int **newpopulacao, int newpopSize, int *optimal);
int  alggen (int popSize, int numero_geracoes, float crossoverrate, 
	     float mutationrate, int S, int C, int R, char *strA, 
	     char *strB, char *strC, int semente);


int
main (int argc, char **argv) 
{

  int   popSize      = 50;
  int   geracoes     = 100;
  float pcross       = 0.6;
  float pmut         = 0.1;;
  int   sucesso      = 0;
  int   nro_de_execs = 1000;
  int   semente      = -1;
  int   S, C, R;
  float convergencia[12];

  char *mapa;
  char  fraseA[] = "coca";
  char  fraseB[] = "cola";
  char  fraseC[] = "oasis";

  int i;
  int j = 0;

  if (argc == 4) 
    {
      S = atoi(argv[1]);
      C = atoi(argv[2]);
      R = atoi(argv[3]);
      alggen (popSize, geracoes, pcross, pmut, S, C, R, fraseA, fraseB, fraseC, semente);
    }
  else 
    {
      for (R = 1; R <= 2; R++) 
	{
	  for (C = 1; C <= 2; C++)
	    {
	      for (S = 1; S <= 3; S++) 
		{
		  for (i = 0; i < nro_de_execs; ++i) 
		    {
		      if ( alggen (popSize, geracoes, pcross, pmut, S, C, R, 
				   fraseA, fraseB, fraseC, semente) ) 
			{
			  ++sucesso;
			  getchar ();
			}
		    }
		  convergencia[j++] = ((float) sucesso) / nro_de_execs;
		  sucesso = 0;
		}
	    }
	}
    }
  return 0;
}


int 
alggen (int popSize, int numero_geracoes, float crossoverrate, float mutationrate, int S, int C, int R, char *strA, char *strB, char *strC, int semente) 
{

  int   **originalpopulacao, **newpopulacao, newpopSize;
  int   nrocromossomos = 10;
  int   tour;
  int   i, j, acc,it;
  int   optimal = -1;
  int   **temp;
  int   base;
  int   pai1, pai2;
  char  *mapa;

  newpopSize = popSize * crossoverrate;
  newpopSize = newpopSize + newpopSize % 2; //parifica

  /* srand( (unsigned int) (semente < 0 ? time(NULL) : semente) ); */
  srand (time (NULL));


  // Gera Pop
  originalpopulacao = geraPop (nrocromossomos, popSize);
  newpopulacao = geraPop (nrocromossomos, popSize);
	
  // Preenche Pop
  for (i = 0; i < popSize; ++i) 
    {
      aleatorioSemRep (originalpopulacao[i], nrocromossomos, nrocromossomos);
    }

  // Insere Mapa na ultima linha
  mapa = initMap (strA, strB, strC);
  //printf ("\nmapa = %s", mapa);
  //insereMapa (mapa, originalpopulacao, popSize);

  // Avalia Pop
  acc = 0;
  for (i = 0; i < popSize; ++i) 
    {
      acc += originalpopulacao[i][nrocromossomos] = aval (originalpopulacao[i], 
							  mapa, strA, strB, strC);

      if (originalpopulacao[i][nrocromossomos] == notaMaxima) 
	{
	  optimal = i;
	}

      originalpopulacao[i][11] = acc;
      /* show(originalpopulacao[i], 12); */
    }
  base = originalpopulacao[0][11];

  for (j = 0; (j < numero_geracoes) && (optimal < 0); ++j) 
    {
    
      for (it = 0; it < (newpopSize / 2); ++it) 
	{

	  //*************************** SELECAO ******************************
	  if (S == 1) 
	    {
	      // Roleta 
	      pai1 = roleta (originalpopulacao, popSize, nrocromossomos + 1, base, acc);
	      pai2 = roleta (originalpopulacao, popSize, nrocromossomos + 1, base, acc);
	    }
	  else if (S == 2 || S == 3) 
	    {
	      // Torneio Simples
	      tour = (S == 2) ? 3 : 2;

	      pai1 = torneio (originalpopulacao, popSize, tour, mapa, 
			      strA, strB, strC);
	      pai2 = torneio (originalpopulacao, popSize, tour, mapa,
			      strA, strB, strC);
	    }
	  else
	    {
	      printf ("\nErro na Selecao! Entrada invalida.\n");
	      getchar ();
	      exit (1);
	    }

	  /*************************** CROSSOVER ******************************/
	  //Ciclico
	  if (C == 1) 
	    {
	      /* ptoSimples(newpopulacao, it, originalpopulacao, pai1, pai2); */
	      ciclico (originalpopulacao, pai1, pai2, nrocromossomos, newpopulacao, it);
	    }
	  if (C == 2) 
	    {
	      pmx (originalpopulacao, pai1, pai2, nrocromossomos, newpopulacao, it);
	    }
	}
		
      /*************************** MUTACAO ******************************/
      mutation(newpopulacao, newpopSize, mutationrate);

      norep (newpopulacao, newpopSize, 10, j);

      /***** AVALIA os novos indiv. apos Crossover e Mutacao ************/
      acc = 0;

      for (i = 0; i < newpopSize; ++i) 
	{
	  acc += newpopulacao[i][10] = aval (newpopulacao[i], mapa, strA, strB, strC);
	  newpopulacao[i][11] = acc;
	}
      /******************************************************************/


      /****************** REINSERCAO ************************************/

      if (R == 1)
	{
	  originalpopulacao = reinsercao_ordenada (originalpopulacao, popSize,
						   newpopulacao, newpopSize, 
						   &optimal);
	}
      else if (R == 2)
	{
	  //reinsercao_pura_elistismo ();
	}
      else 
	{
	  printf ("\nArgumento invalido!\n");
	  getchar ();
	  exit(1);
	};
    }

  if (optimal >= 0) 
    {
      printf ("\nSucesso - Optimal = %d\n", optimal);
      show (originalpopulacao[0], 11);
      return 1;
    }


  return 0;
}

int
aleatorioSemRep(int *vet, int size, int edge) 
{
  /* Recebe um vetor e preeche com inteiros aleatorios sem repeticao */
  int i, r, size2 = 0;
  for (i = 0; i < size; ++i) 
    {
      do
	{
	  r = rand()%edge;
	} 
      while (contido (vet, r, size2) >= 0);
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
      return i;
    }
    ++i;
  }
  return -1;
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
  //printf ("\nmap = %s", map);
  return map;
}

int
aval (int* vet, char* mapa, char *strA, char *strB, char *strC) 
{
  int A = 0;
  int B = 0;
  int C = 0;
  int i, j, k;
  int mapasize;

  mapasize = strlen (mapa);

  for (i = strlen (strA) - 1, j = 0; i >= 0; --i, ++j) 
    {
      k = 0;
      while (mapa[k] != strA[i]) { ++k; }
      A += vet[k] * pow (10, j);
    }

  for (i = strlen (strB) - 1, j = 0; i >= 0; --i, ++j) 
    {
      k = 0;
      while (mapa[k] != strB[i]) { ++k; }
      B += vet[k] * pow (10, j);
    }

  for (i = strlen (strC) - 1, j = 0; i >= 0; --i, ++j) 
    {
      k = 0;
      while (mapa[k] != strC[i]) { ++k; }
      C += vet[k] * pow (10, j);
    }

  /* if (A + B == C) { */
  /*   printf("\n\n!!! ENCONTROU !!!\nSend + More = Money\n\n"); */
  /* } */
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
torneio (int **populacao, int popSize, int tour,
	 char *mapa, char *A, char *B, char *C)
{
  
  int* candidates = (int *) malloc (tour * sizeof (int));
  int i, winner = 0, grade = 0, topgrade = 0;

  for (i = 0; i < tour; ++i) {
    candidates[i] = rand() % popSize;
    grade = aval (populacao[candidates[i]], mapa, A, B, C);
    /* printf("\ni = %d", i); */
    /* show(populacao[candidates[i]],10); */
    /* printf("\ncandidates[i] = %d", candidates[i]); */
    /* printf("\ngrade = %d", grade); */
    if (grade > topgrade) {
      topgrade = grade;
      winner = candidates[i];
    }
  }
  /* printf("\nVencedor: "); */
  /* show(populacao[winner],10); */
  /* printf("\nwinner = %d\n", winner); */
  return winner;
}

void
show (int *vet, int size) 
{
  int i;
  putchar('\n');
  for (i = 0; i < size; ++i) {
    printf ("%d ", vet[i]);
  }
  putchar('\n');
}

int** 
geraPop (int individual_size, int populacao_size) 
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
ptoSimples (int **newpopulacao, int **originalpopulacao, int it, int pai1, int pai2) 
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
mutation (int **newpopulacao, int newpopSize, float mutationrate) 
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
pmx (int **pais, int pai1, int pai2, int vector_size, int **filhos, int it) 
{

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

  /* printf ("\n******************** PMX2 **************************"); */
  /* printf ("\nFilhos Originais"); */
  /* show (filhos[it], 12); */
  /* show (filhos[it + 1], 12); */

  // Troca o miolo dos filhos
  for (i = r1; i <= r2; ++i)
    {
      aux = filhos[it][i];
      filhos[it][i] = filhos[it + 1][i];
      filhos[it + 1][i] = aux;
    }

  /* printf ("\nMiolos trocados"); */
  /* show (filhos[it], 12); */
  /* show (filhos[it + 1], 12); */

  // Saporra acha os conflitos do filho 1
  for (i = r1; i <= r2; ++i)
    {
      for (j = 0; j < vector_size; ++j)
	{
	  if (j >= r1 && j <= r2) 
	    {
	      continue;
	    }
	  else if (filhos[it][i] == filhos[it][j]) 
	    {
	      conf1[size1++] = j;
	    }
  	}
    }
  // Saporra acha os conflitos do filho 2
  for (i = r1; i <= r2; ++i)
    {
      for (j = 0; j < vector_size; ++j)
	{
	  if (j >= r1 && j <= r2) 
	    {
	      continue;
	    }
	  else if (filhos[it + 1][i] == filhos[it + 1][j]) 
	    {
	      conf2[size2++] = j;
	    }
	}
    }

  // Verifica erro do PMX
  if (size1 != size2)
    {
      printf ("\nERRO NO PMX2- Tamanhos de conflitos diferentes\n");
      getchar ();
    }

  /* printf ("\nConflitos:"); */
  /* show (conf1, size1); */
  /* show (conf2, size2); */

  // Troca os conflitos de filho
  for (i = 0; i < size1; ++i)
    {
      aux = filhos[it][ conf1[i] ];
      filhos[it][ conf1[i] ] = filhos[it + 1][ conf2[i] ];
      filhos[it + 1][ conf2[i] ] = aux;
    }

  /* printf ("\nConflitos Trocados:"); */
  /* show (filhos[it], 12); */
  /* show (filhos[it + 1], 12); */
}

void
norep (int **pop, int size, int domain, int it) 
{
  int j,i;
  for (i = 0; i < size; ++i) 
    {
      for (j = 0; j < domain - 1; ++j) 
	{
	  if (contido (&pop[i][1+j], pop[i][j], domain - j - 1) >= 0) 
	    {
	      printf ("\nnewpopsize = %d", size);
	      printf ("\nlinha %d", i);
	      printf ("\nit = %d", it);
	      show (pop[i], 12);
	      getchar ();
	    }
	}
    }
}

void
noreplin (int *pop, int domain, int it) 
{
  int j,i;
  for (j = 0; j < domain - 1; ++j) 
    {
      if (contido (&pop[1+j], pop[j], domain - j - 1) >= 0) 
	{
	  printf ("\nit = %d", it);
	  show (pop, 10);
	  getchar ();
	}
    }

}

void
ciclico (int **pais, int pai1, int pai2, int vector_size, int **filhos, int it)
{

  int r = rand() % vector_size;
  int ref, i, j, k, aux,prox;
  int trocas[vector_size];
  int trocas_size = 0;

  it *= 2;
  copy(filhos[it], pais[pai1], 12);
  copy(filhos[it + 1], pais[pai2], 12);

  prox = ref = filhos[it][0];

  do 
    {
      for (i = 0; i < vector_size; ++i) 
	{
	  if (filhos[it][i] == prox) 
	    {
	      prox = filhos[it + 1][i];
	      aux = filhos[it][i];
	      filhos[it][i] = filhos[it + 1][i];
	      filhos[it + 1][i] = aux;
	      break;
	    }
	}
    } 
  while (prox != ref); 

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
compact (char *str)
{
  int  len = strlen(str);
  int  alphabet[26];
  int  find, i=0, j=0;
  char temp[len + 1];
  char *temp2;

  for (i = 0; i < 26; ++i) { alphabet[i] = 0; }

  i = 0;

  while (str[i] != '\0')
    {
      find = str[i] - 'a';

      if (alphabet[find] != 1)
	{
	  alphabet[find] = 1;
	  temp[j++] = str[i];
	}
      i++;
    }

  temp2 = (char *) malloc (sizeof (char) * j + 1);

  for (i = 0; i < j; i++)
    {
      temp2[i] = temp[i];
    }

  temp2[j] = '\0';

  return temp2;
}

int **
reinsercao_ordenada (int **originalpopulacao, int popSize, int **newpopulacao,
		     int newpopSize, int* optimal)
{
  int valor_maior = -1;
  int maior = 0;
  int pais0filhos1 = 0;
  int x, y, i;
  int **temp;

  temp = geraPop (10, popSize);

  for (i = 0; i < popSize; i++) 
    {
      for (x = 0; x < popSize; ++x) 
	{
	  if (originalpopulacao[x][10] > valor_maior) 
	    {
	      maior = x;
	      pais0filhos1 = 0;
	      valor_maior = originalpopulacao[maior][10];
	    }
	}

      for (y = 0; y < newpopSize; ++y) 
	{
	  if (newpopulacao[y][10] > valor_maior)
	    {
	      maior = y;
	      pais0filhos1 = 1;
	      valor_maior = newpopulacao[y][10];
	    }
	}

      if (pais0filhos1) 
	{
	  copy (temp[i], newpopulacao[maior], 12);
	  newpopulacao[maior][10] = -1;
	  valor_maior = -1;
	  if (temp[i][10] == notaMaxima) 
	    {
	      *optimal = i;
	    }
	  
	}

      else 
	{
	  copy (temp[i], originalpopulacao[maior], 12);
	  originalpopulacao[maior][10] = -1;
	  valor_maior = -1;
	  if (temp[i][10] == notaMaxima) 
	    {
	      *optimal = i;
	    }
	}
      /* printf("\nSEL2 i\n"); */
      /* noreplin(temp[i] , 10, i); */
    }

  for (i = 0; i < popSize; ++i) 
    {
      free (originalpopulacao[i]);
      //free (newpopulacao[i]);
    }

  free (originalpopulacao);
  //free (newpopulacao);

  return temp;
}
