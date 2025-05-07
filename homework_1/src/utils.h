#ifndef __UTILS_H__
#define __UTILS_H__

#define CHR_MAX 4096
#define TIME_INTERVAL(start,end) ((end.tv_sec-start.tv_sec)*1e6+(end.tv_usec-start.tv_usec));

#include "spmv.h"

double* gen_random_vec(uint32_t dim);

void parse_header(SpM* out_spm,FILE* mtx_file);
void skip_line(FILE* mtx_file);
double* parse_line(FILE* mtx_file,char read_ch);
SpM import_spm(char* mtx_name);

#endif
