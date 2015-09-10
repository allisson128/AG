#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INITIAL_POPULATION 50

int size_of_string(char* string);
int parameters_validation(int argc, char* argv[]);
char* compact(char* str);
char* individual_key_vector(char *key1, char *key2, char *key3);

typedef struct individual {
	char* values;
	int * key;
	int total_sum;
}Individual;

Individual population[INITIAL_POPULATION];
char* key;

int main(int argc, char* argv[])
{
	int i = 0;

	if(!parameters_validation(argc, argv))
		return 0;

	key = individual_key_vector(argv[1], argv[2], argv[3]);

	printf("%s", key);

	printf("Criando uma chave para: ");
	for(i = 0; key[i] != '\0'; i++)
	{
		printf("%c", key[i]);
	}
	printf("\n");

	return 1;
}

// Retorna o tamanho de uma Strig
int size_of_string(char* string)
{
	int size = 0;

	// Enquanto não for final
	while(string[size] != '\0')
		size++;

	return size;
}

// Valida os parametros de entrada da chamada do exe
int parameters_validation(int argc, char* argv[])
{
	char *first, *second, *third;
	int size1, size2, size3;

	printf("Processing...\n");

	if(argc < 4)
	{
		printf("ERROR: Nao foi possivel formar a chave, menos de 3 argumentos\n");

		return 0;
	}

	first = argv[1];
	size1 = size_of_string(first);

	second = argv[2];
	size2 = size_of_string(second);
	
	third = argv[3];
	size3 = size_of_string(third);

	if(size3 < size1 || size3 < size2)
	{
		printf("ERROR: Parametros invalidos, soma impossivel de ser efetuada\n");

		return 0;
	}

	return 1;
}

// Faz um vetor de intercecao entre as tres palavras
// Send More Money
// SENDMOY
char* individual_key_vector(char *key1, char *key2, char *key3)
{
	int size1, size2, size3;
	char* temp;

	size1 = size_of_string(key1);
	size2 = size_of_string(key2);
	size3 = size_of_string(key3);

	if(!size1 || !size2 || !size3)
		return NULL;

	temp = (char *)malloc(sizeof(char) * (size1 + size2 + size3));

	strcpy(temp, key1);
	strcat(temp, key2);
	strcat(temp, key3);
	
	return compact(temp);
}

char* compact(char* str)
{
	int len = strlen(str);
	int alphabet[26];
	int find, i=0, j=0;
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