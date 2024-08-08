### libkmers

Library to work with k-mers table

#### Dependencies

- gcc

- make

#### Compilation

```bash
git clone https://github.com/Muuur/libkmers
cd libkmers
make
export LD_LIBRARY_PATH="$LD_LIBRARY_PATH:$HOME/.local/lib"
```

#### libkmers.h

- Macros

```c
#define reset_table(ktable) memset(ktable->table, 0, ktable->tsize << 2)
#define save_table(ofh, ktable) fwrite(ktable->table, 4, ktable->tsize, ofh)
#define read_table(ifh, ktable) fread(ktable->table, 4, ktable->tsize, ifh)
```

- Union

```c
typedef union
{
	uint32_t num;
	float dec;
} numdec_t;
```

- Structure

```c
typedef struct
{
	numdec_t *table;
	uint32_t tsize;
	uint64_t gsize;
} table_t;
```

- Functions

```c
void die(const char *fmt);
bool dropline(FILE *ifh);
void create_table(FILE *stream, uint32_t k, table_t *table);
void create_mtable(FILE *stream, uint32_t k, table_t *table);
uint32_t vir_num(FILE *ifh);
void convert_table(table_t *table);
float pdist(table_t *bactable, table_t *virtable, FILE *ofh);
numdec_t *getkmem(uint32_t size);
void delete_table(table_t *table);
```

#### smain

Perform analysis from single-fasta file to a multi-fasta file

#### mmain

Perform analysis from multi-fasta file to another multi-fasta file
