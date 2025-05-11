#ifndef __SPMV_H__
#define __SPMV_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define PRINT(msg) fprintf(stdout,msg);
#define MAX_ELEM 100000 
#define MIN(n1,n2) n1 > n2 ? n2 : n1;

typedef struct{
	uint32_t row[MAX_ELEM];
	uint32_t col[MAX_ELEM];
	double value[MAX_ELEM];

	uint32_t dim;
	uint32_t tot_rows;
	uint32_t tot_cols;
}SpM;


void merge(SpM* input_spm,size_t start,size_t mid,size_t end);
void sort(SpM* input_spm);
void get_csr_repr(SpM* input_spm);
double* csr_mult(SpM input_spm,double* input_vec);

#endif
