/*
Name        : H.Varun
Date        : 08/10/2023
Description : LSB Image steganography.
*/
#ifndef ENCODE_H
#define ENCODE_H

#include "types.h" // Contains user defined types
#include <string.h>
/* 
 * Structure to store information required for
 * encoding secret file to source Image
 * Info about output and intermediate data is
 * also stored
 */

#define MAX_SECRET_BUF_SIZE 1								// Encode 1 bit
#define MAX_IMAGE_BUF_SIZE (MAX_SECRET_BUF_SIZE * 8)		// Encode 8 bit (1 byte)
#define MAX_FILE_SUFFIX 4

typedef struct _EncodeInfo
{
    /* Source Image info */
    char *src_image_fname;							// Store name of source image file (beautiful.bmp)
    FILE *fptr_src_image;							// File pointer for source image file
    uint image_capacity;							// To check size for encode process, memory is available or not
    uint bits_per_pixel;
    char image_data[MAX_IMAGE_BUF_SIZE];			// Array to store content read from source image file

    /* Secret File Info */
    char *secret_fname;								// Store name of secret file (secret.txt or sectre.c) 
    FILE *fptr_secret;								// File pointer for secrect file
    char extn_secret_file[MAX_FILE_SUFFIX];
    char secret_data[MAX_SECRET_BUF_SIZE];			// Array to store secret data
    long size_secret_file;							// Size of the secret file

    /* Stego Image Info */
    char *stego_image_fname;						// Store name of stego image (stego.bmp or name.bmp)
    FILE *fptr_stego_image;							// File pointer for stego image

} EncodeInfo;


/* Encoding function prototype */

/* Check operation type */
OperationType check_operation_type(char *argv[]);

/* Read and validate Encode args from argv */
Status read_and_validate_encode_args(char *argv[], EncodeInfo *encInfo);

/* Perform the encoding */
Status do_encoding(EncodeInfo *encInfo);

/* Get File pointers for i/p and o/p files */
Status open_files(EncodeInfo *encInfo);

/* check capacity */
Status check_capacity(EncodeInfo *encInfo);

/* Get image size */
uint get_image_size_for_bmp(FILE *fptr_image);

/* Get file size */
uint get_file_size(FILE *fptr);

/* Copy bmp image header */
Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_dest_image);

/* Store Magic String */
Status encode_magic_string(char *magic_string, EncodeInfo *encInfo);

/* Encode secret file extension size */
Status encode_file_extention_size(int size, FILE *fptr_src_image, FILE *fptr_stego_image);

/* Encode size into LSB of image data array */
Status encode_size_to_lsb(char *buffer, int size);

/* Encode secret file extenstion */
Status encode_secret_file_extn(char *file_extn, EncodeInfo *encInfo);

/* Encode secret file size */
Status encode_secret_file_size(long file_size, EncodeInfo *encInfo);

/* Encode secret file data*/
Status encode_secret_file_data(EncodeInfo *encInfo);

/* Encode function, which does the real encoding */
Status encode_data_to_image(char *data, int size, FILE *fptr_src_image, FILE *fptr_stego_image, EncodeInfo *encInfo);

/* Encode a byte into LSB of image data array */
Status encode_byte_to_lsb(char data, char *image_data);

/* Copy remaining image bytes from src to stego image after encoding */
Status copy_remaining_img_data(FILE *fptr_src_image, FILE *fptr_dest_image);

#endif
