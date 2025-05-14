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

	for(int run = -NR_RUNS; run < NR_RUNS; run++){ 

		SpM input_spm = import_spm(mtx_name);

		//start chronometer
		gettimeofday(&start,(struct timezone*)0);

		double* input_vec = gen_random_vec(input_spm.tot_cols);
		get_csr_repr(&input_spm);	
		double* res2 = csr_mult(input_spm,input_vec); 
		
		//stop chronometer
		gettimeofday(&end,(struct timezone*)0);

		// update cumulated variables
		if(run >= 0)
			total_time *= TIME_INTERVAL(start,end)

		free(res2);
		free(input_vec);

		free_spm(&input_spm);
	}

	double total_mean = pow(total_time,1.0/NR_RUNS);

	//calculate geometric mean and display time results
	fprintf(stdout,"%s, total elapsed time: %f\n",mtx_name,total_mean);

	return 0;
}
