# LSB Image Steganography – C Project

## Overview

This project implements **LSB (Least Significant Bit) Image Steganography** in C.

The system allows secret data to be hidden inside a BMP image by modifying the least significant bits of the image data. The hidden information can later be extracted from the stego image.

## Features

- Hide secret text data inside a BMP image
- Extract hidden data from a stego image
- LSB-based data encoding
- BMP image handling
- File handling and binary data processing
- Command-line based execution
- Modular C implementation

## Technologies Used

- C Programming
- File Handling
- Bitwise Operations
- BMP Image Processing
- Data Encoding & Decoding
- Command-Line Programming

## Project Structure

- `encode.c` – Encoding implementation
- `encode.h` – Encoding function declarations
- `decode.c` – Decoding implementation
- `decode.h` – Decoding function declarations
- `common.h` – Common definitions
- `types.h` – Project data types
- `test_encode.c` – Main program and command-line handling

## How It Works

### Encoding

The program takes:

- A source BMP image
- A secret file
- An output BMP image

The secret data is embedded into the least significant bits of the image data.

### Decoding

The program reads the stego BMP image and extracts the hidden information from the modified LSBs.

## Learning Outcomes

Through this project, I gained hands-on experience in:

- C programming
- Bitwise operations
- File handling
- Image data processing
- Encoding and decoding techniques
- Modular programming
- Debugging and problem solving

## Project Type

Academic / Technical Project – C Programming
