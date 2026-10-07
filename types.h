/*
Name        : H.Varun
Date        : 08/10/2023
Description : LSB Image steganography.
Documentation*/
#ifndef TYPES_H
#define TYPES_H

/* User defined types */
typedef unsigned int uint;

/* Status will be used in fn. return type */
typedef enum
{
    e_success,				// Perticular proccess is completed, return e_sucess
    e_failure				// Perticular proccess is not completed, return e_faiure
} Status;

typedef enum
{
    e_encode,				// If argv[1] is "-e", return e_encode
    e_decode,				// If argv[1] is "-d", return e_decode
    e_unsupported			// If argv[1] is otherthan above 2, return e_unsupported
} OperationType;

#endif
