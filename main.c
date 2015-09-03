#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int size_of_string(char* string);
int parameters_validation(int argc, char* argv[]);
char* compact(char* str);
char* individual_key_vector(char *key1, char *key2, char *key3);

int main(int argc, char* argv[])
{
	if(!parameters_validation(argc, argv))
		return 0;

	printf("Key without repetition: %s", individual_key_vector(argv[1], argv[2], argv[3]));

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
	char* values;

	size1 = size_of_string(key1);
	size2 = size_of_string(key2);
	size3 = size_of_string(key3);

	if(!size1 || !size2 || !size3)
		return NULL;

	values = (char *)malloc(sizeof(char) * (size1 + size2 + size3));

	strcpy(values, key1);
	strcat(values, key2);
	strcat(values, key3);
	
	return compact(values);
}

char* compact(char* str)
{
    int len = strlen(str);
    int a[26], i, j;
    char temp[len];

    for(i = 0, j = 0; i < len; i++)
    {
        int t = (int)str[i] - 65;

         if(a[t]!=1)
         {
         	temp[j] = str[i];
         	j++;
         }
            
        a[t] = 1;
    }

    return temp;
}