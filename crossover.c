#include <stdio.h>

void pmx(int* parent1, int* parent2, int* son1, int* son2, int vector_size, int p1, int p2);

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
