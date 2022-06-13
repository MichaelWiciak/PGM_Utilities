#include <stdio.h>
#include <stdlib.h>
#include "pgmCheckingValue.h"
#include "pgmErrors.h"
#include "pgmRead.h"
#include "pgmWrite.h"
#include <math.h>

int main(int argc, char **argv){
  // Check argument count supplied is correct
  int code = checkArgsErrorsForReduce(argc, argv[0]);
  int isASCII=0;
  if (code!=999){
    // If not, terminate with error code
    return code;}
  // initialises fileInfo struct with default values
  FileInfo *fileValues = initialisationOfReadFileStruct();
  // Opens input file and reads it
  FILE *inputFile = fopen(argv[1], "r");
  code = readFile(argv[1],fileValues, inputFile);
  // Check if the file was successfully read
  if (code!=0){
    return code;}
  long nImageBytes = fileValues->width * fileValues->height * sizeof(unsigned char);
  // Reads the imageData based on magicNumber
  if (*(fileValues->magic_Number) == MAGIC_NUMBER_ASCII_PGM){
  code = readImageData(fileValues, nImageBytes, inputFile, argv[1]);
  isASCII=0;}
  else if (*(fileValues->magic_Number) == MAGIC_NUMBER_RAW_PGM){
    code = readImageDataBinary(fileValues, nImageBytes, inputFile, argv[1]);
    isASCII=0;}
  // Checks if the data was read successfully
  if (code !=0){
    return code;}
  closeTheFile(inputFile);
  // Checks if the factor provided was an integer and terminate program if not
  code = checkIfinteger(argv[2]);
  if (code!=0){
    return code;}
  // Converts factor string to integer
  unsigned int factor = atoi(argv[2]);
  // Open file for writing
  FILE *outputFile = fopen(argv[3], "w");
  div_t output;
  // Calculate the new height and width of reduced file
  output = div(fileValues->width, factor);
  int width = output.quot;
  output = div(fileValues->height, factor);
  int height = output.quot;
  // Write the ASCII header with modified values
  writeASCIIPropertyBasedOnParameters(outputFile, fileValues, height, width, fileValues->maxGray, isASCII, argv[3]);
  // Check if written successfully
  if (code!=0){
    return code;}
  // Reduce the file and write to it based on the magicNumber
  if (isASCII==0){
    code = writeReducedASCII(outputFile, fileValues, argv[3], nImageBytes, factor, width);}
  else if (isASCII==1){
    code = writeASCIIDataInBinaryReduced(outputFile, fileValues, argv[3], nImageBytes, factor);}
  //Check if successfully writteen to file
  if (code!=0){
    return code;}
  closeTheFile(outputFile);
  printSuccessString(argv[0]);
  return 0;
}
