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

  double* input_vec = gen_random_vec(input_spvm.tot_cols);
	get_csr_repr(&input_spvm);	
	double* res2 = csr_mult(input_spvm,input_vec); 

	free(res2);
	free(input_vec);
	return 0;
}
