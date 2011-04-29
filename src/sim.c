/* File: sim.c
 *
 * Author: Mike Swift <theycallmeswift@gmail.com>
 * Date Created: April 28th, 2011
 * Date Modified: April 28th, 2011
 * 
 * This is a program that simulates a cache using a trace file 
 * and either a write through or write back policy.
 * 
 * Usage: Usage: ./sim [-h] <write policy> <trace file>
 *
 * <write policy> is one of:
 *      wt - simulate a write through cache.
 *      wb - simulate a write back cache
 *
 * <trace file> is the name of a file that contains a memory access trace.
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "sim.h"

/* Global Variables */
int writePolicy; /* Write Through = 0, Write Back = 1 */
int numLines; /* Number of lines in the cache */


int main(int argc, char **argv)
{
    /* Help Menu
     *
     * If the help flag is present or there are fewer than
     * three arguments, print the usage menu and return. 
     */
     
    if(argc < 3 || strcmp(argv[1], "-h") == 0)
    {
        fprintf(stderr, 
        "Usage: ./sim [-h] <write policy> <trace file>\n\n<write policy> is one of: \n\twt - simulate a write through cache. \n\twb - simulate a write back cache \n\n<trace file> is the name of a file that contains a memory access trace.\n");
        return 0;
    }
    
    /* Write Policy */
    if(strcmp(argv[1], "wt") == 0)
    {
        writePolicy = 0;
        printf("Write Policy: Write Through\n");
    }
    else if(strcmp(argv[1], "wb") == 0)
    {
        writePolicy = 1;
        printf("Write Policy: Write Back\n");
    }
    else
    {
        fprintf(stderr, "Invalid Write Policy.\nUsage: ./sim [-h] <write policy> <trace file>\n");
        return 0;
    }
    
    /* Calculate numLines */
    numLines = (int)(CACHE_SIZE / BLOCK_SIZE);
    
    printf("Cache Lines: %i\n", numLines);

    return 1;
}

