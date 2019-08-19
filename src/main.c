#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "proto.h"
#include "fpp.h"

/**
 * @file
 * @brief      FPP client
 * Contain     handler "client" of FPP program
 */


static const char *ErrorMessages[] = {
	"Error code: 1. Not enough parameters.",     // 0
	"Error code: 1. Incorrect parameters.",      // 1
	"Error code: 2. RSA key is too short",       // 2
	"Error code: 3. Invalid RSA key",            // 3
	"Error code: 4. Input file is damaged",      // 4
	"Error code: 5. Not enought free memory",    // 5
	"Error code: 6. No such input file",         // 6
	"Error code: 7. Output file already exist"}; // 7

/**
 * @brief      Entry point of program
 *
 * @param[in]  argv[1]      Mode
 * @param[in]  argv[2]      Input file
 * @param[in]  argv[3]      Output file
 * @param[in]  argv[4]      RSA key
 * 
 */

int main (int argc, char *argv[]) {
	int err_check = 0;

	if (argc > 1) {
		if (strcmp(argv[1], "-v") == 0 && argc == 2) {
			fprintf(stdout, "FPP client, v.%s\n", VERSION_OF_BUILD);
		}
		
		else if (strcmp(argv[1], "-e") == 0) {
			if (argc != 5) {
				fprintf(stderr, "%s\n", ErrorMessages[0]);
				exit(1);
			}

			err_check = fpp_enc(argv[2], argv[3], argv[4]);


			switch (err_check){
				case ERR_RSA_KEY_SIZE:
					fprintf(stderr, "\n%s\n", ErrorMessages[2]);
					exit(2);
				case ERR_RSA_KEY_FAILED:
					fprintf(stderr,"\n%s\n", ErrorMessages[3]);
					exit(3);
				case ERR_RSA_KEY_BROKEN:
					fprintf(stderr, "\n%s\n", ErrorMessages[4]);
					exit(4);
				case ERROR_MEMORY:
					fprintf(stderr, "\n%s\n", ErrorMessages[5]);
					exit(5);
				case ERROR_FILE:
					fprintf(stderr, "\n%s\n", ErrorMessages[6]);
					exit(6);
				case ERROR_FILE_EXIST:
					fprintf(stderr, "\n%s\n", ErrorMessages[7]);
					exit(7);
			}
		}

		else if (strcmp(argv[1], "-d") == 0) {
			if (argc != 5) {
				fprintf(stderr, "%s\n", ErrorMessages[0]);
				exit(1);
			}

			err_check = fpp_dec(argv[2], argv[3], argv[4]);

			switch (err_check){

				case ERR_RSA_KEY_SIZE:
					fprintf(stderr, "\n%s\n", ErrorMessages[2]);
					exit(2);
				case ERR_RSA_KEY_FAILED:
					fprintf(stderr, "\n%s\n", ErrorMessages[3]);
					exit(3);
				case ERR_RSA_KEY_BROKEN:
					fprintf(stderr, "\n%s\n", ErrorMessages[4]);
					exit(4);
				case ERROR_MEMORY:
					fprintf(stderr, "\n%s\n", ErrorMessages[5]);
					exit(5);
				case ERROR_FILE:
					fprintf(stderr, "\n%s\n", ErrorMessages[6]);
					exit(6);
			}
		}

		else {
			fprintf(stderr, "%s\n", ErrorMessages[1]);
			exit(1);
		}
	}

	else {
		fprintf(stderr, "%s\n", ErrorMessages[0]);
		exit(1);
	}

	return 0;
}
