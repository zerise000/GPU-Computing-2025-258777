#include "utils.h"
#include <time.h>
#include <sys/time.h>
#include <math.h>

#define NR_RUNS 5

int main(int argc,char** argv){
  srand(time(NULL));
  
	if(argc < 2){
		fprintf(stderr,"ERROR: .mtx input file not provided");
		return 1;
	}

	warm_up();

	struct timeval start={0,0};
	struct timeval end={0,0};

	double total_time = 1.0;
	double import_time = 1.0;
	double csr_mult_time = 1.0;

	for(size_t run = 0; run < NR_RUNS; run++){ 

		//import sparse matrix and calculate elapsed time
		gettimeofday(&start,(struct timezone*)0);

		char* mtx_name = argv[1];
		SpM input_spm = import_spm(mtx_name);

		gettimeofday(&end,(struct timezone*)0);
		double tmp_import_time = TIME_INTERVAL(start,end) 

		//compute csr multiplication and get elapsed time
		gettimeofday(&start,(struct timezone*)0);

		double* input_vec = gen_random_vec(input_spm.tot_cols);
		get_csr_repr(&input_spm);	
		double* res2 = csr_mult(input_spm,input_vec); 
		
		gettimeofday(&end,(struct timezone*)0);
		double tmp_csr_time = TIME_INTERVAL(start,end) 

		// update cumulated variables
		import_time *= tmp_import_time;
		csr_mult_time *= tmp_csr_time;
		total_time *= (tmp_import_time + tmp_csr_time);

		free(res2);
		free(input_vec);
	}

	double csr_mult_mean = pow(csr_mult_time,1.0/NR_RUNS);
	double import_mean = pow(import_time,1.0/NR_RUNS);
	double total_mean = pow(total_time,1.0/NR_RUNS);

	//calculate geometric mean and display time results
	fprintf(stdout,"total elapsed time: %f\n",total_mean);
	fprintf(stdout,"import elapsed time: %f\n",import_mean);
	fprintf(stdout,"multiplication elapsed time: %f\n",csr_mult_mean);

	return 0;
}
