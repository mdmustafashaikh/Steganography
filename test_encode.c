#include <stdio.h>
#include "encode.h"
#include "types.h"
#include "decode.h"



int main(int argc, char **argv)

{
    printf("\n----------Welcome To Steganography Project----------\n");
    if (argc > 1 && argc < 8)
    {
        OperationType op_type = check_operation_type(argv);

        if (op_type == e_encode)
        {
            EncodeInfo encInfo;
            if (read_and_validate_encode_args(argc, argv, &encInfo) == e_success)
            {
                if (do_encoding(&encInfo) == e_failure)
                {
                    fprintf(stderr,"❌ %s function failed\n","do_encoding()");
                    return 1;
                }
                fclose(encInfo.fptr_src_image);
                fclose(encInfo.fptr_secret);
                fclose(encInfo.fptr_stego_image);
                printf("## Encoding Done Successfully ##\n");
                printf("----------Thank You----------\n");
            }
            else
            {
                fprintf(stderr, "❌ %s function failed\n", "read_and_validate_encode_args()");
                return 1;
            }
        }
        else if (op_type == e_decode)
        {
            DecodeInfo decInfo;
            if (read_and_validate_decode_args(argc, argv, &decInfo) == e_success)
            {
                if (do_decoding(&decInfo) == e_success)
                {
                    printf("## Decoding Done Successfully ##\n");
                     printf("----------Thank You----------\n");
                    fclose(decInfo.fptr_stego_image);
                    fclose(decInfo.fptr_output_file);
                }
                else
                {
                    fprintf(stderr,"❌ %s function failed\n","do_decoding()");
                    return 1;
                }
            }
            else
            {
                fprintf(stderr," ❌ %s function failed\n", "read_and_validate_decode_args()");
                return 1;
            }
        }
        else
        {
            fprintf(stderr,"❌ Invalid option\n");
            printf("❌ ./lsb_steg: Encoding: ./lsb_steg -e <.bmp file> <.txt file> [Output file]\n./lsb_steg: Decoding: ./lsb_steg -d <.bmp file> [output file]\n");
            return 1;
        }
    }
    else
    {
        printf("❌ ./lsb_steg: Encoding: ./lsb_steg -e <.bmp file> <.txt file> [Output file]\n./lsb_steg: Decoding: ./lsb_steg -d <.bmp file> [output file]\n");
        return 1;
    }
    return 0;
}

