#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "kmers.h"

char *tmpfilepath = "/tmp/virus.tmp.XXXXXX";

// ? Must compile with -lm option
int main(int argc, const char **argv)
{
	if(argc < 4)
		die("Error: not enough arguments. Usage\n\t./mmain <K> <bacteria_file> <virus_file> [ <output_file> ]\n");
	uint32_t k = atoi(argv[1]);
	printf("Starting with k=%d\n", k);
	FILE *bac = fopen(argv[2], "rb"), *vir = fopen(argv[3], "rb");

	if(bac == NULL || vir == NULL)
		die("Failed to open files");

	uint32_t size = 1 << (k << 1);
	table_t *bactable = NULL, *virtable = NULL;
	if(bactable == NULL || virtable == NULL)
		die("Could not reserve memory\n");
	uint32_t virq = 0;
	char rd = 0;

	if(fgetc(bac) != '>' || fgetc(vir) != '>')
		die("File is not a fasta file\n");

	dropline(bac);
	dropline(vir);

	FILE *ofh = NULL;
	if(argc > 3)
		ofh = fopen(argv[4], "w");
	else
		ofh = fopen("output_bac.csv", "w+");
	if(ofh == NULL)
		die("Cannot open output file\n");
	
	FILE *tmp = fopen(tmpfilepath, "wb");
	if(tmp == NULL)
		die("Could not open tmp file\n");

	fprintf(ofh, "#k=%d;", k);
	while(!feof(vir))
	{
		printf("Creating virus %d table\r", ++virq);
		create_table(vir, k, virtable);
		convert_table(virtable);
		fprintf(ofh, "virus%d-%ld;", virq, virtable->gsize);
		save_table(tmp, virtable);
		reset_table(virtable);
		dropline(vir);
	}
	putchar('\n');
	fseek(ofh, -1, SEEK_CUR);
	fputc('\n', ofh);
	fclose(tmp);
	tmp = fopen(tmpfilepath, "rb");

	uint32_t numbac = 1;
	while(!feof(bac))
	{
		printf("Creating bacteria table %d\n", numbac);
		create_table(bac, k, bactable);
		convert_table(bactable);

		fseek(tmp, 0, SEEK_SET);
		fprintf(ofh, "Bacteria%d;", numbac);

		for(uint32_t i = 0; i < virq; i++)
		{
			printf("Comparing bacteria %d to virus %d\r", numbac, i + 1);
			read_table(tmp, virtable);
			fprintf(ofh, "%f%c", pdist(bactable, virtable, ofh), i == virq - 1 ? '\n' : ';');
		}

		putchar('\n');
		dropline(bac);
		reset_table(bactable);
		numbac++;
	}
	printf("Finished file %s!!\n", argc > 3 ? argv[4] : "output_bac.csv");

	delete_table(bactable);
	delete_table(virtable);
	fclose(ofh);
	fclose(tmp);
	fclose(bac);
	fclose(vir);
	remove(tmpfilepath);
	return 0;
}

