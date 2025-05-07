#include "spmv.h"

void sort(SpM* input_spm){

	uint32_t dim = input_spm->dim;
	size_t min_index;

	uint32_t cur_row;
	uint32_t cur_col;

	uint32_t tmp_col;
	uint32_t tmp_row;
	double tmp_val;

	for(size_t elem = 0; elem < dim; elem++){

		min_index = elem;
		cur_row = input_spm->row[elem];
		cur_col = input_spm->col[elem];

		for(size_t other = elem; other < dim; other++){
				uint8_t condition = (cur_row > input_spm->row[other]) ||
												(cur_row == input_spm->row[other] && 
												 cur_col > input_spm->col[other]);

			if(condition){
				cur_row = input_spm->row[other]; 
				cur_col = input_spm->col[other]; 
				min_index = other;
			}
		}

		tmp_row = input_spm->row[elem];
		tmp_col = input_spm->col[elem];
		tmp_val = input_spm->value[elem];
		
		input_spm->row[elem] = input_spm->row[min_index];
		input_spm->col[elem] = input_spm->col[min_index];
		input_spm->value[elem] = input_spm->value[min_index];

		input_spm->row[min_index] = tmp_row;
		input_spm->col[min_index] = tmp_col;
		input_spm->value[min_index] = tmp_val;
	}
}

void get_csr_repr(SpM* input_spm){

	sort(input_spm);
	uint32_t dim_tmp = input_spm->tot_rows+1;
	uint32_t dim = input_spm->dim;

	uint32_t tmp[dim_tmp];
	tmp[0] = 0;
	
	for(size_t elem = 0; elem < dim; elem++){
		for(size_t row = input_spm->row[elem]; row < input_spm->row[elem+1]; row++){ 
			tmp[row] = elem+1;	
		}
	}

	tmp[dim_tmp-1] = dim;
	memcpy(input_spm->row,tmp,dim_tmp*sizeof(uint32_t));
}


double* csr_mult(SpM input_spm,double* input_vec){
	uint32_t tot_rows = input_spm.tot_rows;
	double* res = (double*)malloc(tot_rows*sizeof(double));

	for(size_t row = 0; row < tot_rows; row++){
		for(size_t elem = input_spm.row[row]; elem < input_spm.row[row+1]; elem++){
			res[row] += input_spm.value[elem]*input_vec[input_spm.col[elem]];
		}
	}

	return res;
}
