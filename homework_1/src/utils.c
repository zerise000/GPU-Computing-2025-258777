#include "utils.h"

float* gen_random_vec(uint32_t dim){
  float* vec = (float*)malloc(dim*sizeof(float));

  for(size_t i = 0; i < dim; i++){
    vec[i] = (float)rand()*(2000/(float)RAND_MAX);

    if(vec[i] - (float)RAND_MAX < 10e-3)
      vec[i] /= 2.0;
  }
  
  return vec;
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
