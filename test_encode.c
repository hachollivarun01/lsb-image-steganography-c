/*
Name        : H.Varun
Date        : 08/10/2023
Description : LSB Image steganography.
*/
#include <stdio.h>
#include "encode.h"
#include "decode.h"
#include "types.h"

int main(int argc, char *argv[])
{
	if(argc > 1)
	{
		/* To check Encoding selected by user */
		if( check_operation_type(argv) == e_encode )
		{
			if(argc < 4)
			{
				printf("Encoding is not possible, please pass more than 3 arguments\n");
				printf("Usage :./a.out -e <.bmp file> <.txt file> [output file]\n");
			}

			if(argc > 3)
			{
				if( check_operation_type(argv) == e_encode )
				{
					printf("INFO: ## Encoding Procedure Started ##\n");
					EncodeInfo encInfo;
					/* To check proper arguments passed or not */
					if(read_and_validate_encode_args(argv, &encInfo) == e_success)
					{
						printf("INFO: Read & validation is success\n");
						if(do_encoding(&encInfo) == e_success)
						{
							printf("INFO: ## Encoding Done Successfully ##\n");
						}
					}
					else
					{
						printf("Usage :./a.out -e <.bmp file> <.txt file> [output file]\n");
					}
				}
			}
		}
		/* To check Decoding selected by user */
		else if( check_operation_type(argv) == e_decode )
		{
			if(argc < 3)
			{
				printf("Decoding is not possible, please pass more than 2 arguments\n");
				printf("Usage :./a.out -d <.bmp file> [output file]\n");
			}

			if(argc > 2)
			{
				if( check_operation_type(argv) == e_decode )
				{
					printf("INFO: ## Decoding Procedure Started ##\n");
					DecodeInfo decInfo;
					/* To check proper arguments passed or not */
					if(read_and_validate_decode_args(argv, &decInfo) == e_success)
					{
						printf("INFO: Read & validation is success\n");
						if(do_decoding(&decInfo) == e_success)
						{
							printf("INFO: ## Decoding Done Successfully ##\n");
						}
					}
					else
					{
						printf("Usage :./a.out -d <.bmp file> <.txt file> [output file]\n");
					}
				}
			}
		}
		else
		{
			printf("./a.out: Encoding: ./a.out -e <.bmp file> <.txt file> [output file]\n");
			printf("./a.out: Decoding: ./a.out -d <.bmp file> <.txt [output file]\n");
		}
	}
	else
	{
		printf("./a.out: Encoding: ./a.out -e <.bmp file> <.txt file> [output file]\n");
		printf("./a.out: Decoding: ./a.out -d <.bmp file> <.txt [output file]\n");
	}

	return 0;
}

/* Operation to check which option selected */
OperationType check_operation_type(char *argv[])
{
	if( strcmp(argv[1], "-e") == 0 )
	{
		return e_encode;
	}
	else if( strcmp(argv[1], "-d") == 0 )
	{
		return e_decode;
	}
	return e_unsupported;
}
