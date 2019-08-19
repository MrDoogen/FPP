#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <unistd.h>

#include "proto.h"
#include "io_text_streams.h"
#include "io_binary_streams.h"
#include "progress_bar.h"
#include "rsa.h"

#define SIZE_STR     450
#define LEN_BYTE     3
#define CODE_SYMBOL  500

/**
 * @brief      Encrypted function
 * @param      input_file   The input file
 * @param      output_file  The output file
 * @param      file_key     The file key
 * 
 * @return     Code error
 */

int fpp_enc (char *input_file, char *output_file, char *file_key) {
	size_t size_input_file = 0;   // Size of input file
	char *all_input_bytes = NULL; // Buffer contain bytes of input file
	char *proc_bytes = NULL;      // Buffer contain part of bytes
	char *enc_proc_bytes = NULL;  // Buffer contain encrypted part of bytes
	char *byte_to_int = NULL;     // Buffer contain bytes as integer numbers
	char enter[] = "\n";          // Symbol of enter
	
	char *rsa_e_numb = NULL;      // Number e from RSA key
	char *rsa_n_numb = NULL;      // Number n from RSA key
	
	int err_check = 0;

	/* Read RSA keys from files */
	err_check = text_read_line(file_key, &rsa_e_numb, 1);
	
	switch (err_check){
		case ERROR_MEMORY:
			return ERROR_MEMORY;
		case ERROR_FILE:
			return ERROR_FILE;
	}

	err_check = text_read_line(file_key, &rsa_n_numb, 2);


	switch (err_check){
		case ERROR_MEMORY:
			return ERROR_MEMORY;
		case ERROR_FILE:
			return ERROR_FILE;
	}

	/* Initializing buffers */
	proc_bytes = (char *) calloc((SIZE_STR * LEN_BYTE + 1), sizeof(char));
	byte_to_int = (char *) malloc((LEN_BYTE + 1) * sizeof(char));

	if (proc_bytes == NULL) {
		return ERROR_MEMORY;
	}

	if (byte_to_int == NULL) {
		return ERROR_MEMORY;
	}

	if (size_of_file(output_file) != ERROR_FILE) {
		return ERROR_FILE_EXIST;
	}

	/* Open input file */
	size_input_file = file_read(input_file, &all_input_bytes);

	for (size_t i = 0, k = 1; i < size_input_file; i++, k++) {
		progress_bar(i, size_input_file);
		sprintf(byte_to_int, "%d", (all_input_bytes[i] + CODE_SYMBOL));
		byte_to_int[LEN_BYTE] = '\0';
		strcat(proc_bytes, byte_to_int);

		if (k == SIZE_STR || (i + 1) == size_input_file) {
			proc_bytes[SIZE_STR * LEN_BYTE] = '\0';
			free(enc_proc_bytes);

			err_check = rsa_enc(&enc_proc_bytes,
								proc_bytes,
								rsa_e_numb,
								rsa_n_numb);
			
			switch (err_check){
				case ERR_RSA_KEY_SIZE:
					return ERR_RSA_KEY_SIZE;
				case ERR_RSA_KEY_FAILED:
					return ERR_RSA_KEY_FAILED;
				case ERR_RSA_KEY_BROKEN:
					return ERR_RSA_KEY_BROKEN;
			}

			text_write(output_file, enc_proc_bytes, 'a');

			if ((i + 1) != size_input_file) {
				text_write(output_file, enter, 'a');
			}
			memset(proc_bytes, '\0', (SIZE_STR * LEN_BYTE + 1) * sizeof(char));
			k = 0;
		}
	}

	progress_bar(1, 1);

	free(all_input_bytes);
	free(proc_bytes);
	free(enc_proc_bytes);
	free(byte_to_int);
	free(rsa_e_numb);
	free(rsa_n_numb);

	return 0;
}


