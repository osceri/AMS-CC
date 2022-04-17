/*
 * cell_balancer.c
 *
 *  Created on: Apr 16, 2022
 *      Author: oscar
 */

#include "smile_callbacks.h"
#include "programme_queues.h"
#include "programme_functions.h"
#include "math.h"

double *cell_voltages;

uint8_t sorted_indices[] = {
#define _abcd(a,b,c,d) (7*3*3*a+7*3*b+7*c+d),
#define _abc(a,b,c) _abcd(a,b,c,0)_abcd(a,b,c,1)_abcd(a,b,c,2)_abcd(a,b,c,3)_abcd(a,b,c,4)_abcd(a,b,c,5)_abcd(a,b,c,6)
#define _ab(a,b) _abc(a,b,0)_abc(a,b,1)_abc(a,b,2)
#define _a(a) _ab(a,0)_ab(a,1)_ab(a,2)
		_a(0)_a(1) };

void swap(uint8_t g) {
	uint8_t _I = sorted_indices[g];
	sorted_indices[g] = sorted_indices[g - 1];
	sorted_indices[g - 1] = _I;
}

uint8_t compare(uint8_t g) {
	return cell_voltages[sorted_indices[g]] < cell_voltages[sorted_indices[g - 1]];
}

void gnome_sort() {
	uint8_t gnome = 0;
	while (gnome < 126) {
		if ((gnome == 0) || compare(gnome)) {
			gnome++;
		} else {
			swap(gnome);
			gnome--;
		}
	}
}

uint8_t do_cell_balancing() {
	if (xQueuePeek(cell_voltages_queue, &cell_voltages, 0)) {
		gnome_sort();
		for (int i = 0; i < 126; i++) {
			set_balance_ext(0, i, NULL);
		}
		for (int i = 0; i < 41; i++) {
			set_balance_ext(1, sorted_indices[i], NULL);

		}
	}
	return 1;
}

uint8_t end_cell_balancing() {
	for (int i = 0; i < 126; i++) {
		set_balance_ext(0, i, NULL);
	}
	return 1;
}
