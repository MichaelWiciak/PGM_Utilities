#include <stdio.h>
#include <stdlib.h>
#include "pgmCheckingValue.h"
#include "pgmErrors.h"
#include "pgmRead.h"
#include "pgmWrite.h"

int main(int argc, char **argv){
  // Check if argument number error occured
  int code = checkArgsErrors(argc, argv[0]);
  if (code!=999){
    // If yes, exit the program with error code
    return code;}
  // Creates fileInfo struct and sets it to default values
  FileInfo *fileValues1 = initialisationOfReadFileStruct();
  // Open the input file
	FILE *inputFile1 = fopen(argv[1], "r");
	code = readFile(argv[1],fileValues1, inputFile1);
  // Check the file has been read properly
	if (code!=0){
		return code;}
  // Calculates the size of the file
	long nImageBytes1 = fileValues1->width * fileValues1->height * sizeof(unsigned char);
  // Read the file according to whether the file is in ASCII or binary format
  if (*(fileValues1->magic_Number) == MAGIC_NUMBER_ASCII_PGM){
    code = readImageData(fileValues1, nImageBytes1, inputFile1, argv[1]);}
  else if (*(fileValues1->magic_Number) == MAGIC_NUMBER_RAW_PGM){
    code = readImageDataBinary(fileValues1, nImageBytes1, inputFile1, argv[1]);}
  // Check if the data could be read
	if (code !=0){
		return code;}
	closeTheFile(inputFile1);
  // Read file 2, same everything as above
  FileInfo *fileValues2 = initialisationOfReadFileStruct();
  FILE *inputFile2 = fopen(argv[2], "r");
  code = readFile(argv[2],fileValues2, inputFile2);
  if (code!=0){
    return code;}
  long nImageBytes2 = fileValues2->width * fileValues2->height * sizeof(unsigned char);
  if (*(fileValues2->magic_Number) == MAGIC_NUMBER_ASCII_PGM){
  code = readImageData(fileValues2, nImageBytes2, inputFile2, argv[2]);}
  else if (*(fileValues2->magic_Number) == MAGIC_NUMBER_RAW_PGM){
    code = readImageDataBinary(fileValues2, nImageBytes2, inputFile2, argv[2]);}
  if (code !=0){
    return code;}
  closeTheFile(inputFile2);
  // Check if the two files are logically equivalent
  int count = checkIfTwoPGMFilesAreLogicallyIdentical(fileValues1, fileValues2);
  // Display the result
  printPGMCompOutput(count);
  return 0;
}
