#include <stdlib.h>
#include <stdio.h>
#include "kmers.h"

// ? Must compile with -lm option
int main(int argc, const char **argv)
{
	if(argc < 4)
		die("error: not enough arguments. Usage\n\t./smain <K> <bacteria_file> <virus_file> [ <output_file> ]\n");
	uint32_t k = atoi(argv[1]);
	printf("Starting with k=%d\n", k);
	FILE *bac = fopen(argv[2], "rb"), *vir = fopen(argv[3], "rb");

	if(bac == NULL || vir == NULL)
		die("Failed to open files\n");

	uint64_t size = 1 << (k << 1);
	table_t *bactable = NULL, *virtable = NULL;
	char rd = 0;

	if(fgetc(bac) != '>' || fgetc(vir) != '>')
		die("File is not a fasta file\n");

	dropline(bac);
	dropline(vir);
	uint32_t virq = vir_num(vir);

	FILE *ofh = NULL;
	if(argc > 3)
		ofh = fopen(argv[4], "w");
	else
		ofh = fopen("output_bac.csv", "w");
	if(ofh == NULL)
		die("Cannot open output file\n");
	
	puts("Creating virus table");
	create_mtable(vir, k, virtable);
	convert_table(virtable);

	fprintf(ofh, "#k=%d;virus%d-%ld\n", k, virq, virtable->gsize);

	uint32_t numbac = 1;
	while(!feof(bac))
	{
		printf("Creating bacteria table %d\n", numbac);
		create_table(bac, k, bactable);
		dropline(bac);
		convert_table(bactable);
		fprintf(ofh, "bacteria%d;%f\n", numbac, pdist(bactable, virtable, ofh));
		reset_table(bactable);
		numbac++;
	}

	printf("Finished file %s!!\n", argc > 3 ? argv[4] : "output_bac.csv");
	delete_table(bactable);
	delete_table(virtable);
	fclose(ofh);
	fclose(bac);
	fclose(vir);
	return 0;
}

