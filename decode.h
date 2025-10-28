#ifndef DECODE_H
#define DECODE_H
#include "types.h" 
#include "common.h"


#define DECODE_FILE_EXTN_SIZE 32
#define DECODE_FILE_SIZE 32
#define MAX_IMAGE_BUF_SIZE 8
#define MAX_FILE_SUFFIX 4

typedef struct  _DecodeInfo
{
	/* Encoded stego image info */
	char *stego_image_fname;
	FILE *fptr_stego_image;
	uint image_data_size;
	char image_data[MAX_IMAGE_BUF_SIZE];
	char decode_data[1];
	char expected_magic_string[64];
	char extn_output_file[MAX_FILE_SUFFIX + 1]; // +1 for null terminator
	FILE *fptr_output_file;
	char *output_file_name;

}	DecodeInfo;

Status read_and_validate_decode_args(int argc, char *argv[], DecodeInfo *decInfo);

static Status open_decode_files(DecodeInfo *decInfo);

Status do_decoding(DecodeInfo *decInfo);

Status decode_magic_string(DecodeInfo *decInfo);

Status decode_output_file_extn_size(DecodeInfo *decInfo);

Status decode_output_file_extn(uint extn_size, DecodeInfo *decInfo);

Status decode_file_size(DecodeInfo *decInfo);

Status decode_data_to_file(DecodeInfo *decInfo);

Status decode_lsb_to_byte(char *decode_data, char *image_data);

#endif
