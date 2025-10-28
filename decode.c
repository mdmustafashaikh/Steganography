#include <stdio.h>
#include <string.h>
#include <stdlib.h> 
#include "decode.h"
#include "types.h"



static Status open_stego_image(DecodeInfo *decInfo)
{
    if (!decInfo || !decInfo->stego_image_fname){
        fprintf(stderr, "Invalid decode info or stego image name.\n");
        return e_failure;
    }
    decInfo->fptr_stego_image = fopen(decInfo->stego_image_fname, "rb");
    if (decInfo->fptr_stego_image == NULL)
    {
        perror("fopen");
        fprintf(stderr,"Unable to open stego image file %s\n", decInfo->stego_image_fname);
        return e_failure;
    }
    printf("Opened %s\n", decInfo->stego_image_fname);

    return e_success;
}
static Status decode_uint_from_lsbs(uint *decoded_value, int num_lsbs_to_read, DecodeInfo *decInfo){
    char *buffer = malloc(num_lsbs_to_read);
    if (buffer == NULL){
        fprintf(stderr, "Failed to allocate memory for size decoding.\n");
        return e_failure;
    }
    if (fread(buffer, 1, num_lsbs_to_read, decInfo->fptr_stego_image) != num_lsbs_to_read){
        fprintf(stderr, "Could not read data for size information from stego image.\n");
        free(buffer);
        return e_failure;
    }

    *decoded_value = 0;
    for (int i = 0; i < num_lsbs_to_read; i++)
    {
        *decoded_value = (*decoded_value << 1) | (buffer[i] & 0x01);
    }
    free(buffer);
    return e_success;
}
Status read_and_validate_decode_args(int argc, char *argv[], DecodeInfo *decInfo)
{
    if (argc < 3)
    {
        fprintf(stderr, "Arguments are missing.\nUsage: %s -d stego_image.bmp\n", argv[0]);
        return e_failure;
    }
    if (strcmp(argv[1], "-d") != 0)
    {
        fprintf(stderr, "Invalid option. Use -d for decoding.\nUsage: %s -d stego_image.bmp\n", argv[0]);
        return e_failure;
    }
    if (strstr(argv[2], ".bmp") == NULL)
    {
        fprintf(stderr, "Stego image %s format should be .bmp\n", argv[2]);
        return e_failure;
    }
    decInfo->stego_image_fname = argv[2];
    if (argc > 3){
        decInfo->output_file_name = argv[3];
    }
    else{
        decInfo->output_file_name = NULL; 
    }
    return e_success;
}

Status do_decoding(DecodeInfo *decInfo)
{
    printf("## Decoding Procedure Started ##\n");

    printf("Opening required files\n");
    if (open_stego_image(decInfo) != e_success)
    {
        fprintf(stderr, "Error: Failed to open stego image.\n");
        return e_failure;
    }
    printf("Done\n");

    uint pixel_data_offset = 0;
    fseek(decInfo->fptr_stego_image, 10L, SEEK_SET);
    fread(&pixel_data_offset, sizeof(uint), 1, decInfo->fptr_stego_image);
    fseek(decInfo->fptr_stego_image, pixel_data_offset, SEEK_SET);
   

    printf("Decoding Magic String\n");
    if (decode_magic_string(decInfo) != e_success)
    {
        fprintf(stderr, "Magic string does not match. The file may not contain the expected data.\n");
        fclose(decInfo->fptr_stego_image);
        return e_failure;
    }
    printf("Done. Magic String is correct.\n");

    printf("Decoding Output File Extension Size\n");
    if (decode_output_file_extn_size(decInfo) != e_success)
    {
        fprintf(stderr, "Failed to decode file extension size.\n");
        fclose(decInfo->fptr_stego_image);
        return e_failure;
    }
    printf("Done\n");

    printf("Decoding Output File Extension\n");
    if (decode_output_file_extn(decInfo->image_data_size, decInfo) != e_success)
    {
        fprintf(stderr, "Error: Failed to decode file extension.\n");
        fclose(decInfo->fptr_stego_image);
        return e_failure;
    }
    printf("Decoded extension is \"%s\"\n", decInfo->extn_output_file);

    if (decInfo->output_file_name == NULL)
    {
        size_t len = strlen("decoded_output") + strlen(decInfo->extn_output_file) + 1;
        char *default_output = malloc(len);
        if (default_output == NULL)
        {
            fprintf(stderr, "Memory allocation failed for default output file name.\n");
            fclose(decInfo->fptr_stego_image);
            return e_failure;
        }
        strcpy(default_output, "decoded_output");
        strcat(default_output, decInfo->extn_output_file);
        decInfo->output_file_name = default_output;
        printf("No output file specified. Creating %s\n", decInfo->output_file_name);
    }
    decInfo->fptr_output_file = fopen(decInfo->output_file_name, "wb");
    if (decInfo->fptr_output_file == NULL)
    {
        perror("fopen");
        fprintf(stderr, "Unable to open output file %s\n", decInfo->output_file_name);
        fclose(decInfo->fptr_stego_image);
        return e_failure;
    }
    printf("Opened output file %s\n", decInfo->output_file_name);

    printf("Decoding File Size\n");
    if (decode_file_size(decInfo) != e_success)
    {
        fprintf(stderr, "Failed to decode file size.\n");
        fclose(decInfo->fptr_stego_image);
        fclose(decInfo->fptr_output_file);
        return e_failure;
    }
    printf("Decoded file size is %u bytes\n", decInfo->image_data_size);

    printf("Decoding file data and writing to output file\n");
    if (decode_data_to_file(decInfo) != e_success){
        fprintf(stderr, "Failed to decode and write file data.\n");
        fclose(decInfo->fptr_stego_image);
        fclose(decInfo->fptr_output_file);
        return e_failure;
    }

    // printf("## Decoding Done Successfully ##\n");
    return e_success;
}
Status decode_magic_string(DecodeInfo *decInfo){
    printf("Enter the expected Magic String: ");
    fgets(decInfo->expected_magic_string, sizeof(decInfo->expected_magic_string), stdin);
    decInfo->expected_magic_string[strcspn(decInfo->expected_magic_string, "\n")] = 0;

    size_t magic_len = strlen(decInfo->expected_magic_string);
    if (magic_len == 0)
    {
        fprintf(stderr, "Magic string cannot be empty.\n");
        return e_failure;
    }
    char decoded_char;
    for (size_t i = 0; i < magic_len; i++){
        if (fread(decInfo->image_data, 1, MAX_IMAGE_BUF_SIZE, decInfo->fptr_stego_image) != MAX_IMAGE_BUF_SIZE){
            fprintf(stderr, "Reached end of file while decoding magic string.\n");
            return e_failure;
        }

        if (decode_lsb_to_byte(&decoded_char, decInfo->image_data) != e_success){
            return e_failure; 
        }

        if (decoded_char != decInfo->expected_magic_string[i]){
            return e_failure; 
        }
    }

    return e_success;
}

