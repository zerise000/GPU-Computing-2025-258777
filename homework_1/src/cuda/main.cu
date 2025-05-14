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
	float kernel_time;
	char* mtx_name = argv[1];

	cudaEvent_t kernel_start;
	cudaEvent_t kernel_end;

	for(int run = -NR_RUNS; run < NR_RUNS; run++){ 

		//import sparse matrix and calculate elapsed time
		gettimeofday(&start,(struct timezone*)0);

		SpM input_spm = import_spm(mtx_name);
		get_csr_repr(&input_spm);	
		double* input_vec = gen_random_vec(input_spm.tot_cols);

		gettimeofday(&end,(struct timezone*)0);
		//compute csr multiplication and get elapsed time

		cudaEventCreate(&kernel_start);
		cudaEventCreate(&kernel_end);
		double* res = (double*)malloc(input_spm.tot_rows*sizeof(double));

		cudaEventRecord(kernel_start);
		csr_mult<<<2,50>>>(input_spm,input_vec,res); 
		cudaEventRecord(kernel_end);
		cudaEventSynchronize(kernel_end);
		cudaDeviceSynchronize();

		kernel_time = 0;
		cudaEventElapsedTime(&kernel_time,kernel_start,kernel_end); 

		// update cumulated variables
		if(run >= 0)
			total_time *= kernel_time+TIME_INTERVAL(start,end)

		cudaEventDestroy(kernel_start);
		cudaEventDestroy(kernel_end);
		free(res);
		free(input_vec);
	}

	double total_mean = pow(total_time,1.0/NR_RUNS);

	//calculate geometric mean and display time results
	fprintf(stdout,"%s, total elapsed time: %f\n",mtx_name,total_mean);

	return 0;
}
