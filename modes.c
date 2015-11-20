#include <stdio.h>
#include <string.h>
#include "rijndael-alg-fst.h"

#define ENCRYPT 0
#define DECRYPT 1

int hex_decode(char *src, unsigned char **buffer);
void print_buffer(unsigned char *buffer, int buffer_size);

int main (int argc, char **argv)
{
    if (argc < 3)
    {
        printf("Invalid number of arguments: <key> [-e | -d] <input>\n");
        exit(1);
    }

    int mode = (strcmp("-d", argv[2]) == 0) ? DECRYPT : ENCRYPT;

    unsigned char *input;
    int input_size = hex_decode(argv[3], &input);

    unsigned char *key;
    int key_size = hex_decode(argv[1], &key);

    printf("mode: %s\n", mode == DECRYPT ? "decrypt" : "encrypt");

    block_state *block = malloc(sizeof(block_state));
    if (block) memset(block, 0, sizeof(block_state));

    unsigned char *e_buffer = malloc(input_size);
    unsigned char *d_buffer = malloc(input_size);

    block_init(block, key, key_size);
    block_encrypt(block, input, e_buffer);
    block_decrypt(block, e_buffer, d_buffer);

    printf("input buffer:\n");
    print_buffer(input, input_size);

    printf("encrypted buffer:\n");
    print_buffer(e_buffer, input_size);

    printf("decrypted buffer:\n");
    print_buffer(d_buffer, input_size);

    printf("\n");

    free(e_buffer);
    free(d_buffer);
    free(block);
    free(input);
    free(key);
}

int hex_decode(char *src, unsigned char **buffer)
{
    int len = strlen(src);
    int buffer_size = (len / 2);

    *buffer = (unsigned char *)malloc(buffer_size * sizeof(unsigned char));

    unsigned char *dst = *buffer;
    unsigned char *end = *buffer + buffer_size;
    unsigned int u;

    while (dst < end && sscanf(src, "%2x", &u) == 1)
    {
        *dst++ = u;
        src += 2;
    }

    return buffer_size;
}

void print_buffer(unsigned char *buffer, int buffer_size)
{
    unsigned char *dst = buffer;
    unsigned char *end = buffer + buffer_size;

    for (dst = buffer ; dst < end; dst++)
         printf("%d: %c (%d, 0x%02x)\n", dst - buffer,
           (isprint(*dst) ? *dst : '.'), *dst, *dst);
}