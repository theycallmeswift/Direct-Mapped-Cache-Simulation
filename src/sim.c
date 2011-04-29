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
#include <ctype.h>
#include "sim.h"

/* Structs */

struct Cache_ {
    int hits;
    int misses;
    int reads;
    int writes;
    int cache_size;
    int block_size;
    int numLines;    
};

struct Block_ {
    int valid;
    char* tag;
    char* block;
};


/* htoi
 *
 * Converts hexidecimal memory locations to unsigned integers.
 * No real error checking is performed. This function will skip
 * over any non recognized characters.
 */
 
unsigned int htoi(const char str[])
{
    /* Local Variables */
    unsigned int result;
    int i;

    i = 0;
    result = 0;
    
    if(str[i] == '0' && str[i+1] == 'x')
    {
        i = i + 2;
    }

    while(str[i] != '\0')
    {
        result = result * 16;
        if(str[i] >= '0' && str[i] <= '9')
        {
            result = result + (str[i] - '0');
        }
        else if(tolower(str[i]) >= 'a' && tolower(str[i]) <= 'f')
        {
            result = result + (tolower(str[i]) - 'a') + 10;
        }
        i++;
    }

    return result;
}

/* getBinary
 *
 * Converts an unsigned integer into a string containing it's
 * 32 bit binary representation separated by two spaces for 
 * easier parsing. 
 *
 * Format:
 *  -----------------------------------------------------
 * | Tag: 18 bits | Index: 12 bits | Byte Select: 4 bits |
 *  -----------------------------------------------------
 *
 * @param   num         number to be converted
 *
 * @result  char*       binary string separated by spaces.
 */
 
char *getBinary(unsigned int num)
{
    char* bstring;
    int i;
    
    /* Calculate the Binary String */
    
    bstring = (char*) malloc(sizeof(char) * 33);
    assert(bstring != NULL);
    
    bstring[32] = '\0';
    
    for( i = 0; i < 32; i++ )
    {
        bstring[32 - 1 - i] = (num == ((1 << i) | num)) ? '1' : '0';
    }
    
    return bstring;
}

char *formatBinary(char *bstring)
{
    char *formatted;
    int i;
    
    /* Format for Output */
    
    formatted = (char *) malloc(sizeof(char) * 35);
    assert(formatted != NULL);
    
    formatted[34] = '\0';
    
    for(i = 0; i < TAG; i++)
    {
        formatted[i] = bstring[i];
    }
    
    formatted[TAG] = ' ';
    
    for(i = TAG + 1; i < INDEX + TAG + 1; i++)
    {
        formatted[i] = bstring[i - 1];
    }
    
    formatted[INDEX + TAG + 1] = ' ';
    
    for(i = INDEX + TAG + 2; i < OFFSET + INDEX + TAG + 2; i++)
    {
        formatted[i] = bstring[i - 2];
    }

    return formatted;
}

int bin2dec(char *bin)
{
    int  b, k, m, n;
    int  len, sum;

    sum = 0;
    len = strlen(bin) - 1;

    for(k = 0; k <= len; k++)
    {
        n = (bin[k] - '0'); 
        if ((n > 1) || (n < 0))
        {
            return 0;
        }
        for(b = 1, m = len; m > k; m--)
        {
            b *= 2;
        }
        sum = sum + n * b;
    }
    return(sum);
}


int main(int argc, char **argv)
{
    /* Local Variables */
    int write_policy;
    Cache cache;
    
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
        write_policy = 0;
        if(DEBUG) printf("Write Policy: Write Through\n");
    }
    else if(strcmp(argv[1], "wb") == 0)
    {
        write_policy = 1;
        if(DEBUG) printf("Write Policy: Write Back\n");
    }
    else
    {
        fprintf(stderr, "Invalid Write Policy.\nUsage: ./sim [-h] <write policy> <trace file>\n");
        return 0;
    }

    cache = createCache(CACHE_SIZE, BLOCK_SIZE, write_policy);
    
    printCache(cache);

    return 1;
}

Cache createCache(int cache_size, int block_size, int write_policy)
{
    /* Local Variables */
    Cache cache;
    int num;
    char *bstring;
    
    /* Validate Inputs */
    if(cache_size <= 0)
    {
        fprintf(stderr, "Cache size must be greater than 0 bytes...\n");
        return NULL;
    }
    
    if(block_size <= 0)
    {
        fprintf(stderr, "Block size must be greater than 0 bytes...\n");
        return NULL;
    }
    
    if(write_policy != 0 && write_policy != 1)
    {
        fprintf(stderr, "Write policy must be either \"Write Through\" or \"Write Back\".\n");
        return NULL;
    }
    
    /* Lets make a cache! */
    cache = (Cache) malloc( sizeof( struct Cache_ ) );
    if(cache == NULL)
    {
        fprintf(stderr, "Could not allocate memory for cache.\n");
        return NULL;
    }
    
    cache->hits = 0;
    cache->misses = 0;
    cache->reads = 0;
    cache->writes = 0;
    
    cache->cache_size = CACHE_SIZE;
    cache->block_size = BLOCK_SIZE;
    
    /* Calculate numLines */
    cache->numLines = (int)(CACHE_SIZE / BLOCK_SIZE);
    
    bstring = getBinary(htoi("0x0023AF7C"));
    num = htoi("0x0023AF7C");
    
    printf("0x0023AF7C to Decimal: %u\n", num);
    printf("%u to Binary: %s\n", num, bstring);
    printf("%s to Decimal: %i\n", bstring, bin2dec(bstring));
 
    
    printf("\n");
    return cache;
}

void destroyCache(Cache cache)
{
    if(cache != NULL)
    {
        free(cache);
    }
    return;
}

void printCache(Cache cache)
{
    if(cache != NULL)
    {
        printf("Cache:\n\tCACHE HITS: %i\n\tCACHE MISSES: %i\n\tMEMORY READS: %i\n\tMEMORY WRITES: %i\n\n\tCACHE SIZE: %i Bytes\n\tBLOCK SIZE: %i Bytes\n\tNUM LINES: %i\n", cache->hits, cache->misses, cache->reads, cache->writes, cache->cache_size, cache->block_size, cache->numLines);
    }
}

