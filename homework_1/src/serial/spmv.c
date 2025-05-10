#include "spmv.h"

void merge(SpM* input_spm,size_t start,size_t mid,size_t end,size_t dim,uint32_t* tmp_row,uint32_t* tmp_col,double* tmp_value){

		size_t k = start;
		size_t left_pos = start;
		size_t right_pos = mid + 1;

    while (left_pos <= mid && right_pos <= end) {
				uint8_t condition = input_spm->row[left_pos] < input_spm->row[right_pos] || 
														(input_spm->row[left_pos] == input_spm->row[right_pos] && 
														 input_spm->col[left_pos] < input_spm->col[right_pos]);
        if (condition) {
					
					tmp_row[k] = input_spm->row[left_pos];
					tmp_col[k] = input_spm->col[left_pos];
					tmp_value[k] = input_spm->value[left_pos];
				
					left_pos++;
        }
        else {
					
					tmp_row[k] = input_spm->row[right_pos];
					tmp_col[k] = input_spm->col[right_pos];
					tmp_value[k] = input_spm->value[right_pos];

					right_pos++;
        }

				k++;
    }
 
    while (left_pos < dim && left_pos <= mid) {

        tmp_row[k] = input_spm->row[left_pos];
				tmp_col[k] = input_spm->col[left_pos];
				tmp_value[k] = input_spm->value[left_pos];

				k++;
				left_pos++;
    }

    for (size_t i = start; i <= end; i++) {
			input_spm->row[i] = tmp_row[i];
			input_spm->col[i]= tmp_col[i];
			input_spm->value[i] = tmp_value[i];
    }
}

void sort(SpM* input_spm){

	uint32_t dim = input_spm->dim;
	uint32_t tmp_row[dim];
	uint32_t tmp_col[dim];
	double tmp_value[dim];

	memcpy(tmp_row,input_spm->row,dim*sizeof(uint32_t));
	memcpy(tmp_col,input_spm->col,dim*sizeof(uint32_t));
	memcpy(tmp_value,input_spm->value,dim*sizeof(double));

	for(size_t i=1; i <= dim-1; i *= 2){
		for(size_t j=0; j < dim-1; j += 2*i){

			uint32_t mid = j+i-1;
			uint32_t start = j;
			uint32_t end = MIN(j+2*i-1,dim-1); 

			merge(input_spm,start,mid,end,dim,tmp_row,tmp_col,tmp_value);
		}
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
