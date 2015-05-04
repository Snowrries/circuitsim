#include <stdio.h>
#include <stdlib.h>
/*argv[0] is the Circuit Description File
  argv[1] is the Input Values File
  */
  	char *buffer;
	char *inputs;
	char *outputs;
	int *invals;
	int *outvals;
	int inno;
	int outno;
	int *intemp;
	int intemp_s;
	int *outtemp;
	int outtemp_s;
	int i;
	char *in;
	char *out;
	int decin;
	int muxin;
	int numin;
	int numout;
  
int main(int argc, char* argv[]){
	if(argc != 3){
		printf("Incorrect number of arguments.");
	}
	FILE cdf;
	FILE ivf;
	intemp = malloc(sizeof(int)*2);
	intemp_s = 2;
	outtemp = malloc(sizeof(int)*4);
	outtemp_s = 4;
	in = malloc(sizeof(char)*2);
	out = malloc(sizeof(char)*4);
	if(!(intemp&&outtemp&&in&&out)){
		perror("mallocing error");
		exit(1);
	}
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
				numin = 1;
				numout = 1;
				read(cdf);
				outputs[outtemp[0]] = !inputs[intemp[0]];
			}
			else if(strcmp(buffer, "AND") == 0){
				numin = 2;
				numout = 1;
				read(cdf);
				outputs[outtemp[0]] = (inputs[intemp[0]] && inputs[intemp[1]]);
			}
			else if(strcmp(buffer, "OR") == 0){
				numin = 2;
				numout = 1;
				read(cdf);
				outputs[outtemp[0]] = (inputs[intemp[0]] || inputs[intemp[1]]);
			}
			else if(strcmp(buffer, "DECODER") == 0){
				if (fscanf(cdf, "%d", numin) != 1){
					perror("Could not read numin");
					exit(1);
				}
				numout = numin << 1 ;
				if(numin > intemp_s){
					realloc(intemp, sizeof(int)*numin);
					realloc(in, sizeof(char)*numin);
				}
				if(numout > outtemp_s){
					realloc(outtemp, sizeof(int)*numout);
				}
				
				
				read(cdf, decin, (i << 1));
				
				
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
/*Reads variables and sets up values in intemp and outtemp. Use to perform gate operation in main.*/
int read(FILE cdf, int intemp[], char in[], char *inputs, int outtemp[], char out[], char *outputs, int inno, int outno, int numin, int numout){
	int a;
	int b;
	a = b = 0;
	while(a <= numin){
		if (fscanf(cdf, "%c", in[a]) != 1){
			perror("Could not read input");
			exit(1);
		}
		a++;
	}
	while(b <= numout){
		if(fscanf(cdf, "%c", out[b]) != 1){
			perror("Could not read output");
			exit(1);
		}
		b++;
	}
	a = 0;
	b = 0;
	while(a <= numin){
		if((intemp[a] = find(inputs, in[a], inno)) == -1){
			perror("Could not find input");
			exit(1);
		}
		a++;
	}
	while(b <= numout){
		if((outtemp[b] = find(outputs, out[b], outno)) == -1){
			perror("Could not find output");
			exit(1);
		}
		b++;
	}
	
}






