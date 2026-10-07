/*
Name        : H.Varun
Date        : 08/10/2023
Description : LSB Image steganography.
*/
#include <stdio.h>
#include "decode.h"
#include "types.h"
#include "common.h"

/* To Read & validate command lines argumnets */
Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo)
{
	if( strcmp(strstr(argv[2], "."), ".bmp") == 0 )							// To check image is .bmp or not
	{
		//printf("Yes!, argv[2] is a .bmp file\n");
		decInfo->stego_image_fname = argv[2];
		if( argv[3] == NULL)
		{
			decInfo->decode_fname = "output";
		}
		else
		{
			char *str = strtok(argv[3], ".");								// Extract output file name given by user
			decInfo->decode_fname = str;
		}
	}
	else
	{
		return e_failure;
	}
	return e_success;
}

/* To open required files */
Status open_files_d(DecodeInfo *decInfo)
{
	printf("INFO: Opening required files\n");
    // Stego Image file
    decInfo->fptr_stego_image = fopen(decInfo->stego_image_fname, "r");
    // Do Error handling
    if (decInfo->fptr_stego_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", decInfo->stego_image_fname);

    	return e_failure;
    }

    // No failure return e_success
    return e_success;
}

/* To decode magic string */
Status decode_magic_string(DecodeInfo *decInfo)
{
	printf("INFO: Decoding Magic String Signature\n");
	fseek(decInfo->fptr_stego_image, 54, SEEK_SET);
	decode_data_to_image(strlen(MAGIC_STRING), decInfo->fptr_stego_image, decInfo);		// Calling function to extract data from image

	return e_success;
}

/* To decode data from image */
Status decode_data_to_image(int len, FILE *fptr_stego_image, DecodeInfo *decInfo)
{
	int i;
	char buffer[8], magic_str[len+1];
	for( i = 0; i < len; i++)													// Run loop till no of characters
	{
		fread(buffer, 8, 1, fptr_stego_image);
		decode_byte_to_lsb(buffer, &magic_str[i]);								// Calling function to decode bytes from LSB
	}
	magic_str[len] = '\0';

	if( strcmp(magic_str, "#*") == 0 )											// To comparing extracted string is "#*" or not.
	{
		return e_success;
	}
	else if( strcmp(magic_str, ".txt") == 0 )
	{
		for(int k = 0; magic_str[k] != 0; k++)
		{
			decInfo->file_extn[k] = magic_str[k];								// Storing string to file_extn
		}
		decInfo->file_extn[4]='\0';
		return e_success;
	}
	else if( strcmp(magic_str, ".c") == 0 )
	{
		for(int k = 0; magic_str[k] != 0; k++)
		{
			decInfo->file_extn[k] = magic_str[k];
		}
		decInfo->file_extn[2]='\0';
		return e_success;
	}
	else if(decInfo->file_size == (strlen(magic_str)))
	{
		for(int k = 0; magic_str[k] != 0; k++)
		{
			secret_data[k] = magic_str[k];
		}
		return e_success;
	}
	else
	{
		return e_failure;
	}
}

/* Decoding bytes from LSB */
Status decode_byte_to_lsb(char *buffer, char *magic_str)
{
	int i;
	char temp = 0;
	for( i = 0; i < 8; i++)											// Run loop for 8 times because for 1 char we have to decode 8 byte
	{
		temp = ((buffer[i] & 0x01) << i) | temp;					// Logic to extract bits
	}
	*magic_str = temp;
}

/* Function for decoding secret file extension size */
Status decode_file_extention_size(FILE *fptr_stego_image, DecodeInfo *decInfo)
{
	long size = 0;
	char buffer[32];
	fread(&buffer, 32, 1, fptr_stego_image);
	decode_size_to_lsb(buffer, &size);								// Calling size to lsb function
	decInfo->extn_size = size;										// Storing size into extn_size

	return e_success;	
}

/* Decoding function for decode bytes for size */
Status decode_size_to_lsb(char *buffer, long *size)
{
	int i;
	for( i = 0; i < 32; i++)										// size is int, so we have to fatch 32 bytes
	{
		*size = ((buffer[i] & 0x01) << i) | *size;					// Logic to decode bits for size
	}
}

/* Function for decoding secret file extension */
Status decode_secret_file_extn(DecodeInfo *decInfo)
{
	printf("INFO: Decoding Output File Extension\n");
	decode_data_to_image(decInfo->extn_size, decInfo->fptr_stego_image, decInfo);

	return e_success;
}

/* Function to join name & extension*/
Status concate(DecodeInfo *decInfo)
{
	//char fname[ decInfo->extn_size + strlen(decInfo->decode_fname) ];
	
	decInfo->file_extn[4]='\0';
	
	strcpy(fname,decInfo->decode_fname);									// Copying file name to fname
	strcat(fname,decInfo->file_extn);										// Concate function to make file name with extension

	decInfo->fptr_decode_file = fopen(fname, "w");							// Open output file in write mode
    if (decInfo->fptr_decode_file == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", fname);

    	return e_failure;
    }
	printf("INFO: Opened %s \nINFO: Done. Opened all required files\n", fname);

	return e_success;
}

/* Function for decoding secret data size */
Status decode_secret_file_size(DecodeInfo *decInfo)
{
	printf("INFO: Decoding %s File size\n", fname);
	long file_size = 0;
  	char buffer[32];
	fread(&buffer, 32, 1, decInfo->fptr_stego_image);
	decode_size_to_lsb(buffer, &file_size);
	decInfo->file_size = file_size;
	
	return e_success;
}


/* Function for decoding secret data */
Status decode_secret_file_data(DecodeInfo *decInfo)
{
	printf("INFO: Decoding %s File Data\n", fname);
	decode_data_to_image(decInfo->file_size, decInfo->fptr_stego_image, decInfo);
	
	for(int i = 0; secret_data[i] != '\0'; i++)									// Run loop till data reached to end
		fputc(secret_data[i], decInfo->fptr_decode_file);						// Store information to decoded file
	
	return e_success;
}

/* Function for decoding */
Status do_decoding(DecodeInfo *decInfo)
{
	if(open_files_d(decInfo) == e_success)
	{
		printf("INFO: Opened %s\n", decInfo->stego_image_fname);
		if( decode_magic_string(decInfo) == e_success )
		{
			printf("INFO: Done\n");
			if( decode_file_extention_size(decInfo->fptr_stego_image, decInfo) == e_success)
			{
				//printf("Decoding of file extension size is success\n");
				if( decode_secret_file_extn(decInfo) == e_success)
				{
					printf("INFO: Done\n");
					if( concate(decInfo) == e_success)
					{
						//printf("concate file name is success\n");
					}
					if( decode_secret_file_size(decInfo) == e_success)
					{
					  	  printf("INFO: Done\n");
					  	  if(decode_secret_file_data(decInfo) == e_success)
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
