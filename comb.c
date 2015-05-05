#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/*argv[0] is the Circuit Description File
  argv[1] is the Input Values File
  */
	struct entry_t{
		char name;
		int value;
	};
  	typedef struct entry_t* entry;

  	char *buffer;
	//That's space for all the variables.
	entry inputs[52];
	entry gatein[52];
	entry gateout[52];
	entry outputs[26];
	//inputs size
	int cursize;
	//Space for our multiplexer inputs.
	int *multiplex; 
	//Number of inputs, number of outputs.
	int inno;
	int outno;
	int mux[64];
	unsigned int i;
	char in;
	char out;
	int numin;
	int numout;
	/*It's definitely possible to go from binary to grey sequencing to decimal.*/
int binary_to_gs_to_dec(entry *gatesin, int numin){
	int inpu = 0;
	for(i = 0; i < numin; i++){
		inpu = inpu << 1;
		inpu += gatesin[i]->value;
	}
	for(i = 0; i < (numin << 1); i++){
		if(inpu == (i^(i>>1))){//Check against the grey sequence. 
			return i;
		}
	}
	return 0;
}

entry find(entry array[], char target, int saiz){
	int i;
	for(i = 0; i < saiz; i++){
		if(target == (array[i])->name){
			return array[i];
		}
	}
	return NULL;
}
/*Reads variables and sets up values in temporary, or output vars. Use to perform gate operation in main.*/
void read(FILE* cdf){
	int a;
	int b;
	a = 0;
	b = 0;
	while(a < numin){
		if (fscanf(cdf, "%c", &in) != 1){
			perror("Could not read input");
			exit(1);
		}
		if((gatein[a] = find(inputs, in, inno)) == NULL){
			perror("Could not find input");
			exit(1);
		}
		a++;
	}
	while(b < numout){
		if(fscanf(cdf, "%c", &out) != 1){
			perror("Could not read output");
			exit(1);
		}
		if((gateout[b] = find(inputs, out, outno)) == NULL){
			if((gateout[b] = find(inputs, out, outno)) == NULL){
				inputs[cursize] -> name = out;
				gateout[b] = inputs[cursize];
				cursize++;
			}
		}
		b++;
	}
	
}
int main(int argc, char* argv[]){
	if(argc != 3){
		printf("Incorrect number of arguments.");
	}
	FILE *cdf;
	FILE *ivf;
	cursize = 0;
	buffer = malloc(sizeof(char)*12);
	
	if((cdf = fopen(argv[1], "r")) == NULL){
		perror("Could not open Circuit Description File.");
		exit(1);
	}
	else if((ivf = fopen(argv[2], "r")) == NULL){
		perror("Could not open Input Values File.");
		exit(1);
	}
	
	fscanf(cdf,"%s",&buffer);
	if(strcmp(buffer, "INPUTVAR") == 0){
		if (fscanf(cdf, "%d", &inno) != 1){
			perror("Could not read inno");
			exit(1);
		}
		for(i = 0; i < inno; i++){
			if(fscanf(cdf, "%c", &(inputs[i]->name)) != 1){
				perror("Could not read all input variables.");
				exit(1);
			}
		}
		cursize = inno;
		/*Finished setting up all initial input variables.
		Temporary ones will be set up as we go.*/
	}
	else{
		perror("Could not grab input vars. ")
		exit(1);
	}
	fscanf(cdf,"%s",&buffer);
	if(strcmp(buffer, "OUTPUTVAR") == 0){
		if (fscanf(cdf, "%d", &outno) != 1){
			perror("Could not read outno");
			exit(1);
		}
		for(i = 0; i < outno; i++){
			if(fscanf(cdf, "%c", &(outputs[i] ->name)) != 1){
				perror("Could not read all output variables.");
				exit(1);
			}
		}
		/*Finished setting up all output variables.*/
	}
	else{
		perror("Could not grab output vars. ")
		exit(1);
	}
	
	/*Begin loading values.*/
	while(fscanf(ivf, "%d", &(inputs[0] ->value)) != EOF){
		for(i = 1; i < inno; i++){
			if(fscanf(ivf, "%d", &(inputs[i] -> value)) != 1){
				perror("Couldn't grab inputs values.");
				exit(1);
			}
		}
	/*Execute circuit.*/
		while(fscanf(cdf, "%s", &buffer)!= EOF){
			if(strcmp(buffer, "NOT") == 0){
				numin = 1;
				numout = 1;
				read(cdf);
				(gateout[0]->value) = !(gatein[0]->value);
			}
			else if(strcmp(buffer, "AND") == 0){
				numin = 2;
				numout = 1;
				read(cdf);
				(gateout[0]->value) = (gatein[0]->value) && (gatein[1]->value);
			}
			else if(strcmp(buffer, "OR") == 0){
				numin = 2;
				numout = 1;
				read(cdf);
				(gateout[0]->value) = (gatein[0]->value) || (gatein[1]->value);
			}
			else if(strcmp(buffer, "DECODER") == 0){
				if (fscanf(cdf, "%d", &numin) != 1){
					perror("Could not read numin");
					exit(1);
				}
				numout = numin << 1 ;
				read(cdf);
				for(i = 0; i < numout; i++){
					gateout[i]->value = 0;
				}
				gateout[binary_to_gs_to_dec(gatein, numin)]->value = 1;
				
			}
			else if(strcmp(buffer, "MULTIPLEXER") == 0){
				if (fscanf(cdf, "%d", &numin) != 1){
					perror("Could not read numin");
					exit(1);
				}
				if(numin > 64){
					perror("Buffer overflow protection. We can only take 64 inputs in the multiplexer.");
					exit(1);
				}
				for(i = 0; i < numin; i++){
					if(fscanf(cdf, "%d", &mux[i])!= 1){
						perror("Trouble reading mux inputs.");
						exit(1);
					}
				}
				numin = numin >> 1;
				numout = 1;
				read(cdf);
				gateout[0]->value = mux[(binary_to_gs_to_dec(gatein, numin))];
			}
		}
	}
	return 0;
}

