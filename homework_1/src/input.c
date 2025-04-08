#include "input.h"

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

SpVM import_spvm(char* mtx_name){
	FILE* mtx_file =fopen(mtx_name,"r");
	char read_ch;

	SpVM out;

	uint32_t line_count = 0;

	do{
		read_ch = fgetc(mtx_file);
		if(read_ch == '%')
			skip_line(mtx_file);
		else{
			float* tmp_res = parse_line(mtx_file,read_ch);
			if(line_count < MAX_LINES){
				out.row[line_count] = (uint32_t)tmp_res[0];
				out.col[line_count] = (uint32_t)tmp_res[1];

				out.value[line_count] = tmp_res[2];

				line_count++;
			}

			free(tmp_res);
		}
	}while(read_ch != EOF);

	out.dim = line_count;
	fclose(mtx_file);
	return out;
}
