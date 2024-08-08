#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <inttypes.h>
#include <ctype.h>
#include <stdbool.h>
#include <math.h>

typedef union
{
	uint32_t num;
	float dec;
} numdec_t;

typedef struct
{
	numdec_t *table;
	uint32_t tsize;
	uint64_t gsize;
} table_t;

void die(const char *fmt)
{
	fputs(fmt, stderr);
	exit(1);
}

bool dropline(FILE *ifh)
{
	while(fgetc(ifh) != '\n' && !feof(ifh));
	return feof(ifh);
}

numdec_t *getkmem(uint32_t size)
{
	size <<= 2;
	numdec_t *res = malloc(size);
	memset(res, 0, size);
	return res;
}

void delete_table(table_t *table)
{
	free(table->table);
	free(table);
}

void create_table(FILE *stream, uint32_t k, table_t *table)
{
	uint64_t hash = 0, kdesp = k << 1;
	char rd = 0;
	uint32_t pos = 0;
	kdesp -= 2;
	kdesp = 1 << kdesp;
	kdesp--;
	if(table == NULL)
	{
		table = malloc(sizeof(table_t));
		table->table = getkmem(kdesp),
		table->tsize = kdesp,
		table->gsize = 0;
	}

	while(true)
	{
		rd = toupper(fgetc(stream));
		if(feof(stream))
			break;
		switch(rd)
		{
		case '>':
			fseek(stream, -1, SEEK_CUR);
			return;
		case 'A':
			table->gsize++;
			pos++;
			hash <<= 2;
			break;
		case 'C':
			table->gsize++;
			pos++;
			hash <<= 2;
			hash++;
			break;
		case 'G':
			table->gsize++;
			pos++;
			hash <<= 2;
			hash += 2;
			break;
		case 'T':
			table->gsize++;
			pos++;
			hash <<= 2;
			hash += 3;
			break;
		case '\n':
			break;
		default:
			table->gsize++;
			pos = 0;
			hash = 0;
			break;
		}
		if(pos == k)
		{
			table->table[hash].num++;
			hash &= kdesp;
			pos--;
		}
	}
}

void create_mtable(FILE *stream, uint32_t k, table_t *table)
{
	while(!feof(stream))
	{
		create_table(stream, k, table);
		dropline(stream);
	}
}

uint32_t vir_num(FILE *ifh)
{
	uint32_t res = 0, pos = ftell(ifh);
	fseek(ifh, 0, SEEK_SET);
	char bf = 0;
	while(!feof(ifh))
	{
		bf = fgetc(ifh);
		switch(bf)
		{
		case '>':
			res++;
			break;
		case '\n':
			break;
		default:
			dropline(ifh);
			break;
		}
	}
	fseek(ifh, pos, SEEK_SET);
	return res;
}

void convert_table(table_t *table)
{
	float tmp;
	for(uint64_t i = 0; i < table->tsize; i++)
	{
		tmp = (float) table->table[i].num;
		table->table[i].dec = tmp / (float) table->gsize;
	}
}

float pdist(table_t *bactable, table_t *virtable, FILE* ofh)
{
	float res = 0, tmp = 0;
	for(uint32_t i = 0; i < virtable->tsize; i++)
	{
		tmp = bactable->table[i].dec - virtable->table[i].dec;
		tmp *= tmp;
		res += tmp;
	}
	return sqrt(res);
}
