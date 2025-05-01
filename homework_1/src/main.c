#include "utils.h"
#include <time.h>

int main(int argc,char** argv){
  srand(time(NULL));
  
	if(argc < 2){
		fprintf(stderr,"ERROR: .mtx input file not provided");
		return 1;
	}

	char* mtx_name = argv[1];
	SpM input_spm = import_spm(mtx_name);

  double* input_vec = gen_random_vec(input_spm.tot_cols);
	get_csr_repr(&input_spm);	
	double* res2 = csr_mult(input_spm,input_vec); 

	for(size_t i=0; i<input_spm.tot_rows; i++)
		fprintf(stdout,"%f\n",res2[i]);

	free(res2);
	free(input_vec);
	return 0;
}
