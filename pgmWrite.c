#include "pgmWrite.h"
#include <stdlib.h>
#include <stdio.h>
#include "pgmCheckingValue.h"
#include "pgmErrors.h"

int writeToFile(FILE *outputFile, FileInfo *fileValues, char* outputFileName, long nImageBytes){
  int code=0;

  // Check if file is empty
  if (access(outputFileName, W_OK) == -1){
		printErrorStringAndDescription(EXIT_BAD_INPUT_FILE, outputFileName);
		return EXIT_BAD_INPUT_FILE;
	}

  code = fileEmptyErrorCheck(outputFile, fileValues, outputFileName);
  if (code != EXIT_NO_ERRORS){
    // Terminate with error code
    return code;}

  int isBinary=0;
  size_t nBytesWritten;
  // Write the ASCII header based on magicNumber
  if (*(fileValues->magic_Number) == MAGIC_NUMBER_ASCII_PGM){
    nBytesWritten = fprintf(outputFile, "P2\n%d %d\n%d\n", fileValues->width, fileValues->height, fileValues->maxGray);
    isBinary=1;}
  else if (*(fileValues->magic_Number) == MAGIC_NUMBER_RAW_PGM){
    nBytesWritten = fprintf(outputFile, "P5\n%d %d\n%d\n", fileValues->width, fileValues->height, fileValues->maxGray);
    isBinary=2;}
  // Check if the file is in binary and was able to write the header, and write to it the imageData if yes
  if ((isBinary==2) && (nBytesWritten >= 0)){

    nBytesWritten = fwrite(fileValues->imageData, sizeof(unsigned char), fileValues->width * fileValues->height, outputFile);

    if (nBytesWritten < 0){
      // If not, terminate with error code
      free(fileValues->imageData);
      printErrorStringAndDescription(EXIT_OUTPUT_FAILED, outputFileName);
      return EXIT_OUTPUT_FAILED;}

    return EXIT_NO_ERRORS;}

  else if (isBinary==2){
    // File in binary but wasn't able to write header
    printErrorStringAndDescription(EXIT_BAD_DATA, outputFileName);
    return EXIT_BAD_DATA;}
  // Check if was able to write the header
  if (nBytesWritten < 0){
    // Terminate thr program if not with error code
    free(fileValues->commentLine);
    free(fileValues->imageData);
    printErrorStringAndDescription(EXIT_BAD_DIMENTION, outputFileName);
    return EXIT_BAD_DIMENTION;}
  // Write in ASCII to outputFile
  for (unsigned char *nextGrayValue = fileValues->imageData; nextGrayValue < fileValues->imageData + nImageBytes; nextGrayValue++){
    int nextCol = (nextGrayValue - fileValues->imageData + 1) % fileValues->width;
    nBytesWritten = fprintf(outputFile, "%d%c", *nextGrayValue, (nextCol ? ' ' : '\n') );
    // Check if it was possible to write into file
    if (nBytesWritten < 0){
      // If not, terminate with error code
      free(fileValues->commentLine);
      free(fileValues->imageData);
      printErrorStringAndDescription(EXIT_OUTPUT_FAILED, outputFileName);
      return EXIT_OUTPUT_FAILED;}}
    // Successfully written all to output file
    return EXIT_NO_ERRORS;
}

int writeASCIIDataInBinary(FILE *outputFile, FileInfo *fileValues, char* outputFileName, long nImageBytes){
  // Write the data in binary
  int code=0;
  // Check if file is empty
  code = fileEmptyErrorCheck(outputFile, fileValues, outputFileName);
  if (code != 0){
    // Terminate with error code
    return code;}
    if (access(outputFileName, W_OK) == -1){
  		printErrorStringAndDescription(EXIT_BAD_INPUT_FILE, outputFileName);
  		return EXIT_BAD_INPUT_FILE;
  	}
  for (unsigned char *nextGrayValue = fileValues->imageData; nextGrayValue < fileValues->imageData + nImageBytes; nextGrayValue++){
    // Write one byte at a time in binary
    fwrite(nextGrayValue, 1, 1, outputFile);
  }
  return EXIT_NO_ERRORS;
}

