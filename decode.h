/*
Name        : H.Varun
Date        : 08/10/2023
Description : LSB Image steganography.
*/
#ifndef DECODE_H
#define DECODE_H

#include "types.h" // Contains user defined types
#include <string.h>

#define MAX_SECRET_BUF_SIZE 1								// Encode 1 bit
#define MAX_IMAGE_BUF_SIZE (MAX_SECRET_BUF_SIZE * 8)		// Encode 8 bit (1 byte)
#define MAX_FILE_SUFFIX 4

typedef struct _DecodeInfo
{
    /* Stego Image Info */
    char *stego_image_fname;						// Store name of stego image (stego.bmp or name.bmp)
    FILE *fptr_stego_image;							// File pointer for stego image

	char *decode_fname;	
	FILE *fptr_decode_file;
	char file_extn[MAX_FILE_SUFFIX];

	int extn_size;
	int file_size;

} DecodeInfo;

char secret_data[1000];					// Char array To store decoded message
char fname[100];						// Char array to store output file name

/* Decoding function prototype */

/* Check operation type */
OperationType check_operation_type(char *argv[]);

/* Read and validate Decode args from argv */
Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo);

/* Perform the decoding */
Status do_decoding(DecodeInfo *decInfo);

/* Get File pointers for i/p and o/p files */
Status open_files_d(DecodeInfo *decInfo);

/* Decode Magic String */
Status decode_magic_string(DecodeInfo *decInfo);

/* Decode a size from LSB of image data array */
Status decode_size_to_lsb(char *buffer, long *size);

/* Decode secret file extenstion size */
Status decode_file_extention_size(FILE *fptr_stego_image, DecodeInfo *decInfo);

/* Decode secret file extenstion */
Status decode_secret_file_extn(DecodeInfo *decInfo);

/* Decode secret file size */
Status decode_secret_file_size(DecodeInfo *decInfo);

/* Decode secret file data*/
Status decode_secret_file_data(DecodeInfo *decInfo);

/* Decode function, which does the real Decoding */
Status decode_data_to_image(int len, FILE *fptr_stego_image, DecodeInfo *decInfo);

/* Decode a byte into LSB of image data array */
Status decode_byte_to_lsb(char *buffer, char *magic_str);

#endif

