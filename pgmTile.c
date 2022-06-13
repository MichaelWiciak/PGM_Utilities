#include <stdio.h>
#include <stdlib.h>
#include "pgmCheckingValue.h"
#include "pgmErrors.h"
#include "pgmRead.h"
#include "pgmWrite.h"
#include <math.h>
#include <unistd.h>

int main(int argc, char **argv){
  // Check argument count supplied is correct
  int code = checkArgsErrorsForReduce(argc, argv[0]);
  //int isASCII=0;
  if (code!=999){
    // If not, terminate with error code
    return code;}
  // Check if a bad template was provided
  if ((!strstr(argv[3], "<row>"))|| (!strstr(argv[3], "<column>"))){
    printf("ERROR: Miscellaneous (Bad template)");
    return EXIT_MISCELLANEOUS;
  }
  // Check if the file name provided has the .pgm extension specified
  if (!strstr(argv[3], ".pgm")){
    printf("ERROR: Miscellaneous (Bad template)");
    return EXIT_MISCELLANEOUS;
  }
  // initialises fileInfo struct with default values
  FileInfo *fileValues = initialisationOfReadFileStruct();
  // Opens input file and reads it
  FILE *inputFile = fopen(argv[1], "r");
  code = readFile(argv[1],fileValues, inputFile);
  // Check if the file was successfully read
  if (code!=0){
    return code;}
  int isASCII=0;
  long nImageBytes = fileValues->width * fileValues->height * sizeof(unsigned char);
  // Reads the imageData based on magicNumber
  if (*(fileValues->magic_Number) == MAGIC_NUMBER_ASCII_PGM){
    code = readImageData(fileValues, nImageBytes, inputFile, argv[1]);
    isASCII=0;}
  else if (*(fileValues->magic_Number) == MAGIC_NUMBER_RAW_PGM){
    code = readImageDataBinary(fileValues, nImageBytes, inputFile, argv[1]);
    isASCII=1;}
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
  div_t output;
  // Calculate the new height and width of reduced file
  output = div(fileValues->width, factor);
  int widthAdjusted = output.quot;
  output = div(fileValues->height, factor);
  int heightAdjusted = output.quot;
  int minRow=0, maxRow=heightAdjusted, minCol=0, maxCol=widthAdjusted;
  // Calculate the names of the tiled images
  char filename[1000];
  int len = strlen(argv[3]);
  char base[100] = {'\0'};
  int exitFlag=0;
  for (int i=0; i < len && exitFlag==0; i++){
    if ((argv[3][i] != '<') || (argv[3][i+1] != 'r')){
      strncat(base, &argv[3][i], 1);
    }
    else {
      exitFlag=1;
    }
  }
  // Write the data into the tiled output image
  for (int i=0; i<factor; i++){
    // calculate the rows and columns for each file
    minCol=0;
    maxCol=widthAdjusted;
    for (int j=0; j<factor; j++){
      // write that range to the filename
      sprintf(filename, "%s%i_%i.pgm", base  , i, j);
      FILE *outputFile = fopen(filename, "w");
      // Check writing right for that file name
      if (access(filename, W_OK) != 0){
        printErrorStringAndDescription(EXIT_BAD_INPUT_FILE, filename);
        return EXIT_BAD_INPUT_FILE;
      }
      // Check if outputFile is not a valid pointer
      if (outputFile == NULL){
        printErrorStringAndDescription(EXIT_BAD_INPUT_FILE, filename);
        free(fileValues->imageData);
        return EXIT_BAD_INPUT_FILE;
      }
      code = writeASCIIPropertyBasedOnParameters(outputFile, fileValues, heightAdjusted, widthAdjusted, fileValues->maxGray, isASCII, filename);
      // Check if tile was written without error
      if (code !=0){
        return code;
      }
      if (isASCII==0){
        code = writeBetweenMaxMin(outputFile, fileValues, filename, nImageBytes, minRow, maxRow, minCol, maxCol);}
      else if (isASCII==1){
        code = writeBetweenMaxMinBinary(outputFile, fileValues, filename, nImageBytes, minRow, maxRow, minCol, maxCol);}
      if (code!=0){
        return code;}
      // update the column
      maxCol+= widthAdjusted;
      minCol+= widthAdjusted;
      closeTheFile(outputFile);}
  // update the height
    minRow+=heightAdjusted;
    maxRow+=heightAdjusted;}
  // Program successfull finished
  printSuccessString(argv[0]);
  return 0;
}