int fileEmptyErrorCheck(FILE *outputFile, FileInfo *fileValues, char* outputFileName){
  // Check if the file provided is NULL
  if (outputFile == NULL)
    {
    // Terminate with error code
    free(fileValues->commentLine);
    free(fileValues->imageData);
    printErrorStringAndDescription(EXIT_OUTPUT_FAILED, outputFileName);
    return EXIT_OUTPUT_FAILED;
    }
  return EXIT_NO_ERRORS;
}

int writeASCIIPropertyONLY(FILE *outputFile, FileInfo *fileValues, char* outputFileName, long nImageBytes){
  // Check writing permissions
  if (access(outputFileName, W_OK) == -1){
    printErrorStringAndDescription(EXIT_BAD_INPUT_FILE, outputFileName);
    return EXIT_BAD_INPUT_FILE;
  }
  int code=0;
  // Check if the output file is empty
  code = fileEmptyErrorCheck(outputFile, fileValues, outputFileName);
  if (code != EXIT_NO_ERRORS){
    return code;}
  // Write to file the ASCII header only based on the magic number
  if (*(fileValues->magic_Number) == MAGIC_NUMBER_ASCII_PGM){
    fprintf(outputFile, "P2\n%d %d\n%d\n", fileValues->width, fileValues->height, fileValues->maxGray);
  }
  else if (*(fileValues->magic_Number) == MAGIC_NUMBER_RAW_PGM){
    fprintf(outputFile, "P5\n%d %d\n%d\n", fileValues->width, fileValues->height, fileValues->maxGray);
  }
  return EXIT_NO_ERRORS;
}

int writeReducedASCII(FILE *outputFile, FileInfo *fileValues, char* outputFileName, long nImageBytes, unsigned int factor, int width){
  // Check writing permissions
  if (access(outputFileName, W_OK) == -1){
    printErrorStringAndDescription(EXIT_BAD_INPUT_FILE, outputFileName);
    return EXIT_BAD_INPUT_FILE;
  }
  int code=0;
  // Check if the output file is empty
  code = fileEmptyErrorCheck(outputFile, fileValues, outputFileName);
  if (code != EXIT_NO_ERRORS){
    return code;}
  int row=0, column=0;
  // Iterate through each ImageData value
  for (unsigned char *nextGrayValue = fileValues->imageData; nextGrayValue < fileValues->imageData + nImageBytes; nextGrayValue++){
    // Reduce the max number of lines to original divided by factor
    int nextCol = (nextGrayValue - fileValues->imageData + 1) % (fileValues->width*factor);
    // if the coordinates match the factor division, write them
    if (row%factor==0 && column%factor==0){
      fprintf(outputFile, "%d%c", *nextGrayValue, (nextCol ? ' ' : '\n') );}
    // Update coordinates accordingly
    if (row==(fileValues->width-1)){
      row=0;
      column+=1;
    }
    else{
      row+=1;}
  }
  return EXIT_NO_ERRORS;
}

int writeASCIIPropertyBasedOnParameters(FILE *outputFile, FileInfo *fileValues, unsigned int height, unsigned int width, unsigned int maxGray, int isASCII, char* outputFileName){
  int code=0;
  // Check Writing permissions
  if (access(outputFileName, W_OK) == -1){
    printErrorStringAndDescription(EXIT_BAD_INPUT_FILE, outputFileName);
    return EXIT_BAD_INPUT_FILE;
  }
  // Check if the output file is empty
  code = fileEmptyErrorCheck(outputFile, fileValues, outputFileName);
  if (code != EXIT_NO_ERRORS){
    return code;}
  // Write the ASCII header only based on parameters
  if (isASCII == 0){
    fprintf(outputFile, "P2\n%d %d\n%d\n", width, height, maxGray);
  }
  else if (isASCII == 1){
    fprintf(outputFile, "P5\n%d %d\n%d\n", width, height, maxGray);
  }
  return EXIT_NO_ERRORS;
}

