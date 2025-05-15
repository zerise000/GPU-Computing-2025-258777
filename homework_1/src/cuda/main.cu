#include "utils.h"
#include <time.h>
#include <sys/time.h>
#include <math.h>

#define NR_RUNS 5

void alloc_dev_buffs(SpM input_spm,uint32_t** kernel_rows,uint32_t** kernel_cols,double** kernel_values){
	cudaMalloc((void**)kernel_rows,(input_spm.tot_rows+1)*sizeof(uint32_t));
	cudaMalloc((void**)kernel_cols,input_spm.dim*sizeof(uint32_t));
	cudaMalloc((void**)kernel_values,input_spm.dim*sizeof(double));

	cudaMemcpy(*kernel_rows,input_spm.row,(input_spm.tot_rows+1)*sizeof(uint32_t),cudaMemcpyHostToDevice);
	cudaMemcpy(*kernel_cols,input_spm.col,input_spm.dim*sizeof(uint32_t),cudaMemcpyHostToDevice);
	cudaMemcpy(*kernel_values,input_spm.value,input_spm.dim*sizeof(double),cudaMemcpyHostToDevice);
}

int main(int argc,char** argv){
  srand(time(NULL));
  
	if(argc < 2){
		fprintf(stderr,"ERROR: .mtx input file not provided");
		return 1;
	}

	struct timeval start={0,0};
	struct timeval end={0,0};

	double total_time = 1.0;
	double host_time= 1.0;
	double device_time = 1.0;

	float kernel_time;
	char* mtx_name = argv[1];

	uint16_t nr_blocks = 512;
	uint16_t block_dim;

	uint32_t* kernel_rows;
	uint32_t* kernel_cols;
	double* kernel_values;

	cudaEvent_t kernel_start;
	cudaEvent_t kernel_end;
	kernel_time = 0;

	cudaEvent_t r_start;
	cudaEvent_t r_stop;
	float r_time = 0;

	for(int run = -NR_RUNS; run < NR_RUNS; run++){ 

		//import sparse matrix and calculate elapsed time
		gettimeofday(&start,(struct timezone*)0);

		SpM input_spm = import_spm(mtx_name);
		get_csr_repr(&input_spm);	
		double seed = (double)rand()*((double)RAND_MAX/2.0);

		gettimeofday(&end,(struct timezone*)0);

		block_dim = input_spm.tot_rows / nr_blocks;

		double* input_vec;
		cudaMallocManaged(&input_vec,input_spm.tot_cols*sizeof(double));

		cudaEventCreate(&r_start);
		cudaEventCreate(&r_stop);
		cudaEventRecord(r_start);

		gen_random_vec<<<nr_blocks,block_dim>>>(input_vec,seed,input_spm.tot_cols);

		cudaEventRecord(r_stop);
		cudaEventSynchronize(r_stop);
		cudaEventElapsedTime(&r_time,r_start,r_stop);
		r_time *= 1e3;
		
		//since structs with dynamic allocated arrays are not allowed
		//it is necessary to copy the arrays of the sparse matrix
		alloc_dev_buffs(input_spm,&kernel_rows,&kernel_cols,&kernel_values);	

		cudaEventCreate(&kernel_start);
		cudaEventCreate(&kernel_end);

		double* res;
		cudaMallocManaged(&res,input_spm.tot_rows*sizeof(double));


		cudaEventRecord(kernel_start);
		csr_mult<<<nr_blocks,block_dim>>>(input_spm.tot_rows,kernel_rows,kernel_cols,kernel_values,input_vec,res); 
		cudaEventRecord(kernel_end);
		cudaEventSynchronize(kernel_end);

		cudaEventElapsedTime(&kernel_time,kernel_start,kernel_end); 
		kernel_time *= 1e3;

		// update cumulated variables
		if(run >= 0){
			total_time *= r_time+kernel_time+TIME_INTERVAL(start,end)
			host_time *= TIME_INTERVAL(start,end)
			device_time *= (kernel_time+r_time);
		}

		cudaEventDestroy(kernel_start);
		cudaEventDestroy(kernel_end);

		cudaFree(kernel_rows);
		cudaFree(kernel_cols);
		cudaFree(kernel_values);
		cudaFree(res);
		free_spm(&input_spm);
		cudaFree(input_vec);
	}

	//calculate geometric mean and display time results
	double total_mean = pow(total_time,1.0/NR_RUNS);
	double host_mean = pow(host_time,1.0/NR_RUNS);
	double device_mean = pow(device_time,1.0/NR_RUNS);

	fprintf(stdout,"%s, host time: %f, device time: %f, total time: %f\n",mtx_name,host_mean,device_mean,total_mean);

	return 0;
}
