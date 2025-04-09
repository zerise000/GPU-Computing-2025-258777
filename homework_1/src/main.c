#include "utils.h"
#include <time.h>

int main(int argc,char** argv){
  srand(time(NULL));
  
	if(argc < 2){
		fprintf(stderr,"ERROR: .mtx input file not provided");
		return 1;
	}

	char* mtx_name = argv[1];
	SpVM input_spvm = import_spvm(mtx_name);
  float* input_vec = gen_random_vec(input_spvm.tot_cols);
  float* res = coo_mult(input_spvm,input_vec);
  
	for(size_t i=0; i<input_spvm.tot_cols; i++)
		printf("i = %ld, value = %f\n",i,res[i]);

	free(input_vec);
	free(res);
	return 0;
}
