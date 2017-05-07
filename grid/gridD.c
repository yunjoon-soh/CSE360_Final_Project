#include "gridD.h"

/*Grid Donor*/
int main(int argc, char** argv){
	char* end_ptr; // used for strtol
	int opt_i=0, opt_k=0, opt_m=0, opt_c=0, opt_d = 0, opt, val=-1;
	long base = 10, val_m=-1, val_c=-1; // base for input number for m or c

	while( (opt = getopt(argc, argv, "ikm:c:d")) != -1 ) {
		switch(opt){
		case 'i':
			opt_i=1;
			break;
		case 'k':
			opt_k=1; break;
		case 'm':
			// convert next option argument to long
			val = Strtol(optarg, &end_ptr, base);

			val_m = val;
			opt_m = 1;
			break;
		case 'c':
			// convert next option argument to long
			val = strtol(optarg, &end_ptr, base);
			if(*end_ptr != '\0'){
				perror("stdtol");
				exit(EXIT_FAILURE);
			}

			val_c = val;
			opt_c = 1;
			break;
		case 'd':
			opt_d++;
			break;
		default:
			fprintf(stderr, "%s\n", USAGE);
			exit(EXIT_FAILURE);
		}
	}

	if(opt_d >= 1)
		fprintf(stderr, "Option i: %d, Option k: %d, Option m: %d %ld, Option c: %d %ld, Option d: %d\n", opt_i, opt_k, opt_m, val_m, opt_c, val_c, opt_d);
			
	fprintf(stdout, "Starting Grid Donor... Version: %s (Build: %d)\n", SERVER_VERSION, BUILDS);
	
	exit(EXIT_SUCCESS);	
}

