#include <stdio.h>
#include <stdlib.h>

#define IN_OP "intermediate.op"
#define OUT_OP "intermediate.h"

int main()
{
	char buf[64];
	unsigned int count = 0;
	FILE *in = fopen(IN_OP, "r");
	if (in == NULL) {
		perror(IN_OP);
		exit(EXIT_FAILURE);
	}

	FILE *out = fopen(OUT_OP, "w");
	if (out == NULL) {
		fclose(in);
		perror(OUT_OP);
		exit(EXIT_FAILURE);
	}

	fprintf(out, "#ifndef _INTERMEDIATE_H\n");
	fprintf(out, "#define _INTERMEDIATE_H\n");

	do {
		fscanf(in, "%s\n", buf);
		fprintf(out, "#define %s %d\n", buf, count++);
		if (count > 255) {
			fclose(in);
			fclose(out);
			printf("Number of OP must less than 255!\n");
			exit(EXIT_FAILURE);
		}
	} while (!feof(in));

	fprintf(out, "#endif\n");

	fclose(in);
	fclose(out);

	printf("Generating %s successfully\n", OUT_OP);
	return 0;
}
