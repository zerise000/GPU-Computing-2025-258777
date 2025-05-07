#include "utils.h"

void warm_up(){

	// define identity matrix
	SpM tmp_spm;
	tmp_spm.dim = (1 << 5);
	tmp_spm.tot_rows = (1 << 5);
	tmp_spm.tot_cols = (1 << 5);

	for(size_t i=0; i<tmp_spm.tot_rows; i++){
		tmp_spm.row[i] = i+1;
		tmp_spm.col[i] = i+1;
		tmp_spm.value[i] = 1;
	}

	
	double* tmp_vec = gen_random_vec(tmp_spm.tot_cols);
	get_csr_repr(&tmp_spm);	
	double* tmp_res = csr_mult(tmp_spm,tmp_vec); 

	free(tmp_vec);
	free(tmp_res);
}


double* gen_random_vec(uint32_t dim){
  double* vec = (double*)malloc(dim*sizeof(double));

  for(size_t i = 0; i < dim; i++){
    vec[i] = 2000*((double)rand()/(double)RAND_MAX);
  }
  
  return vec;
}

void skip_line(FILE* mtx_file){
	char skip_chr;

	do{
		skip_chr = fgetc(mtx_file);
	}while(skip_chr != EOF && skip_chr != '\n');
}

double* parse_line(FILE* mtx_file,char read_ch){

	char line_chr = read_ch;

	uint32_t index = 0;
	uint32_t parse_state = 0;
	char tmp[CHR_MAX];
	double* numbers = (double*)malloc(3*sizeof(double));
	memset(tmp,0,CHR_MAX);

	do{
		if(line_chr == ' '){
			sscanf(tmp,"%lf",&numbers[parse_state]);
			parse_state++;
			index = 0;
			memset(tmp,0,CHR_MAX);
			line_chr = fgetc(mtx_file);
		}

		if(index < CHR_MAX){
			tmp[index] = line_chr;	
			index++;
		}

		line_chr = fgetc(mtx_file);
	}while(line_chr != EOF && line_chr != '\n');

	sscanf(tmp,"%lf",&numbers[parse_state]);
	return numbers; 
}

void parse_header(SpM* out_spm,FILE* mtx_file){
	char iter_ch;

	for(iter_ch = fgetc(mtx_file); iter_ch == '%'; iter_ch = fgetc(mtx_file))
		skip_line(mtx_file);

	double* dim_info = parse_line(mtx_file,iter_ch);

	out_spm->tot_rows = (uint32_t)dim_info[0];
	out_spm->tot_cols = (uint32_t)dim_info[1];
	out_spm->dim = (uint32_t)dim_info[2];

	free(dim_info);

}

SpM import_spm(char* mtx_name){

	FILE* mtx_file =fopen(mtx_name,"r");
	SpM out;

	//parse header, then return next character
	parse_header(&out,mtx_file);

	for(size_t elem=0; elem<out.dim; elem++){
		char read_ch = fgetc(mtx_file);
		double* tmp_res = parse_line(mtx_file,read_ch);

		if(elem < MAX_ELEM){
			out.row[elem] = (uint32_t)tmp_res[0];
			out.col[elem] = (uint32_t)tmp_res[1];

			out.value[elem] = tmp_res[2];
		}

		free(tmp_res);
	}

	
	fclose(mtx_file);
	return out;
}
