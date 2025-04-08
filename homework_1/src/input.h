#ifndef __INPUT_H__
#define __INPUT_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define PRINT(msg) fprintf(stdout,msg);
#define CHR_MAX 4096
#define MAX_LINES 10000 

typedef struct{
	uint32_t row[MAX_LINES];
	uint32_t col[MAX_LINES];
	float value[MAX_LINES];
	uint32_t dim;
}SpVM;

void skip_line(FILE* mtx_file);
float* parse_line(FILE* mtx_file,char read_ch);
SpVM import_spvm(char* mtx_name);


#endif