int writeASCIIDataInBinaryReduced(FILE *outputFile, FileInfo *fileValues, char* outputFileName, long nImageBytes, int factor){
  // Check writing permissions
  if (access(outputFileName, W_OK) == -1){
    printErrorStringAndDescription(EXIT_BAD_INPUT_FILE, outputFileName);
    return EXIT_BAD_INPUT_FILE;
  }
  int code=0;
  // Check if the file is empty
  code = fileEmptyErrorCheck(outputFile, fileValues, outputFileName);
  if (code != EXIT_NO_ERRORS){
    return code;}
  int row=0, column=0;
  // Same as writeReducedASCII function but the data is written in binary format
  for (unsigned char *nextGrayValue = fileValues->imageData; nextGrayValue < fileValues->imageData + nImageBytes; nextGrayValue++){
    if (row%factor==0 && column%factor==0){
      fwrite(nextGrayValue, 1, 1, outputFile);}
    if (row==(fileValues->width-1)){
      row=0;
      column+=1;
    }
    else{
      row+=1;}
  }
  return 0;
}

int writeBetweenMaxMin(FILE *outputFile, FileInfo *fileValues, char* outputFileName, long nImageBytes, int minRow, int maxRow, int minCol, int maxCol){
  // Check writing permissions
  if (access(outputFileName, W_OK) == -1){
    printErrorStringAndDescription(EXIT_BAD_INPUT_FILE, outputFileName);
    return EXIT_BAD_INPUT_FILE;
  }
  int code=0;
  // Check if the output file is empty
  code = fileEmptyErrorCheck(outputFile, fileValues, outputFileName);
  if (code != EXIT_NO_ERRORS){
    return code;}
  int row=0, column=0;
  size_t sizeOfWritten;
  // Iterate through each ImageData value
  for (unsigned char *nextGrayValue = fileValues->imageData; nextGrayValue < fileValues->imageData + nImageBytes; nextGrayValue++){
    int nextCol = (nextGrayValue - fileValues->imageData + 1) % (fileValues->width);
    // if the coordinates are in range, write them
    if (row>=minRow && row<maxRow && column>=minCol && column<maxCol){
      sizeOfWritten = fprintf(outputFile, "%d%c", *nextGrayValue, (nextCol ? ' ' : '\n') );
      // Check if nothing was written into the file, so little data problem
      if (sizeOfWritten<0){
        free(fileValues->commentLine);
        free(fileValues->imageData);
        printErrorStringAndDescription(EXIT_OUTPUT_FAILED, outputFileName);
        return EXIT_OUTPUT_FAILED;
      }
    }
    // Update coordinates accordingly
    if (column==(fileValues->width-1)){
      column=0;
      row+=1;
    }
    else{
      column+=1;}
  }
  return EXIT_NO_ERRORS;
}

int writeBetweenMaxMinBinary(FILE *outputFile, FileInfo *fileValues, char* outputFileName, long nImageBytes, int minRow, int maxRow, int minCol, int maxCol){
  // Check writing permissions
  if (access(outputFileName, W_OK) == -1){
    printErrorStringAndDescription(EXIT_BAD_INPUT_FILE, outputFileName);
    return EXIT_BAD_INPUT_FILE;
  }
  int code=0;
  // Check if the output file is empty
  code = fileEmptyErrorCheck(outputFile, fileValues, outputFileName);
  if (code != 0){
    return code;}
  int row=0, column=0;
  size_t sizeOfWritten;
  // Iterate through each ImageData value
  for (unsigned char *nextGrayValue = fileValues->imageData; nextGrayValue < fileValues->imageData + nImageBytes; nextGrayValue++){
    // if the coordinates are in range, write them
    if (row>=minRow && row<maxRow && column>=minCol && column<maxCol){
      sizeOfWritten = fwrite(nextGrayValue, 1, 1, outputFile);
      // Check if nothing was written into the file, so little data problem
      if (sizeOfWritten<0){
        free(fileValues->commentLine);
        free(fileValues->imageData);
        printErrorStringAndDescription(EXIT_OUTPUT_FAILED, outputFileName);
        return EXIT_OUTPUT_FAILED;
      }}
    // Update coordinates accordingly
    if (column==(fileValues->width-1)){
      column=0;
      row+=1;
    }
    else{
      column+=1;}
  }
  return EXIT_NO_ERRORS;
}
