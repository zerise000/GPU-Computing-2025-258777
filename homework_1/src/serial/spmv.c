#include "spmv.h"

void merge(SpM* input_spm,size_t start,size_t mid,size_t end){

	uint32_t left_size = mid-start+1;
	uint32_t right_size = end-mid;


	uint32_t row_left[left_size];
	uint32_t col_left[left_size];
	double value_left[left_size];

	uint32_t row_right[right_size];
	uint32_t col_right[right_size];
	double value_right[right_size];


	for(size_t i=0; i<left_size; i++){
		row_left[i] = input_spm->row[start+i];
		col_left[i] = input_spm->col[start+i];
		value_left[i] = input_spm->value[start+i];
	}

	for(size_t i=0; i<right_size; i++){
		row_right[i] = input_spm->row[i+mid+1];
		col_right[i] = input_spm->col[i+mid+1];
		value_right[i] = input_spm->value[i+mid+1];
	}

	uint32_t index_left = 0;
	uint32_t index_right = 0;
	uint32_t pos = start;


	while(index_left < left_size && index_right < right_size){
		uint8_t condition = (row_left[index_left] < row_right[index_right] || 
												(row_left[index_left] == row_right[index_right] && 
												 col_left[index_left] < col_right[index_right]));
		if(condition){

			input_spm->row[pos] = row_left[index_left];
			input_spm->col[pos] = col_left[index_left];
			input_spm->value[pos] = value_left[index_left];

			index_left++;

		}else{

			input_spm->row[pos] = row_right[index_right];
			input_spm->col[pos] = col_right[index_right];
			input_spm->value[pos] = value_right[index_right];

			index_right++;

		}

		pos++;
	}


	while(index_left < left_size){
		input_spm->row[pos] = row_left[index_left];
		input_spm->col[pos] = col_left[index_left];
		input_spm->value[pos] = value_left[index_left];

		index_left++;
		pos++;
	}


	while(index_right < right_size){

		input_spm->row[pos] = row_right[index_right];
		input_spm->col[pos] = col_right[index_right];
		input_spm->value[pos] = value_right[index_right];

		index_right++;
		pos++;
	}
}

void sort(SpM* input_spm){

	uint32_t dim = input_spm->dim;

	for(size_t i=1; i <= dim-1; i *= 2){
		for(size_t j=0; j < dim-1; j += 2*i){

			uint32_t mid = MIN(dim-1,j+i-1);
			uint32_t start = j;
			uint32_t end = MIN(j+2*i-1,dim-1); 

			merge(input_spm,start,mid,end);
		}
	}
}

void free_spm(SpM* input_spm){
	free(input_spm->row);
	free(input_spm->col);
	free(input_spm->value);
}

void get_csr_repr(SpM* input_spm){

	sort(input_spm);

	uint32_t dim_tmp = input_spm->tot_rows+1;
	uint32_t dim = input_spm->dim;

	uint32_t tmp[dim_tmp];
	tmp[0] = 0;
	
	for(size_t elem = 0; elem < dim-1; elem++){
		for(size_t row = input_spm->row[elem]; row <= input_spm->row[elem+1]; row++){ 
			tmp[row] = elem+1;	
		}
	}

	tmp[dim_tmp-2]++; 
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
