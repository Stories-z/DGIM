#include <stdio.h>
#include <stdlib.h>

int main()
{
	FILE * fp = fopen("01stream.txt", "w");
	int r = rand()%2;
	fprintf(fp, "%d", r);
	for(int i = 1; i < 1000000; i++)
	{
		int r = rand()%2;
		fprintf(fp, "\n%d", r);
	}
	fclose(fp);
	return 0;
 } 
