#ifndef __UTILS_H__
#define __UTILS_H__

#include "spvm.h"

float* gen_random_vec(uint32_t dim);

char parse_header(SpVM* out_spvm,FILE* mtx_file);
void skip_line(FILE* mtx_file);
float* parse_line(FILE* mtx_file,char read_ch);
SpVM import_spvm(char* mtx_name);

#endif
