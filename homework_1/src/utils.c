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

void skip_line(FILE* mtx_file){
	char skip_chr;

	do{
		skip_chr = fgetc(mtx_file);
	}while(skip_chr != EOF && skip_chr != '\n');
}

float* parse_line(FILE* mtx_file,char read_ch){

	char line_chr = read_ch;

	uint32_t index = 0;
	uint32_t parse_state = 0;
	char tmp[CHR_MAX];
	float* numbers = (float*)malloc(3*sizeof(float));
	memset(tmp,0,sizeof(tmp));

	do{
		if(line_chr == ' '){
			sscanf(tmp,"%f",&numbers[parse_state]);
			parse_state++;
			index = 0;
			memset(tmp,0,sizeof(tmp));
			line_chr = fgetc(mtx_file);
		}

		if(index < CHR_MAX){
			tmp[index] = line_chr;	
			index++;
		}

		line_chr = fgetc(mtx_file);
	}while(line_chr != EOF && line_chr != '\n');

	sscanf(tmp,"%f",&numbers[parse_state]);
	return numbers; 
}

char parse_header(SpVM* out_spvm,FILE* mtx_file){
	char iter_ch;

	for(iter_ch = fgetc(mtx_file); iter_ch == '%'; iter_ch = fgetc(mtx_file))
		skip_line(mtx_file);

	float* dim_info = parse_line(mtx_file,iter_ch);

	out_spvm->tot_rows = (uint32_t)dim_info[0];
	out_spvm->tot_cols = (uint32_t)dim_info[1];
	out_spvm->dim = (uint32_t)dim_info[2];

	free(dim_info);

	return iter_ch;
}

SpVM import_spvm(char* mtx_name){

	FILE* mtx_file =fopen(mtx_name,"r");
	SpVM out;

	//parse header, then return next character
	char read_ch = parse_header(&out,mtx_file);

	for(size_t line=0; line<out.dim; line++){
		float* tmp_res = parse_line(mtx_file,read_ch);
		if(line < MAX_LINES){
			out.row[line] = (uint32_t)tmp_res[0];
			out.col[line] = (uint32_t)tmp_res[1];

			out.value[line] = tmp_res[2];
		}

		read_ch = fgetc(mtx_file);
		free(tmp_res);
	}

	
	fclose(mtx_file);
	return out;
}
