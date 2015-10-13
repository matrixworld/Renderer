#include <stdio.h>

int main()
{
	FILE *outfile = fopen("Output.txt", "wb");
	fprintf(outfile, "test");
	fclose(outfile);

	FILE *infile = fopen("Output.txt", "rb");
	char string[20] = { 0 };

	if (!infile)
	{
		puts("Error!");
	}
	else {
		fgets(string, 20, infile);

		puts("String From The File:");
		puts(string);
	}
	fclose(infile);
	
	return 0;
}