/*
 **********************************************************************
 ** md5driver.c -- sample routines to test                           **
 ** RSA Data Security, Inc. MD5 message digest algorithm.            **
 ** Created: 2/16/90 RLR                                             **
 ** Updated: 1/91 SRD                                                **
 **********************************************************************
 */

/*
 **********************************************************************
 ** Copyright (C) 1990, RSA Data Security, Inc. All rights reserved. **
 **                                                                  **
 ** RSA Data Security, Inc. makes no representations concerning      **
 ** either the merchantability of this software or the suitability   **
 ** of this software for any particular purpose.  It is provided "as **
 ** is" without express or implied warranty of any kind.             **
 **                                                                  **
 ** These notices must be retained in any copies of any part of this **
 ** documentation and/or software.                                   **
 **********************************************************************
 */

#include <stdio.h>
#include <sys/types.h>
#include <time.h>
#include <string.h>

#include "md5.h"

/* Prints message digest buffer in ctx as 32 hexadecimal digits.
   Order is from low-order byte to high-order byte of digest.
   Each byte is printed with high-order hexadecimal digit first.
 */
static void md5_print(md5_ctx *ctx)
{
    int i;

    for (i = 0; i < 16; i++) {
        printf ("%02x", ctx->digest[i]);
    }
}

/* size of test block */
#define TEST_BLOCK_SIZE 1000

/* number of blocks to process */
#define TEST_BLOCKS 10000

/* number of test bytes = TEST_BLOCK_SIZE * TEST_BLOCKS */
static long TEST_BYTES = (long)TEST_BLOCK_SIZE * (long)TEST_BLOCKS;

/* A time trial routine, to measure the speed of MD5.
   Measures wall time required to digest TEST_BLOCKS * TEST_BLOCK_SIZE
   characters.
 */
static void md5_time_trial(void)
{
    md5_ctx ctx;
    time_t end_time, start_time;
    uint8_t data[TEST_BLOCK_SIZE];
    uint32_t i;

    /* initialize test data */
    for (i = 0; i < TEST_BLOCK_SIZE; i++) {
        data[i] = (uint8_t)(i & 0xFF);
    }

    /* start timer */
    printf ("MD5 time trial. Processing %ld characters...\n", TEST_BYTES);
    start_time = clock();

    /* digest data in TEST_BLOCK_SIZE byte blocks */
    md5_init(&ctx);
    for (i = TEST_BLOCKS; i > 0; i--) {
        md5_update(&ctx, data, TEST_BLOCK_SIZE);
    }
    md5_final(&ctx);

    /* stop timer, get time difference */
    end_time = clock();
    md5_print(&ctx);
    printf(" is digest of test input.\n");
    printf("Seconds to process test input: %ld ms\n", (long)(end_time - start_time));
    if (end_time - start_time) {
        printf("Characters processed per millisecond: %ld\n", TEST_BYTES / (end_time - start_time));
    }
}

/* Computes the message digest for string in_string.
   Prints out message digest, a space, the string (in quotes) and a
   carriage return.
 */
static void md5_string(char *in_string)
{
    md5_ctx ctx;
    uint32_t len = strlen(in_string);

    md5_init(&ctx);
    md5_update(&ctx, in_string, len);
    md5_final(&ctx);
    md5_print(&ctx);
    printf (" \"%s\"\n\n", in_string);
}

/* Computes the message digest for a specified file.
   Prints out message digest, a space, the file name, and a carriage
   return.
 */
static void md5_file(char *filename)
{
    FILE *in_file = fopen(filename, "rb");
    md5_ctx ctx;
    int bytes;
    uint8_t data[1024];

    if (in_file == NULL) {
        printf ("%s can't be opened.\n", filename);
        return;
    }

    md5_init(&ctx);
    while ((bytes = fread(data, 1, 1024, in_file)) != 0) {
        md5_update(&ctx, data, bytes);
    }
    md5_final(&ctx);
    md5_print(&ctx);
    printf(" %s\n", filename);
    fclose(in_file);
}

/* Writes the message digest of the data from stdin onto stdout,
   followed by a carriage return.
 */
static void md5_filter(void)
{
    md5_ctx ctx;
    int bytes;
    uint8_t data[16];

    md5_init(&ctx);
    while ((bytes = fread(data, 1, 16, stdin)) != 0) {
        md5_update(&ctx, data, bytes);
    }
    md5_final(&ctx);
    md5_print(&ctx);
    printf ("\n");
}

/* Runs a standard suite of test data.
 */
static void md5_test_suite(void)
{
    printf ("MD5 test suite results:\n\n");
    md5_string("");
    md5_string("a");
    md5_string("abc");
    md5_string("message digest");
    md5_string("abcdefghijklmnopqrstuvwxyz");
    md5_string("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789");
    md5_string("12345678901234567890123456789012345678901234567890123456789012345678901234567890");
    /* Contents of file foo are "abc" */
    md5_file("foo.txt");
}

int main_md5(int argc,char *argv[]){
    int i;
    /* For each command line argument in turn:
    ** filename          -- prints message digest and name of file
    ** -sstring          -- prints message digest and contents of string
    ** -t                -- prints time trial statistics for 1M characters
    ** -x                -- execute a standard suite of test data
    ** (no args)         -- writes messages digest of stdin onto stdout
    */
    if (argc == 1) {
        md5_filter();
    } else {
        for (i = 1; i < argc; i++) {
            if (argv[i][0] == '-' && argv[i][1] == 's') {
                md5_string(argv[i] + 2);
            } else if (strcmp (argv[i], "-t") == 0) {
                md5_time_trial();
            } else if (strcmp (argv[i], "-x") == 0) {
                md5_test_suite();
            } else {
                md5_file (argv[i]);
            }
        }
    }

    return 0;
}

/*
 **********************************************************************
 ** End of md5driver.c                                               **
 ******************************* (cut) ********************************
 */

void md5_cal(char *in_string, char *outString ) {
	md5_ctx ctx;
	uint32_t len = strlen(in_string), i;

	md5_init(&ctx);
	md5_update(&ctx, (uint8_t*)in_string, len);
	md5_final(&ctx);

	for (i = 0; i < 16; i++) {
        sprintf((outString+(2*i)), "%02x", ctx.digest[i]);
    }
	outString[32] = '\0' ;
}

void md5_cal_file(char * filename, char * outString ) {
    FILE *in_file = NULL ;
    md5_ctx ctx;
    int bytes,i;
    uint8_t data[1024];

    if (!strcmp(filename,"-")){
        in_file = stdin;
    } else {
        in_file = fopen (filename, "rb");
    }

    if (in_file == NULL){
        printf ("%s can't be opened.\n", filename);
        return;
    }

    md5_init(&ctx);
    while ((bytes = fread (data, 1, 1024, in_file)) != 0) {
        md5_update(&ctx, data, bytes);
    }
    md5_final(&ctx);
    for (i = 0; i < 16; i++) {
        sprintf((outString+(2*i)), "%02x", ctx.digest[i]);
    }
    outString[32] = '\0' ;
    if (in_file!=stdin) {
        fclose(in_file);
    }
}

#define MAIN
#ifdef MAIN
int main(int argc, char *argv[], char *arge[]) {
	return main_md5(argc, argv);
}
#endif