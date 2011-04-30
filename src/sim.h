/* File: sim.h
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
 
#ifndef SWIFT_SIM_H_
#define SWIFT_SIM_H_

/* Constants 
 *
 * Both CACHE_SIZE and BLOCK_SIZE are in bytes. We can calculate the number 
 * of lines in the cache with CACHE_SIZE / BLOCK_SIZE.
 * 
 * Ex. 16kb / 4 Bytes = 4096 Lines
 */

/* Print Debug Messages */
#define DEBUG 0

/* Max Line Length in Trace */
#define LINELENGTH 128

/* Cache Sizes (in bytes) */
#define CACHE_SIZE 16384
#define BLOCK_SIZE 4

/* Block Sizes */
#define TAG 18 /* 18 + 0 = 18 */
#define INDEX 12 /* 18 + 12 = 30 */
#define OFFSET 2 /* 30 + 2 = 32 */


/* Typedefs */
typedef struct Cache_* Cache;
typedef struct Block_* Block;


/* createCache
 *
 * Function to create a new cache struct.  Returns the new struct on success
 * and NULL on failure.
 *
 * @param   cache_size      size of cache in bytes
 * @param   block_size      size of each block in bytes
 * @param   write_policy    0 = write through, 1 = write back
 *
 * @return  success         new Cache
 * @return  failure         NULL
 */
 
Cache createCache(int cache_size, int block_size, int write_policy);

/* destroyCache
 * 
 * Function that destroys a created cache. Frees all allocated memory. If 
 * you pass in NULL, nothing happens. So make sure to set your cache = NULL
 * after you destroy it to prevent a double free.
 *
 * @param   cache           cache object to be destroyed
 *
 * @return  void
 */
 
void destroyCache(Cache cache);

/* readFromCache
 *
 * Function that reads data from a cache. Returns 0 on failure
 * or 1 on success. 
 *
 * @param       cache       target cache struct
 * @param       address     hexidecimal address
 *
 * @return      success     1
 * @return      failure     0
 */

int readFromCache(Cache cache, char* address);

/* writeToCache
 *
 * Function that writes data to the cache. Returns 0 on failure or
 * 1 on success. Frees any old tags that already existed in the
 * target slot.
 *
 * @param       cache       target cache struct
 * @param       address     hexidecimal address
 *
 * @return      success     1
 * @return      error       0
 */

int writeToCache(Cache cache, char* address);

/* printCache
 *
 * Prints out the values of each slot in the cache
 * as well as the hit, miss, read, write, and size
 * data.
 *
 * @param       cache       Cache struct
 *
 * @return      void
 */

void printCache(Cache cache);


#endif
/* SWIFT_SIM_H_ */
