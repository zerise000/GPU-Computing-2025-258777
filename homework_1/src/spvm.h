#ifndef __SPVM_H__
#define __SPVM_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define PRINT(msg) fprintf(stdout,msg);
#define CHR_MAX 4096
#define MAX_LINES 10000 

typedef struct{
	uint32_t row[MAX_LINES];
	uint32_t col[MAX_LINES];
	float value[MAX_LINES];

	uint32_t dim;
	uint32_t tot_rows;
	uint32_t tot_cols;
}SpVM;

float retrieve_spvm_cell(SpVM input_spvm, size_t sel_row,size_t sel_col);
float* coo_mult(SpVM input_spvm,float* input_vec);

#endif
