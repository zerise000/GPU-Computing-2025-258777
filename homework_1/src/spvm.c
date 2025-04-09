#include "spvm.h"



float retrieve_spvm_cell(SpVM input_spvm, size_t sel_row,size_t sel_col){
	float res = 0;
	for(size_t cell = 0; cell < input_spvm.dim; cell++){
		if(input_spvm.row[cell] == sel_row && input_spvm.col[cell] == sel_col){
			res = input_spvm.value[cell];
			break;
		}
	}
	return res;
}

float* coo_mult(SpVM input_spvm,float* input_vec){
	float* res = (float*)malloc(input_spvm.dim*sizeof(float));

	float res_cell_val = 0;
	float spvm_cell_val = 0;
	size_t index_res = 0;

	for(size_t i=1; i<=input_spvm.tot_rows; i++){

		for(size_t j=1; j<=input_spvm.tot_cols; j++){
			spvm_cell_val = retrieve_spvm_cell(input_spvm,i,j);	
			res_cell_val += spvm_cell_val*input_vec[i-1];
		}

		if(index_res < input_spvm.tot_cols){
			res[index_res] = res_cell_val;
			index_res++;
		}

	}

	return res;
}
