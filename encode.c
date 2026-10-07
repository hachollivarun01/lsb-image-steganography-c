/*
Name        : H.Varun
Date        : 08/10/2023
Description : LSB Image steganography.
*/
#include <stdio.h>
#include "encode.h"
#include "types.h"
#include "common.h"

/* To Read & validate command lines argumnets */
Status read_and_validate_encode_args(char *argv[], EncodeInfo *encInfo)
{
	if( strcmp(strstr(argv[2], "."), ".bmp") == 0 )							// To check image is .bmp or not
	{
		//printf("Yes!, argv[2] is a .bmp file\n");
		encInfo->src_image_fname = argv[2];
		if( strcmp(strstr(argv[3], "."), ".txt") == 0 || strcmp(strstr(argv[3], "."), ".c") == 0 )
		{
			encInfo->secret_fname = argv[3];
			//printf("Yes!, argv[3] is a .txt file or .c file\n");
			if( argv[4] == NULL )
			{
				encInfo->stego_image_fname = "stego.bmp";
			}
			else
			{
				if( strcmp(strstr(argv[4], "."), ".bmp") == 0 )
				{
					encInfo->stego_image_fname = argv[4];							// Extract output file name given by user
				}
			}
			return e_success;
		}
		else
		{
			return e_failure;
		}
	}
	else
	{
		return e_failure;
	}
}

/* To open required files */
Status open_files(EncodeInfo *encInfo)
{
	printf("INFO: Opening required files\n");
    // Src Image file
    encInfo->fptr_src_image = fopen(encInfo->src_image_fname, "r");
    // Do Error handling
    if (encInfo->fptr_src_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->src_image_fname);

    	return e_failure;
    }

    // Secret file
    encInfo->fptr_secret = fopen(encInfo->secret_fname, "r");
    // Do Error handling
    if (encInfo->fptr_secret == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->secret_fname);

    	return e_failure;
    }

    // Stego Image file
    encInfo->fptr_stego_image = fopen(encInfo->stego_image_fname, "w");
    // Do Error handling
    if (encInfo->fptr_stego_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->stego_image_fname);

    	return e_failure;
    }

    // No failure return e_success
    return e_success;
}

/* Checking source image capaticy to store message */
Status check_capacity( EncodeInfo *encInfo)
{
	printf("INFO: Checking for %s size\n", encInfo->secret_fname);
	encInfo->size_secret_file = get_file_size(encInfo->fptr_secret);			// Storing size of secret file
	encInfo->image_capacity = get_image_size_for_bmp(encInfo->fptr_src_image);		// Storing image capacity
	
	printf("INFO: Checking for %s capacity to handle %s\n", encInfo->src_image_fname, encInfo->secret_fname);

	if( encInfo->image_capacity > (54+(2+4+4+4+(encInfo->size_secret_file))*8) )		// Checking capacity
	{
		return e_success;
	}
	else
	{
		return e_failure;
	}
}

/* Function to get file size */
Status get_file_size(FILE *fptr)
{
	fseek(fptr, 0, SEEK_END);
	return ftell(fptr);
}

/* Function Definitions */

/* Get image size
 * Input: Image file ptr
 * Output: width * height * bytes per pixel (3 in our case)
 * Description: In BMP Image, width is stored in offset 18,
 * and height after that. size is 4 bytes
 */
uint get_image_size_for_bmp(FILE *fptr_image)
{
    uint width, height;
    // Seek to 18th byte
    fseek(fptr_image, 18, SEEK_SET);						// After 18 bytes pointer pointing to width of image (pixel)

    // Read the width (an int)
    fread(&width, sizeof(int), 1, fptr_image);				// Storing width (pixel) in width variables
    //printf("width = %u\n", width);

    // Read the height (an int)
    fread(&height, sizeof(int), 1, fptr_image);				// Storing height (pixel) in height variable
    //printf("height = %u\n", height);

    // Return image capacity
    return width * height * 3;								// So "width*height" will give size in pixels, each pixel having 3 bytes
}

/* 
 * Get File pointers for i/p and o/p files
 * Inputs: Src Image file, Secret file and
 * Stego Image file
 * Output: FILE pointer for above files
 * Return Value: e_success or e_failure, on file errors
 */


Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_dest_image)
{
	printf("INFO: Copying Image Header\n");
	char buffer[54];									// For temperary storage
	fseek(fptr_src_image, 0, SEEK_SET);					// Set file pointer to 0 pos
	fread(&buffer, 54, 1, fptr_src_image);				// Reading 54 bytes(header) from beautiful.bmp
	fwrite(&buffer, 54, 1, fptr_dest_image);			// Writting 54 bytes in stego.bmp

	return e_success;
}

/* To Encode magic string 
*  Calling function to encode data into image 
*/
Status encode_magic_string(char *magic_string, EncodeInfo *encInfo)
{
	printf("INFO: Encoding Magic String Signature\n");
	encode_data_to_image(magic_string, strlen(MAGIC_STRING), encInfo->fptr_src_image, encInfo->fptr_stego_image, encInfo);
}

