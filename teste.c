#include <stdio.h>

int inidividual_value(int* word, int* keys, int* individual, int individual_vector_size, int word_vector_size);
int pow10(int i);

int main() {
    int result;
    int word[4] = {115,101,110,100}; // SEND
    int keys[8] = {115,101,110,100,109,111,121}; // SENDMORY
    int individual[8] = {4,5,2,3,1,6,7,8}; // INDIVIDUAL

    result = inidividual_value(word, keys, individual, 4, 8);

    printf("%d\n", result);
}

int inidividual_value(int* word, int* keys, int* individual, int individual_vector_size, int word_vector_size) {
    int i,j, power=0;
    int key_value;
    int result = 0;

    for(i = (word_vector_size - 1); i >= 0; i--) {

        for(j = 0; j < individual_vector_size; j++) {
            
            if(word[i] == keys[j]) {

                key_value = individual[j];
                result = result + (key_value * pow10(power));

                power++;
            }

        }
    }

    return result;
}

int pow10(int i) {
    if(i == 0)
        return 1;

    return pow10(i-1) * 10;
}