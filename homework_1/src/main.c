#include "utils.h"
#include <time.h>
#include <sys/time.h>

int main(int argc,char** argv){
  srand(time(NULL));
  
	if(argc < 2){
		fprintf(stderr,"ERROR: .mtx input file not provided");
		return 1;
	}

	struct timeval start={0,0};
	struct timeval end={0,0};

	double total_time = 0.0;
	double import_time = 0.0;
	double csr_mult_time = 0.0;

//import sparse matrix and calculate elapsed time
	gettimeofday(&start,(struct timezone*)0);

	char* mtx_name = argv[1];
	SpM input_spm = import_spm(mtx_name);

	gettimeofday(&end,(struct timezone*)0);
	import_time = TIME_INTERVAL(start,end) 

//compute csr multiplication and get elapsed time
	gettimeofday(&start,(struct timezone*)0);

  double* input_vec = gen_random_vec(input_spm.tot_cols);
	get_csr_repr(&input_spm);	
	double* res2 = csr_mult(input_spm,input_vec); 
	
	gettimeofday(&end,(struct timezone*)0);
	csr_mult_time = TIME_INTERVAL(start,end) 


	//sum times togheter, print obtained informations
	total_time = csr_mult_time+import_time;
	double percentage_import = (import_time/total_time)*100;
	double percentage_csr_mult = (csr_mult_time/total_time)*100;

	fprintf(stdout,"total time: %f\n",total_time);
	fprintf(stdout,"import time (percentage): %f%\n",percentage_import);
	fprintf(stdout,"csr multiplication time (percentage): %f%\n",percentage_csr_mult);

	free(res2);
	free(input_vec);

	return 0;
}
