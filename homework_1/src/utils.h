#ifndef __UTILS_H__
#define __UTILS_H__

#include "spvm.h"

double* gen_random_vec(uint32_t dim);

void parse_header(SpVM* out_spvm,FILE* mtx_file);
void skip_line(FILE* mtx_file);
double* parse_line(FILE* mtx_file,char read_ch);
SpVM import_spvm(char* mtx_name);

#endif
