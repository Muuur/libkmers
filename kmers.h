#ifndef KAMERS
#define KAMERS

#ifdef __cplusplus
#include <cstring>
#include <cmath>
#include <cstdint>
#include <cstdio>
extern "C"
{
#else
#include <string.h>
#include <inttypes.h>
#include <ctype.h>
#include <stdbool.h>
#include <math.h>
#include <stdio.h>
#endif

#define reset_table(ktable) memset(ktable->table, 0, ktable->tsize << 2)
/*
 * Resets a table, fill values with zeroes
 *
 * @param ktable: table_t*
 * 	Kmers table
 * 
 * @returns void
*/

#define save_table(ofh, ktable) fwrite(ktable->table, 4, ktable->tsize, ofh)
/*
 * Save the current table in an opened file in raw binary format
 *
 * @param ofh: FILE*
 * 	output file handle
 * 
 * @param ktable: table_t*
 * 	kmer table
 * 
 * @returns void
*/

#define read_table(ifh, ktable) fread(ktable->table, 4, ktable->tsize, ifh)
/*
 * Read a raw table provided previously by save_table from opened file
 *
 * @param ifh: FILE*
 * 	input file handle
 * 
 * @param ktable: table_t*
 * 	kmer table
 * 
 * @returns: void
*/

typedef union
{
	uint32_t num;
	float dec;
} numdec_t;
/*
 * Union that contains an int and a float in the same address
 * to facilitate conversion
*/

typedef struct
{
	numdec_t *table;
	uint32_t tsize;
	uint64_t gsize;
} table_t;
/*
 * Table structure that contains table itself
 * table size (tsize)
 * Genome size (gsize)
*/


void die(const char *fmt);
/*
 * Exits the program with a message prompted to stderr with error code 1
 *
 * @param fmt
 * 	Buffer to write to stderr before exit
 * 
 * @returns void
*/

bool dropline(FILE *ifh);
/*
 * Drops a full line from stream
 *
 * @param ifh
 * 	Input file handle which the function will drop a line
 * 
 * @returns dropline
 * 	Boolean whether line drop has been succesfull or not
*/

void create_table(FILE *stream, uint32_t k, table_t *table);
/*
 * Creates a kmer table. Stream contains single fasta
 *
 * @param stream
 * 	File stream which the function will read
 * 
 * @param k
 * 	group of nucleotides to form a k-mer
 * 
 * @param table
 * 	kmer table to fill (or update)
 *  If no previous table is provided, should be NULL
*/

uint64_t create_mtable(FILE *stream, uint32_t k, table_t *table);
/*
 * Creates a kmer table. Stream is supposed to be a multi fasta file
 *
 * @param stream
 * 	File stream which the function will read
 * 
 * @param k
 * 	group of nucleotides to form a k-mer
 * 
 * @param table
 * 	kmer table to fill (or update)
*/

uint32_t vir_num(FILE *ifh);
/*
 * Gets virus sequences number from fasta file
 * The file will be search from position 0 and at the end seeked
 * to the original position
 * 
 * @param ifh
 * 	input file handle containing the multi fasta file from virus
 * 
 * @returns vir_num
 * 	Number of fasta sequences
*/

void convert_table(table_t *table);
/*
 * Given a count table of uint32_t, convert them into float
 * The number will be divided into the genome size (gsize) for each element
 * 
 * @param table
 * 	kmer table where the operation will be performed in-place
 *  The function convert the table->table uint32_t values into float
*/

float pdist(table_t *bactable, table_t *virtable, FILE *ofh);
/*
 * Compute signature distance between two tables using kmers
 *
 * @param bactable
 * 	Bacteria table (if the organism is baceria)
 *  Table 1
 * 
 * @param virtable
 * 	Virus table (if the other organism is a virus)
 *  Table 2
 * 
 * @param ofh
 *  Output file stream
 *  The operation will write the opened buffer
 * 
 * @returns pdist
 *  A float representing the signature distance between two organisms given
*/

numdec_t *getkmem(uint32_t size);
/*
 * Auxiliary function to create a hash table to table_t structure
 *
 * @param size
 * 	Table size in units
 *  Then it will convert the units to uint32_t size
 * 
 * @returns getkmem
 *  Mallocated numdec_t pointer to the table
*/

void delete_table(table_t *table);
/*
 * Auxililary function to free a table structure
 * The table provided must have been created using
 * create_table or create_mtable functions
 * 
 * @param table
 *  Table structure pointer to which will be freed
*/

#ifdef __cplusplus
}
#endif

#endif // Kamers