int fpp_dec (char *input_file, char *output_file, char *file_key) {
	size_t size_input_enc_str = 0; // Size of encrypted input strings
	size_t size_input_dec_str = 0; // Size of input strings
	size_t size_input_file = 0;    // Size of input file
	size_t size_output_file = 0;   // Summary size of output file
	int char_to_int = 0;           // Symbol like integer number
	char *enc_proc_bytes = NULL;   // Buffer contain encrypted string of bytes
	char *dec_proc_bytes = NULL;   // Buffer contain string of bytes
	char *all_output_bytes = NULL; // Buffer contain bytes of output file
	char *int_to_byte = NULL;      // Buffer contain bytes as integer numbers

	char *rsa_d_numb = NULL;       // Number d from RSA key
	char *rsa_n_numb = NULL;       // Number n from RSA key

	int err_check  = 0;

	/* Read RSA keys from files */
	err_check = text_read_line(file_key, &rsa_d_numb, 1);

	switch (err_check){
		case ERROR_MEMORY:
			return ERROR_MEMORY;
		case ERROR_FILE:
			return ERROR_FILE;
	}

	err_check = text_read_line(file_key, &rsa_n_numb, 2);

	switch (err_check){
		case ERROR_MEMORY:
			return ERROR_MEMORY;
		case ERROR_FILE:
			return ERROR_FILE;
	}

	/* Determine input file size */
	size_input_file = size_of_file(input_file);

	/* Initializing buffers */
	int_to_byte = (char *) malloc((LEN_BYTE + 1) * sizeof(char));

	if (int_to_byte == NULL) {
		return ERROR_MEMORY;
	}

	/* Read first line in input file */
	size_input_enc_str= text_read_line(input_file, &enc_proc_bytes, 1);

	/* First line must be not empty */
	if (enc_proc_bytes == NULL || size_input_enc_str== 0) {
		return ERR_RSA_KEY_BROKEN;
	}

	size_input_dec_str = rsa_dec(&dec_proc_bytes,
								 enc_proc_bytes,
								 rsa_d_numb,
								 rsa_n_numb);

	switch (size_input_dec_str){
		case ERR_RSA_KEY_SIZE:
			return ERR_RSA_KEY_SIZE;
		case ERR_RSA_KEY_FAILED:
			return ERR_RSA_KEY_FAILED;
		case ERR_RSA_KEY_BROKEN:
			return ERR_RSA_KEY_BROKEN;
	}

	size_output_file += size_input_dec_str / LEN_BYTE;

	all_output_bytes = (char *) malloc((size_output_file + 1) * sizeof(char));
	if (all_output_bytes == NULL) {
		return ERROR_MEMORY;
	}

	for (size_t n = 0, target_line = 2; size_input_dec_str != 0 ; target_line++) {
		if (n <= size_input_file / size_input_enc_str * size_input_dec_str / LEN_BYTE) {
			progress_bar(n , size_input_file / size_input_enc_str * size_input_dec_str / LEN_BYTE);
		}

		for (size_t i = 0, k = 0; i <= size_input_dec_str; i++, k++) {
			if (k == LEN_BYTE) {
				k = 0;
				int_to_byte[LEN_BYTE] = '\0';
				char_to_int = (atol(int_to_byte)) - CODE_SYMBOL;
				all_output_bytes[n] = char_to_int;
				n += 1;
			}
			int_to_byte[k] = dec_proc_bytes[i];
		}

		free(enc_proc_bytes);
		size_input_enc_str= text_read_line(input_file, &enc_proc_bytes, target_line);

		if (size_input_enc_str != 0) {
			free(dec_proc_bytes);
			size_input_dec_str = rsa_dec(&dec_proc_bytes,
								  		 enc_proc_bytes,
								  		 rsa_d_numb,
								  		 rsa_n_numb);
		}
		else {
			size_input_dec_str = 0;
		}

		if (size_input_dec_str != 0) {
			size_output_file += size_input_dec_str / LEN_BYTE;
			all_output_bytes = (char *) realloc(all_output_bytes, (size_output_file + 1) * sizeof(char));

			if (all_output_bytes == NULL) {
				return ERROR_MEMORY;
			}
		}
	}

	all_output_bytes[size_output_file] = '\0';
	progress_bar(1, 1);

	file_write(output_file, all_output_bytes, size_output_file);

	free(enc_proc_bytes);
	free(dec_proc_bytes);
	free(all_output_bytes);
	free(int_to_byte);
	free(rsa_d_numb);
	free(rsa_n_numb);

	return 0;
}
