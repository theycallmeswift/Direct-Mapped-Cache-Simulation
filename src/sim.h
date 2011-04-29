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

#define CACHE_SIZE 16384
#define BLOCK_SIZE 4


#endif
/* SWIFT_SIM_H_ */
