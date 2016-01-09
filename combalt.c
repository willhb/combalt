/* 	combalt: combine alternating bytes from binary files
*	01-07-2016 William Breidenthal
*	willhb.com
*
*	Notes: Tested on Mac OSX 10.11.2 with Clang 700.1.81
*/

#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>

int main( int argc, char *argv[] ){

	// Variable Declarations

	unsigned char 	inputByte1 = 0,
					inputByte2 = 0,
					outputBytes[2];

	unsigned int 	file1count = 0,
					file2count = 0,
					outputCount = 0,
					moreData1 = 1,
					moreData2 = 1;

	// Get Files

	if( argc == 4) {
		printf("Input 1: %s\n\rInput 2: %s\n\rOutput: %s\n\r\n\r", argv[1], argv[2], argv[3]);
	} else {
		printf("combalt: combine alternating bytes from binary files\n\r");
		printf("Usage: ./combalt in1.bin in2.bin out.bin\n\r");
		printf("Output: File1_Byte0 File2_Byte0 File1_Byte1 ...\n\r");
		exit(0);
	}

	FILE *inputFile1p, *inputFile2p, *outputFilep;

	// Try to open all the files

	inputFile1p = fopen(argv[1], "r");
	inputFile2p = fopen(argv[2], "r");
	outputFilep = fopen(argv[3], "w");

	// Error and exit if any files fail to open

	if(inputFile1p == NULL){
		printf("Error: Cannot open %s\n\r", argv[1]);
		exit(1);
	}

	if(inputFile2p == NULL){
		printf("Error: Cannot open %s\n\r", argv[2]);
		exit(1);
	}

	if(outputFilep == NULL){
		printf("Error: Cannot open %s\n\r", argv[3]);
		exit(1);
	}

	//Process Data
	//Read a byte from each file pointer, then write the pair to the output file
	//If one file is longer than the other any remaining data will be written sequentially

	do {
		if(moreData1 > 0){
			moreData1 = fread(&inputByte1, sizeof(char), 1, inputFile1p);
			file1count += moreData1;
			outputBytes[0] = inputByte1;
		}
		if(moreData2 > 0){
			moreData2 = fread(&inputByte2, sizeof(char), 1, inputFile2p);
			file2count += moreData2;
			outputBytes[moreData1] = inputByte2;
			//moreData1 should be 1 if there was data read from file 1, otherwise it will be
			// 0 and the second file byte will be written.
		}

		//The moreData flags will count if 0,1 or 2 bytes are available to write when combined
		outputCount += fwrite(&outputBytes, sizeof(char), moreData1+moreData2, outputFilep);

	} while(moreData1 > 0 | moreData2 > 0);

	printf("Read %d bytes from %s\n\r", file1count, argv[1]);
	printf("Read %d bytes from %s\n\r", file2count, argv[2]);
	printf("Wrote %d bytes to %s\n\r", outputCount, argv[3]);

	//Close Files
	fclose(inputFile1p);
	fclose(inputFile2p);
	fclose(outputFilep);

	return 0;
}