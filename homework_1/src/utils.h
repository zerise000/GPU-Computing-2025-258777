#ifndef __UTILS_H__
#define __UTILS_H__

#include "spvm.h"

float* gen_random_vec(uint32_t dim);
float* coo_mult(SpVM input_spvm,float* input_vec);
#endif
