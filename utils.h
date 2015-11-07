#ifndef UTILS_H
#define UTILS_H

#include <stdlib.h>
#include <stdint.h>
#include <sys/types.h>
#include <ifaddrs.h>


#define ntoh_2bytes(buf) ntoh_nbytes(buf, 2, 0)
#define ntoh_4bytes(buf) ntoh_nbytes(buf, 4, 0)
#define ntoh_6bytes(buf) ntoh_nbytes(buf, 6, 0)
#define ntoh_8bytes(buf) ntoh_nbytes(buf, 8, 0)

#define hton_nbytes(buf, len, silly_bits) ntoh_nbytes(buf, len, silly_bits)

#define hton_2bytes(buf) hton_nbytes(buf, 2, 0)
#define hton_4bytes(buf) hton_nbytes(buf, 4, 0)
#define hton_6bytes(buf) hton_nbytes(buf, 6, 0)
#define hton_8bytes(buf) hton_nbytes(buf, 8, 0)

#define hton(dbuf, len) ntoh(dbuf, len)

void put_byte(unsigned char *x, int n);

void swap_byte(unsigned char *b1, unsigned char *b2);

void ntoh_nbytes(unsigned char *buf, size_t len, size_t silly_bits);

void ntoh(unsigned char *dbuf, size_t len);

char *get_interface_name();

char *format_mac(char *mac);

uint64_t transform_mac_to_int64(unsigned char *mac);//char[6] to 0xFFFFFFFFFFFF

char* mac_upper(const char *str);

unsigned char *format(char *mac);// "FF-FF-FF-FF-FF-FF" or "FFFFFFFFFFFF" to unsigned char[6]

#endif
