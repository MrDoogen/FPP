#include <stddef.h>
#include <string.h>
#include <unistd.h>
#include <gmp.h>

#include "proto.h"


ssize_t rsa_enc (char **str_result,
                        char *str_m,
                        char *str_e,
                        char *str_n) {
	mpz_t e;
	mpz_t n;
	mpz_t m;
	mpz_t c;
	size_t size = 0;

	if (!(strlen(str_m) < strlen(str_n))) {
		return ERR_RSA_KEY_SIZE;
	}

	mpz_init(e);
	mpz_set_ui(e, 0);

	if (mpz_set_str(e, str_e, 10) != 0) {
		return ERR_RSA_KEY_FAILED;
	}

	mpz_init(n);
	mpz_set_ui(n, 0);

	if (mpz_set_str(n, str_n, 10) != 0) {
		return ERR_RSA_KEY_FAILED;
	}

	mpz_init(m);
	mpz_set_ui(m, 0);

	if (mpz_set_str(m, str_m, 10) != 0) {
		return ERR_RSA_KEY_BROKEN;
	}

	mpz_init(c);
	mpz_set_ui(c, 0);

	mpz_powm(c, m, e, n);

	size = mpz_sizeinbase(c, 10);
	*str_result = mpz_get_str(NULL, 10, c);

	return size;
}


ssize_t rsa_dec (char **str_result,
                       char *str_c,
                       char *str_d,
                       char *str_n) {
	mpz_t d;
	mpz_t n;
	mpz_t c;
	mpz_t m;
	size_t size = 0;

	mpz_init(d);
	mpz_set_ui(d, 0);

	if (mpz_set_str(d, str_d, 10) != 0) {
		return ERR_RSA_KEY_FAILED;
	}

	mpz_init(n);
	mpz_set_ui(n, 0);

	if (mpz_set_str(n, str_n, 10) != 0) {
		return ERR_RSA_KEY_FAILED;
	}

	mpz_init(c);
	mpz_set_ui(c, 0);

	if (mpz_set_str(c, str_c, 10) != 0) {
		return ERR_RSA_KEY_BROKEN;
	}

	mpz_init(m);
	mpz_set_ui(m, 0);

	mpz_powm(m, c, d, n);
	size = mpz_sizeinbase(m, 10);
	*str_result = mpz_get_str(NULL, 10, m);

	return size;
}
