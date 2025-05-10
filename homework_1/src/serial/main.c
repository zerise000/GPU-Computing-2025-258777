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

	struct timeval start={0,0};
	struct timeval end={0,0};

	double total_time = 1.0;

	char* mtx_name = argv[1];

	for(int run = -3; run < NR_RUNS; run++){ 

		if(run < 0){
			//import sparse matrix and calculate elapsed time
			gettimeofday(&start,(struct timezone*)0);

			SpM input_spm = import_spm(mtx_name);

			double* input_vec = gen_random_vec(input_spm.tot_cols);
			get_csr_repr(&input_spm);	
			double* res2 = csr_mult(input_spm,input_vec); 
			
			gettimeofday(&end,(struct timezone*)0);

			// update cumulated variables
			total_time *= TIME_INTERVAL(start,end)

			free(res2);
			free(input_vec);
		}
	}

	double total_mean = pow(total_time,1.0/NR_RUNS);

	//calculate geometric mean and display time results
	fprintf(stdout,"total elapsed time: %f\n",total_mean);

	return 0;
}
