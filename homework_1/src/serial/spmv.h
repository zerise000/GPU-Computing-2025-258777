#ifndef __SPMV_H__
#define __SPMV_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define PRINT(msg) fprintf(stdout,msg);
#define MIN(n1,n2) n1 > n2 ? n2 : n1;

typedef struct{
	uint32_t* row;
	uint32_t* col;
	double* value;

	uint32_t dim;
	uint32_t tot_rows;
	uint32_t tot_cols;
}SpM;

void free_spm(SpM* input_spm);
void merge(SpM* input_spm,size_t start,size_t mid,size_t end);
void sort(SpM* input_spm);
void get_csr_repr(SpM* input_spm);
double* csr_mult(SpM input_spm,double* input_vec);

#endif
