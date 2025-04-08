#include "input.h"


int main(int argc,char** argv){

	if(argc < 2){
		fprintf(stderr,"ERROR: .mtx input file not provided");
		return 1;
	}

	char* mtx_name = argv[1];
	SpVM input_spvm = import_spvm(mtx_name);

	for(size_t i=0; i<input_spvm.dim; i++)
		printf("i: %d, j:%d, value: %f\n",input_spvm.row[i],input_spvm.col[i],input_spvm.value[i]);

	return 0;
}
