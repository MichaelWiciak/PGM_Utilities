#include <stdio.h>
#include <stdlib.h>
#include "pgmCheckingValue.h"
#include "pgmErrors.h"
#include "pgmRead.h"
#include "pgmWrite.h"

int main(int argc, char **argv){
  // Check if the correct number of argument are provided
  int code = checkArgsErrors(argc, argv[0]);
  if (code!=999){
    return code;}
  // initialises fileInfo struct with default values
  FileInfo *fileValues = initialisationOfReadFileStruct();
  // Read a input file
	FILE *inputFile = fopen(argv[1], "r");
	code = readFile(argv[1],fileValues, inputFile);
  // Check if successfully read
	if (code!=0){
		return code;}
  // Check if magicNumber indicates the input file is a ASCII file
  // and switch the magicNumber to binary
  code = checkIfMagicNumberInASCIIandSwitchToBinary(fileValues, argv[1]);
  if (code!=0){
    return code;}
  // Allocates max image size
	long nImageBytes = fileValues->width * fileValues->height * sizeof(unsigned char);
  //Read the image data
  code = readImageData(fileValues, nImageBytes, inputFile, argv[1]);
  // Check if written successfully to file
	if (code !=0){
		return code;}
  closeTheFile(inputFile);
  // Open the output file
  FILE *outputFile = fopen(argv[2], "w");
  // Write the ASCII header in ASCII to output file
  code = writeASCIIPropertyONLY(outputFile, fileValues, argv[2], nImageBytes);
  // Check if successfully written
  if (code!=0){
    return code;}
  // Write the content of the image in binary to the output file
  code = writeASCIIDataInBinary(outputFile, fileValues, argv[2], nImageBytes);
  // Check if successfully written
  if (code!=0){
    return code;}
  closeTheFile(outputFile);
  printSuccessString(argv[0]);
  return 0;
}
