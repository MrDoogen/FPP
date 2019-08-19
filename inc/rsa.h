#ifndef RSA_H
#define RSA_H
#endif

ssize_t rsa_enc (char **str_result, char *str_m, char *str_e, char *str_n);
ssize_t rsa_dec (char **str_result, char *str_c, char *str_d, char *str_n);
