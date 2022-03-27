# include <stdio.h>
# include <cmath> 
# include <stdlib.h>
# include <string>
using namespace std;

const int count = 1000000;
int binaryArr2[7][count];  // row: i-th bit, column: j-th time

void generateData(char *fileName, int a, int b, int count=1000000) {
	FILE *fp = fopen(fileName, "w");
	printf("Writing integer to file...\n");
	
	while (count--) {
		int num = a + rand()%(b - a + 1);  // [a, b] 
		fprintf(fp, "%d ", num);
	} 
	fclose(fp);
	printf("Writing finished! \n");
	return;
}

void binary2file(char *fileName, int binaryArr[], int len) {
	FILE *fp = fopen(fileName, "w");
	for (int j = 0; j < len; j++) {
		fprintf(fp, "%d ", binaryArr[j]);
	}
	fclose(fp);
	printf("Writing %s finished!\n", fileName);
	return;
}

void int2binaryStream(char *fileName, const int count=1000000) {
	FILE *fp = fopen(fileName, "r");
	if (fp == NULL) {
		printf("No file named %s \n", fileName);
		return;
	}
//	int binaryArr2[7][count];  // row: i-th bit, column: j-th time
	int idx = 0;
	int integer;

	while(!feof(fp)){
		fscanf(fp,"%d ",&integer); 
//		transfer integer to binary
		for (int i = 0; i < 7; i++) {
			int b = integer % 2;
			integer /= 2;
			binaryArr2[i][idx] = b;
		}
		idx++;
	}
	fclose(fp);
	if (idx != count) {
		printf("Error");
	}
	
//	write binary to files
	char biFileName[30];
	for (int i = 0; i < 7; i++) {
		sprintf(biFileName, "Binary_100w_%d.txt", i);
		binary2file(biFileName, binaryArr2[i], count);
	}
	
}


int main(){
	char *fileName = "Integer_100w.txt";
	generateData(fileName, 0, 127, count);
	int2binaryStream(fileName, count);
}