Status decode_output_file_extn_size(DecodeInfo *decInfo){
 
    return decode_uint_from_lsbs(&decInfo->image_data_size, DECODE_FILE_EXTN_SIZE, decInfo);
}

Status decode_output_file_extn(uint extn_size, DecodeInfo *decInfo)
{
    if (extn_size > MAX_FILE_SUFFIX){
        fprintf(stderr, "Decoded extension size (%u) is larger than the allowed maximum (%d).\n", extn_size, MAX_FILE_SUFFIX);
        return e_failure;
    }

    char decoded_char;
    for (uint i = 0; i < extn_size; i++)
    {
        if (fread(decInfo->image_data, 1, MAX_IMAGE_BUF_SIZE, decInfo->fptr_stego_image) != MAX_IMAGE_BUF_SIZE)
        {
            fprintf(stderr, "Reached end of file while decoding file extension.\n");
            return e_failure;
        }

        if (decode_lsb_to_byte(&decoded_char, decInfo->image_data) != e_success)
        {
            return e_failure;
        }
        decInfo->extn_output_file[i] = decoded_char;
    }
    decInfo->extn_output_file[extn_size] = '\0';
    return e_success;
}

Status decode_file_size(DecodeInfo *decInfo){
   
    return decode_uint_from_lsbs(&decInfo->image_data_size, DECODE_FILE_SIZE, decInfo);
}

Status decode_data_to_file(DecodeInfo *decInfo)
{
    char decoded_byte;
    for (uint i = 0; i < decInfo->image_data_size; i++){
       
        if (fread(decInfo->image_data, 1, MAX_IMAGE_BUF_SIZE, decInfo->fptr_stego_image) != MAX_IMAGE_BUF_SIZE)
        {
            fprintf(stderr, "Unexpected end of file while decoding secret data.\n");
            return e_failure;
        }

        if (decode_lsb_to_byte(&decoded_byte, decInfo->image_data) != e_success)
        {
            fprintf(stderr, "LSB decoding failed on byte %u of secret data.\n", i + 1);
            return e_failure;
        }

        if (fwrite(&decoded_byte, 1, 1, decInfo->fptr_output_file) != 1)
        {
            fprintf(stderr, "Failed to write decoded byte to output file.\n");
            return e_failure;
        }
    }
    return e_success;
}

Status decode_lsb_to_byte(char *decoded_byte, char *image_buffer){
    
    *decoded_byte = 0;
    for (uint i = 0; i < MAX_IMAGE_BUF_SIZE; i++)
    {
        *decoded_byte = (*decoded_byte << 1) | (image_buffer[i] & 0x01);
    }
    return e_success;
}