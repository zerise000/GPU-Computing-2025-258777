#include "spvm.h"

void sort(SpVM* input_spvm){

	uint32_t dim = input_spvm->dim;
	size_t min_index;

	uint32_t cur_row;
	uint32_t cur_col;

	uint32_t tmp_col;
	uint32_t tmp_row;
	double tmp_val;

	for(size_t elem = 0; elem < dim; elem++){

		min_index = elem;
		cur_row = input_spvm->row[elem];
		cur_col = input_spvm->col[elem];

		for(size_t other = elem; other < dim; other++){
				uint8_t condition = (cur_row > input_spvm->row[other]) ||
												(cur_row == input_spvm->row[other] && 
												 cur_col > input_spvm->col[other]);

			if(condition){
				cur_row = input_spvm->row[other]; 
				cur_col = input_spvm->col[other]; 
				min_index = other;
			}
		}

		tmp_row = input_spvm->row[elem];
		tmp_col = input_spvm->col[elem];
		tmp_val = input_spvm->value[elem];
		
		input_spvm->row[elem] = input_spvm->row[min_index];
		input_spvm->col[elem] = input_spvm->col[min_index];
		input_spvm->value[elem] = input_spvm->value[min_index];

		input_spvm->row[min_index] = tmp_row;
		input_spvm->col[min_index] = tmp_col;
		input_spvm->value[min_index] = tmp_val;
	}
}

void get_csr_repr(SpVM* input_spvm){

	sort(input_spvm);
	uint32_t dim_tmp = input_spvm->tot_rows+1;
	uint32_t dim = input_spvm->dim;

	uint32_t tmp[dim_tmp];
	tmp[0] = 0;

	for(size_t elem = 0; elem < dim; elem++){
		for(size_t row = input_spvm->row[elem]; row < input_spvm->row[elem+1]; row++){ 
			tmp[row] = elem+1;	
		}
	}

	tmp[dim_tmp-1] = dim;
	memcpy(input_spvm->row,tmp,dim_tmp*sizeof(uint32_t));
}


double* csr_mult(SpVM input_spvm,double* input_vec){
	uint32_t tot_rows = input_spvm.tot_rows;
	double* res = (double*)malloc(tot_rows*sizeof(double));

	for(size_t row = 0; row < tot_rows; row++){
		for(size_t elem = input_spvm.row[row]; elem < input_spvm.row[row+1]; elem++){
			printf("element (%d,%d) has value %f\n",row,input_spvm.col[elem],input_spvm.value[elem]);
			res[row] += input_spvm.value[elem]*input_vec[input_spvm.col[elem]];
		}
	}


	for(size_t i=0; i<tot_rows; i++)
		printf("%f\n",res[i]);

	return res;
}