/* To Encode data from image */
Status encode_data_to_image(char *data, int size, FILE *fptr_src_image, FILE *fptr_stego_image, EncodeInfo *encInfo)
{
	int i;
//	char buffer[8];
	for( i = 0; i < size; i++)													// Run loop till no of characters
	{
		fread(encInfo -> image_data, 8, 1, fptr_src_image);
		encode_byte_to_lsb(data[i], encInfo->image_data);								// Calling function to encode bytes from LSB
		fwrite(encInfo->image_data, 8, 1, fptr_stego_image);								// Storing data to image
	}
	return e_success;
}

/* Decoding bytes from LSB */
Status encode_byte_to_lsb(char data, char *image_data)
{
	int i;
	for( i = 0; i < 8; i++)											// Run loop for 8 times because for 1 char we have to decode 8 byte
	{
		image_data[i] = (image_data[i] & 0xfe) | ((data & (1 << i))) >> i;					// Logic to encode bits
	}	
}

/* Function for encoding secret file extension size */
Status encode_file_extention_size(int size, FILE *fptr_src_image, FILE *fptr_stego_image)
{
	char buffer[32];
	fread(&buffer, 32, 1, fptr_src_image);
	encode_size_to_lsb(buffer, size);										// Calling function encode size to lsb
	fwrite(&buffer, 32, 1, fptr_stego_image);									// Storing data to output image
	return e_success;	
}

/* Function to encode size to lsb */
Status encode_size_to_lsb(char *buffer, int size)
{
	int i;
	for( i = 0; i < 32; i++)										// size is int, so we have to fatch 32 bytes
	{
		buffer[i] = (buffer[i] & 0xfe) | ((size & (1 << i))) >> i;					// Logic to encode bits for size
	}
}

/* Function for encode secret file extension  */
Status encode_secret_file_extn(char *file_extn, EncodeInfo *encInfo)
{
	printf("INFO: Encoding %s File Extension\n", encInfo->secret_fname);
	encode_data_to_image(file_extn, strlen(file_extn), encInfo->fptr_src_image, encInfo->fptr_stego_image, encInfo);
	return e_success;
}

/* Encode secret file data size */
Status encode_secret_file_size(long file_size, EncodeInfo *encInfo)
{
	printf("INFO: Encoding %s File Size\n", encInfo->secret_fname);
	char buffer[32];
	fread(&buffer, 32, 1, encInfo->fptr_src_image);
	encode_size_to_lsb(buffer, file_size);
	fwrite(&buffer, 32, 1, encInfo->fptr_stego_image);
	return e_success;
}

/* Encoding secret file data into image */
Status encode_secret_file_data(EncodeInfo *encInfo)
{
	printf("INFO: Encoding %s File Data\n", encInfo->secret_fname);
	char buffer[encInfo->size_secret_file];
	//int offset = ftell(encInfo->fptr_src_image);
	fseek(encInfo->fptr_secret,0,SEEK_SET);
	fread(&buffer, encInfo->size_secret_file, 1, encInfo->fptr_secret);
	//fseek(encInfo->fptr_src_image, offset,SEEK_SET);
	encode_data_to_image(buffer, encInfo->size_secret_file, encInfo->fptr_src_image, encInfo->fptr_stego_image, encInfo);
	return e_success;
}

/* Copying remaining data from image to output image */
Status copy_remaining_img_data(FILE *fptr_src_image, FILE *fptr_dest_image)
{
	printf("INFO: Copying Left Over Data\n");
	char buffer;
	while(fread(&buffer,1,1,fptr_src_image) > 0)
	{
		fwrite(&buffer,1,1,fptr_dest_image);
	}
	
	return e_success;
}

/* Function for encoding data */
Status do_encoding(EncodeInfo *encInfo)
{
	if(open_files(encInfo) == e_success)
	{
		printf("INFO: Opened %s\n", encInfo->src_image_fname);
		printf("INFO: Opened %s\n", encInfo->secret_fname);
		printf("INFO: Opened %s\n", encInfo->stego_image_fname);
		printf("INFO: Done\n");
		if( check_capacity(encInfo) == e_success)
		{
			printf("INFO: Done. Found OK\n");
			if( copy_bmp_header(encInfo->fptr_src_image, encInfo->fptr_stego_image) == e_success )
			{
				printf("INFO: Done\n");
				if( encode_magic_string(MAGIC_STRING, encInfo) == e_success )
				{
					printf("INFO: Done\n");
					if( encode_file_extention_size(strlen(strstr(encInfo->secret_fname, ".")), encInfo->fptr_src_image, encInfo->fptr_stego_image) == e_success)
					{
						//printf("Encoding of file extension size is success\n");
						if( encode_secret_file_extn(strstr(encInfo->secret_fname, "."), encInfo) == e_success)
						{
							printf("INFO: Done\n");
							if( encode_secret_file_size(encInfo->size_secret_file, encInfo) == e_success)
							{
								printf("INFO: Done\n");
								if(encode_secret_file_data(encInfo) == e_success)
								{
									printf("INFO: Done\n");
									if(copy_remaining_img_data(encInfo->fptr_src_image, encInfo->fptr_stego_image) == e_success)
									{
										printf("INFO: Done\n");
										return e_success;
									}	
								}
							}
						}
					}
				}
			}
		}
	}
}

