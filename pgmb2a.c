// Modules pgmb2a need for execution
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
  FileInfo *fileValues = initialisationOfReadFileStruct();
  // Open the input file
	FILE *inputFile = fopen(argv[1], "rb");
  // Try to read the file ASCII properties. If error occurs, terminate the program
	code = readFile(argv[1],fileValues, inputFile);
	if (code!=0){
		return code;}
  // Check if magicNumber is in ASCII or else, raise error
  // If magicNumber is in binary, switch it to ASCII
  code = checkIfMagicNumberInBinaryandSwitchToASCII(fileValues, argv[1]);
  if (code!=0){
    return code;}
  // Calculates the size of the file
	long nImageBytes = fileValues->width * fileValues->height * sizeof(unsigned char);
  // Read the imageData as binary, if error occurs, terminate the program
  code = readImageDataBinary(fileValues, nImageBytes, inputFile, argv[1]);
	if (code !=0){
		return code;}
  closeTheFile(inputFile);
  // Open output file
  FILE *outputFile = fopen(argv[2], "w");
  code = writeToFile(outputFile, fileValues, argv[2], nImageBytes);
  if (code!=0){
    return code;}
  closeTheFile(outputFile);
  printSuccessString(argv[0]);
  // Program worked correctly, exit with error code 0;
  return 0;
}
