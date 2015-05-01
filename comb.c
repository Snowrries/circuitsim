#include <stdio.h>
#include <stdlib.h>
/*argv[0] is the Circuit Description File
  argv[1] is the Input Values File
  */
  
  
int main(int argc, char* argv[]){
	if(argc != 3){
		printf("Incorrect number of arguments.");
	}
	FILE cdf;
	FILE ivf;
	char *buffer;
	char *inputs;
	char *outputs;
	int *invals;
	int *outvals;
	char var;
	int inno;
	int outno;
	int intemp;
	int outtemp;
	int i;
	
	if((cdf = fopen(argv[1], "r")) == NULL){
		perror("Could not open Circuit Description File.");
		exit(1);
	}
	else if((ivf = fopen(argv[2], "r")) == NULL){
		perror("Could not open Input Values File.");
		exit(1);
	}
	
	while(fscanf(cdf,"%s",buffer) == 1 ){
		if(strcmp(buffer, "INPUTVAR") == 0){
			if (fscanf(cdf, "%d", inno) != 1){
				perror("Could not read inno");
				exit(1);
			}
			inputs = malloc(inno * sizeof(char));
			invals = malloc(inno * sizeof(int));
			for(i = 0; i < inno; i++){
				if(fscanf(cdf, "%c", inputs[i]) != 1){
					perror("Could not read all input variables.");
					exit(1);
				}
			}
			/*Finished setting up all inputs variables and malloced space for their values.*/
		}
		
		else if(strcmp(buffer, "OUTPUTVAR") == 0){
			if (fscanf(cdf, "%d", outno) != 1){
				perror("Could not read outno");
				exit(1);
			}
			outputs = malloc(outno * sizeof(char));
			outvals = malloc(outno * sizeof(int));
			for(i = 0; i < outno; i++){
				if(fscanf(cdf, "%c", outputs[i]) != 1){
					perror("Could not read all output variables.");
					exit(1);
				}
			}
			/*Finished setting up all output variables and malloced space for their values.*/
		}
	}
	rewind(cdf);
	/*Begin loading values.*/
	while(fscanf(ivf, "%d", invals[0]) != EOF){
		for(i = 1; i < inno; i++){
			if(fscanf(ivf, "%d", invals[i]) != 1){
				perror("Couldn't grab inputs values.");
				exit(1);
			}
		}
	/*Execute circuit.*/
		while(fscanf(cdf, "%s", buffer)!= EOF){
			if(strcmp(buffer, "NOT") == 0){
				if (fscanf(cdf, "%c", var) != 1){
					perror("Could not read var");
					exit(1);
				}
				else if((intemp = find(inputs, var, inno)) == -1){
					perror("Could not find inputvar");
					exit(1);
				}
				else if((outtemp = find(outputs, var, outno)) == -1){
					perror("Could not find outputvar");
					exit(1);
				}
				outputs[outtemp] = !inputs[intemp];
			}
			else if(strcmp(buffer, "AND") == 0){
				
			}
			else if(strcmp(buffer, "OR") == 0){
				
			}
			else if(strcmp(buffer, "DECODER") == 0){
				
			}
			else if(strcmp(buffer, "MULTIPLEXER") == 0){
				
			}
		}
	}
}

int find(char array[], char target, int saiz){
	int i;
	for(i = 0; i < saiz; i++){
		if(target == array[i]){
			return i;
		}
	}
	return -1;
}
