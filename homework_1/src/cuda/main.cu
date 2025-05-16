#include "utils.h"
#include <time.h>
#include <sys/time.h>
#include <math.h>

#define NR_RUNS 5

float dev_getInput_vec(float blocks_ratio,uint32_t dim_vec,double* input_vec,double seed){
	float elapsed_time = 0;
	cudaEvent_t start;
	cudaEvent_t stop;

	cudaEventCreate(&start);
	cudaEventCreate(&stop);
	cudaEventRecord(start);
	uint32_t nr_blocks = dim_vec*blocks_ratio;
	uint32_t block_dim = dim_vec/nr_blocks;

	gen_random_vec<<<nr_blocks,block_dim>>>(input_vec,seed,dim_vec);

	cudaEventRecord(stop);
	cudaEventSynchronize(stop);
	cudaEventElapsedTime(&elapsed_time,start,stop);

	cudaEventDestroy(start);
	cudaEventDestroy(stop);

	return elapsed_time*1e3;
}

float dev_csr_mult(float blocks_ratio, uint32_t nr_rows,uint32_t* kernel_rows,uint32_t* kernel_cols, double* kernel_values, double* input_vec,double* res){

	cudaEvent_t kernel_start;
	cudaEvent_t kernel_end;
	float mult_time = 0;
	uint32_t nr_blocks = nr_rows*blocks_ratio;
	uint32_t block_dim = nr_rows/nr_blocks;

	cudaEventCreate(&kernel_start);
	cudaEventCreate(&kernel_end);

	cudaEventRecord(kernel_start);
	csr_mult<<<nr_blocks,block_dim>>>(nr_rows,kernel_rows,kernel_cols,kernel_values,input_vec,res); 
	cudaEventRecord(kernel_end);
	cudaEventSynchronize(kernel_end);

	cudaEventElapsedTime(&mult_time,kernel_start,kernel_end); 

	cudaEventDestroy(kernel_start);
	cudaEventDestroy(kernel_end);

	return mult_time * 1e3;
}

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
	float blocks_ratio = 0.7;

	char* mtx_name = argv[1];

	uint32_t* kernel_rows;
	uint32_t* kernel_cols;
	double* kernel_values;

	double* input_vec;
	double* res;

	for(int run = -NR_RUNS; run < NR_RUNS; run++){ 

		//host code
		gettimeofday(&start,(struct timezone*)0);

		SpM input_spm = import_spm(mtx_name);
		get_csr_repr(&input_spm);	
		double seed = (double)rand()*((double)RAND_MAX/2.0);

		cudaMallocManaged(&input_vec,input_spm.tot_cols*sizeof(double));
		cudaMallocManaged(&res,input_spm.tot_rows*sizeof(double));
	
		//since struct arrays are stored in host memory,copy to device memory
		alloc_dev_buffs(input_spm,&kernel_rows,&kernel_cols,&kernel_values);	

		gettimeofday(&end,(struct timezone*)0);

		//exec GPU kernels and retrieve time execution
		float tmp_dev_time = dev_getInput_vec(blocks_ratio,input_spm.tot_cols,input_vec,seed);
		tmp_dev_time += dev_csr_mult(blocks_ratio,input_spm.tot_rows,kernel_rows,kernel_cols,kernel_values,input_vec,res);
	

		// update cumulated variables
		if(run >= 0){
			total_time *= tmp_dev_time+TIME_INTERVAL(start,end)
			host_time *= TIME_INTERVAL(start,end)
			device_time *= tmp_dev_time;
		}


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
