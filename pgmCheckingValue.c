// Include the header file for this program
#include "pgmCheckingValue.h"
#include <math.h>

int checkArgs (int argCount, int targetCount){
  // If argCount = targetCount, return 1, else 0
  if (argCount != targetCount){
    return 1;
  }
  return 0;
}

int checkArgsErrors(int argc, char* filename){
  // Check if a usage string should be printed
  if (checkArgcUsageString(argc, filename)==0){
    return EXIT_NO_ERRORS;}
  // Check if three arguments were not supplied
  if (checkArgs(argc, 3)==1){
    // Invalid number of arguments, has to be 3
    printErrorStringAndDescription(EXIT_WRONG_ARG_COUNT, "");
    return EXIT_WRONG_ARG_COUNT;}
  // Since EXIT_NO_ERRORS is used by the Usage String exit and the file has to exit if usage string is printed
  // a different error code is returned
  return 999;
}

int checkIfTwoPGMFilesAreLogicallyIdentical(FileInfo *fileValues1, FileInfo *fileValues2){
  // Checks if the width, height of both files are identical
  if ((fileValues1->width) != (fileValues2->width) || (fileValues1->height) != (fileValues2->height)){
    return 1;}
  // Take every imageData for each file and compare them
  // If they differ, the files are different
  long double adjustedValue1, adjustedValue2;
  long max = fileValues1->width * fileValues1->height * sizeof(unsigned char);
  for (int i=0; i<max; i++){
    // Adjust the imageData to be a factor of its value over the maximum value
    adjustedValue1 = (long double)*(fileValues1->imageData+i)/fileValues1->maxGray;
    adjustedValue2 = (long double)*(fileValues2->imageData+i)/fileValues2->maxGray;
    if (adjustedValue1!=adjustedValue2){
      return 1;
    }
  }
  return 0;
}

int checkArgcUsageString(int argc, char* filename){
  if (checkArgs(argc, 1)==0){
    // If so, print usage string for that file
    printUsageString(filename);
    // and return error code 0 so no error as stated in the specification
    return EXIT_NO_ERRORS;}
  return 999;
}
int checkArgsErrorsForReduce(int argc, char* filename){
  // Check if usage string should be printed
  if (checkArgcUsageString(argc, filename)==0){
    return EXIT_NO_ERRORS;}
  // Check if there are four arguments
  if (checkArgs(argc, 4)==1){
    // If not, print error description and return error code
    printErrorStringAndDescription(EXIT_WRONG_ARG_COUNT, "");
    return EXIT_WRONG_ARG_COUNT;}
  return 999;
}

int checkArgsErrorsForAssemble(int argc, char* filename){
  if (checkArgcUsageString(argc, filename)==0){
    return EXIT_NO_ERRORS;}
  if (checkArgs(argc, 4)==0){
    // If not, print error description and return error code
    printErrorStringAndDescription(EXIT_WRONG_ARG_COUNT, "");
    return EXIT_WRONG_ARG_COUNT;}
  if (argc%3 != 1){
    printErrorStringAndDescription(EXIT_WRONG_ARG_COUNT, "");
    return EXIT_WRONG_ARG_COUNT;
  }
  return 999;
}

int checkIfinteger(char* string){
  // Tries to convert a string to integer
  // Will return error if string provided cannot be converted or is less than or equal to 0
  int num = atoi(string);
  if (num==0||num<0){
    printErrorStringAndDescription(EXIT_MISCELLANEOUS, "Reduce factor is either not an integer or is less than 1");
    return EXIT_MISCELLANEOUS;
  }
  return EXIT_NO_ERRORS;
}

int checkIfMagicNumberInBinaryandSwitchToASCII(FileInfo *fileValues, char* filename){
  // Check if magicNumber is in Binary
  if (*(fileValues->magic_Number) == MAGIC_NUMBER_RAW_PGM){
    *(fileValues->magic_Number) = MAGIC_NUMBER_ASCII_PGM;
    return EXIT_NO_ERRORS;}
    // Incorrect file was provided, terminate with an error
  printErrorStringAndDescription(EXIT_BAD_MAGIC_NUMBER, filename);
  return EXIT_BAD_MAGIC_NUMBER;
}

int checkIfMagicNumberInASCIIandSwitchToBinary(FileInfo *fileValues, char* filename){
  // Check if magicNumber is in ASCII
  if (*(fileValues->magic_Number) == MAGIC_NUMBER_ASCII_PGM){
    *(fileValues->magic_Number) = MAGIC_NUMBER_RAW_PGM;
    return EXIT_NO_ERRORS;}
    // Incorrect file was provided, terminate with an error
  printErrorStringAndDescription(EXIT_BAD_MAGIC_NUMBER, filename);
  return EXIT_BAD_MAGIC_NUMBER;
}
