#include <stdio.h>
#include <stdlib.h>
#include "pgmCheckingValue.h"
#include "pgmErrors.h"
#include "pgmRead.h"
#include "pgmWrite.h"

int main(int argc, char **argv)
	{
	// Check if correct number of arguments were provided
	int code = checkArgsErrors(argc, argv[0]);
	if (code!=999){
	// If not, terminate the program
  return code;}
	// Open the input file

	FileInfo *fileValues = initialisationOfReadFileStruct();
	FILE *inputFile = fopen(argv[1], "r");
	// Read to file and check whether it has read successfully
	code = readFile(argv[1],fileValues, inputFile);
	if (code!=0){
		// If not, terminate with error code
		return code;}
	long nImageBytes = fileValues->width * fileValues->height * sizeof(unsigned char);
	// Read image data based on the magicNumber
	if (*(fileValues->magic_Number) == MAGIC_NUMBER_ASCII_PGM){
		code = readImageData(fileValues, nImageBytes, inputFile, argv[1]);}
	else if (*(fileValues->magic_Number) == MAGIC_NUMBER_RAW_PGM){
		code = readImageDataBinary(fileValues, nImageBytes, inputFile, argv[1]);}
	// Check if successfully written imageData
	if (code !=0){
		return code;}
	closeTheFile(inputFile);
	// Open output file

	FILE *outputFile = fopen(argv[2], "w");
	// Write to file the content of input file
	code = writeToFile(outputFile, fileValues, argv[2], nImageBytes);
	// Check if it has written successfully
	if (code!=0){
		return code;}
	closeTheFile(outputFile);
	printSuccessString(argv[0]);
	return 0;
	}
