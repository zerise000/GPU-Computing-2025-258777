#ifndef __SPVM_H__
#define __SPVM_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define PRINT(msg) fprintf(stdout,msg);
#define CHR_MAX 4096
#define MAX_LINES 10000 
#define MIN(n1,n2) n1 > n2 ? n2 : n1;

typedef struct{
	uint32_t row[MAX_LINES];
	uint32_t col[MAX_LINES];
	double value[MAX_LINES];

	uint32_t dim;
	uint32_t tot_rows;
	uint32_t tot_cols;
}SpVM;

void sort(SpVM* input_spvm);
void get_csr_repr(SpVM* input_spvm);
double* csr_mult(SpVM input_spvm,double* input_vec);

#endif
