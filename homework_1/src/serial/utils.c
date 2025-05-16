#include "utils.h"


double* gen_random_vec(uint32_t dim){
  double* vec = (double*)malloc(dim*sizeof(double));

  for(size_t i = 0; i < dim; i++){
    vec[i] = ((double)RAND_MAX/2.0)*(double)rand();
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

	out_spm->row = (uint32_t*)malloc(out_spm->dim*sizeof(uint32_t));
	out_spm->col = (uint32_t*)malloc(out_spm->dim*sizeof(uint32_t));
	out_spm->value = (double*)malloc(out_spm->dim*sizeof(double));

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

		out.row[elem] = (uint32_t)tmp_res[0];
		out.col[elem] = (uint32_t)tmp_res[1];

		out.value[elem] = tmp_res[2];

		free(tmp_res);
	}

	
	fclose(mtx_file);
	return out;
}
