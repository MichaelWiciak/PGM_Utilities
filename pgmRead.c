#include "pgmRead.h"
#include <stdlib.h>
#include <stdio.h>
#include "pgmCheckingValue.h"
#include "pgmErrors.h"

FileInfo *initialisationOfReadFileStruct (){
  // Creates and initialises the fileInfo structure to default values
  FileInfo *output = (FileInfo*) malloc (sizeof(FileInfo));
  output->commentLine = NULL;
  output->magic_number[0] = '0';
  output->magic_number[1] = '0';
  output->magic_Number = (unsigned short *) output->magic_number;
  output->width=0;
  output->height=0;
  output->maxGray=255;
  output->imageData=NULL;
  // Return the pointer to this structure
  return output;
}

void scanForWhiteSpace (FILE *file){
  // Scan for whitespace in the file
  fscanf(file, " ");
}


void closeTheFile (FILE *file){
  // close the file supplied
  fclose(file);
}

int readCommentAndIgnore(FILE *file, int maxCommentLen, FileInfo *fileValues){
  char nextChar = fgetc(file);
  // Check if this is a comment line
  if (nextChar == '#'){
    fileValues->commentLine = (char *) malloc(maxCommentLen + 2);
    // Capture the comment
    char *commentString = fgets(fileValues->commentLine, maxCommentLen + 2, file);
    // Check if a comment was successfully read
    if (
      (commentString == NULL) ||
      (strlen(commentString) > MAX_COMMENT_LINE_LENGTH) ){
      // Free dynamically allocated memory and terminate with error code
        free(fileValues->commentLine);
        closeTheFile(file);
        return 2;
      }
    // correct comment
    free(fileValues->commentLine);
    return 1;}
  // Not a comment line
  else {
    ungetc(nextChar, file);
    return 0;}
}

void getMagicNumber (FileInfo *fileValues, FILE *file){
  // Modify the fileValues struct magic_number based on data in file
  fileValues->magic_number[0] = getc(file);
  fileValues->magic_number[1] = getc(file);
}

int checkIfInputFileEmpty(FILE *inputFile, char* filename){
  if (inputFile == NULL){
    printErrorStringAndDescription(EXIT_BAD_INPUT_FILE, filename);
    return EXIT_BAD_INPUT_FILE;}
  return EXIT_NO_ERRORS;
}

int checkIfValidMagicNumber(char* filename, FileInfo *fileValues){
  // Check if the magic number is either P2 or P5
  if (*(fileValues->magic_Number) != MAGIC_NUMBER_ASCII_PGM && *(fileValues->magic_Number) != MAGIC_NUMBER_RAW_PGM){
    // If not, terminate with error code
    printErrorStringAndDescription(EXIT_BAD_MAGIC_NUMBER, filename);
    return EXIT_BAD_MAGIC_NUMBER;
    }
  return EXIT_NO_ERRORS;
}

int checkIfValidDimension(char* filename, FileInfo *fileValues){
  // Check if the magic number is either P2 or P5
  if (
      (fileValues->width < MIN_IMAGE_DIMENSION) 	||
      (fileValues->width >= MAX_IMAGE_DIMENSION) 	||
      (fileValues->height < MIN_IMAGE_DIMENSION) 	||
      (fileValues->height >= MAX_IMAGE_DIMENSION)  ){
    // If not, terminate with error code
    printErrorStringAndDescription(EXIT_BAD_DIMENTION, filename);
    return EXIT_BAD_DIMENTION;
    }
  return EXIT_NO_ERRORS;
}

int printBadCommentLineError(char *filename){
  printErrorStringAndDescription(EXIT_BAD_COMMENT_LINE,filename);
  return EXIT_BAD_COMMENT_LINE;
}

int checkWidthHeightMaxGrayAreValid(int scanCount, unsigned int width, unsigned int height, unsigned int maxGray, char *filename){
  // Check if the values for width, height, maxGray are valid
  if 	(
    (scanCount != 3)	||
    (width < MIN_IMAGE_DIMENSION) 	||
    (width > MAX_IMAGE_DIMENSION) 	||
    (height < MIN_IMAGE_DIMENSION) 	||
    (height > MAX_IMAGE_DIMENSION) 	||
    // MaxGray for pgm is from 0 to 255, 256 values
    (maxGray > 255) ||
    (maxGray < 0)
    ){
    // Not valid value(s) so terminate with error code
    printErrorStringAndDescription(EXIT_BAD_INPUT_FILE, filename);
    return EXIT_BAD_INPUT_FILE;
  }
  return EXIT_NO_ERRORS;
}

int checkMaxGrayValid(unsigned int maxGray, char *filename){
  if ((maxGray > 255) || (maxGray < 0)){
    printErrorStringAndDescription(EXIT_BAD_MAX_GRAY_VALUE, filename);
    return EXIT_BAD_MAX_GRAY_VALUE;
  }
  return EXIT_NO_ERRORS;
}

int readFile(char* filename, FileInfo *fileValues, FILE *inputFile){
  if (access(filename, R_OK) == -1){
    printErrorStringAndDescription(EXIT_BAD_INPUT_FILE, filename);
    return EXIT_BAD_INPUT_FILE;
  }
  // Check if file supplied is empty
  int code = checkIfInputFileEmpty(inputFile, filename);
  if (code !=0){
    return code;}
  // Fetches the magic number from file
  getMagicNumber(fileValues, inputFile);
  // Check if magicNumber in file is valid
  code = checkIfValidMagicNumber(filename, fileValues);
  // If not, raise error and close file
  if (code !=0){
    closeTheFile(inputFile);
    return code;}

  int scanCount;
  // Scan for whitespace to remove it
  scanForWhiteSpace(inputFile);
  // Check if file contains a comment
  int aOutput = readCommentAndIgnore(inputFile, MAX_COMMENT_LINE_LENGTH, fileValues);
  if (aOutput==2){
    code = printBadCommentLineError(filename);
    return code;}
  // Get width, height, maxGray from file
  scanCount = fscanf(inputFile, " %u %u %u", &(fileValues->width), &(fileValues->height), &(fileValues->maxGray));
  // Check if the values from file are valid for pgm formatting
  code = checkIfValidDimension(filename, fileValues);
    // If not, raise error and close file
  if (code !=0){
    closeTheFile(inputFile);
    return code;}
  code = checkMaxGrayValid(fileValues->maxGray, filename);
  // If not, raise error and close file
  if (code !=0){
    closeTheFile(inputFile);
    return code;}

  code = checkWidthHeightMaxGrayAreValid(scanCount, fileValues->width, fileValues->height, fileValues->maxGray, filename);
  if (code!=0){
    // Terminate, free memory and close file
    free(fileValues->commentLine);
    closeTheFile(inputFile);
    return code;
    }
  return EXIT_NO_ERRORS;
}

int checkIfImageDataIsNULL(FileInfo *fileValues, char* filename){
  // Check if imageData is null
  if (fileValues->imageData == NULL){
    // If yes, terminate with error code and printing description of error
    printErrorStringAndDescription(EXIT_BAD_IMAGE_MALLOC_FAILED, filename);
    return EXIT_BAD_IMAGE_MALLOC_FAILED;}
  return EXIT_NO_ERRORS;
}

int checkImageValuesAreValid(int scanCount, int grayValue, FileInfo *fileValues, char *filename){
  // Check if the gray Value is valid
  // Note: grey value is also invalid if it is bigger than the maxGray value of its file
  if ((scanCount != 1) || (grayValue < 0) || (grayValue > 255) || (grayValue>fileValues->maxGray)){
    printErrorStringAndDescription(EXIT_BAD_DATA, filename);
    return EXIT_BAD_DATA;
    }
  return EXIT_NO_ERRORS;
}

int readImageData(FileInfo *fileValues, long nImageBytes, FILE *inputFile, char* filename){
  if (access(filename, R_OK) == -1){
    printErrorStringAndDescription(EXIT_BAD_INPUT_FILE, filename);
    return EXIT_BAD_INPUT_FILE;
  }
  // Allocate memory on the heap for imageData
  fileValues->imageData = (unsigned char *) malloc(nImageBytes);
  if (fileValues->imageData == NULL){
    printErrorStringAndDescription(EXIT_BAD_IMAGE_MALLOC_FAILED, filename);
    return EXIT_BAD_IMAGE_MALLOC_FAILED;
  }
  // Check if malloc function worked
  int code = checkIfImageDataIsNULL(fileValues, filename);
  if (code!= 0){
    // If not, terminate with error code, free memory and close file
    free(fileValues->commentLine);
    closeTheFile(inputFile);
    return code;}
  // For each value in file, read it, check it and write its pointer into imageData if valid
  int amountOfValues=0;
  for (unsigned char *nextGrayValue = fileValues->imageData; nextGrayValue < fileValues->imageData + nImageBytes; nextGrayValue++){
    int grayValue = -1;
    // Retrieve the value for grayValue from file
    int scanCount = fscanf(inputFile, " %u", &grayValue);
    // Check if values in file are valid
    if (scanCount != 1){
      printErrorStringAndDescription(EXIT_BAD_DATA, filename);
      return EXIT_BAD_DATA;
    }
    code = checkImageValuesAreValid(scanCount, grayValue, fileValues, filename);
    if (code!=0){
      // Invalid data, free memory, close file and terminate error code
      free(fileValues->commentLine);
      free(fileValues->imageData);
      closeTheFile(inputFile);
      return code;}
    // Assign a pointer to imageData, pointing to grayValue
    *nextGrayValue = (unsigned char) grayValue;
    amountOfValues++;
    }
  int grayValue = -1;
  int lastValue = fscanf(inputFile, " %u", &grayValue);
  lastValue = fscanf(inputFile, " %u", &grayValue);
  // Check for bad data
  if ((lastValue != -1) || (amountOfValues != (fileValues->height * fileValues->width))){
    printErrorStringAndDescription(EXIT_BAD_DATA, filename);
    return EXIT_BAD_DATA;
  }
  return EXIT_NO_ERRORS;
}

int readImageDataBinary(FileInfo *fileValues, long nImageBytes, FILE *inputFile, char* filename){
  // Check reading rights
  if (access(filename, R_OK) == -1){
    printErrorStringAndDescription(EXIT_BAD_INPUT_FILE, filename);
    return EXIT_BAD_INPUT_FILE;
  }
  // Allocate memory on the heap for imageData
  fileValues->imageData = (unsigned char *) malloc(nImageBytes);
  // Check if malloc function worked
  if (fileValues->imageData == NULL){
    printErrorStringAndDescription(EXIT_BAD_IMAGE_MALLOC_FAILED, filename);
    return EXIT_BAD_IMAGE_MALLOC_FAILED;
  }
  int code = checkIfImageDataIsNULL(fileValues, filename);
  if (code!= 0){
    // If not, terminate with error code, free memory and close file
    free(fileValues->commentLine);
    closeTheFile(inputFile);
    return code;}
  // For each value in file, read it, check it and write its pointer into imageData if valid
  int grayValue;
  char uselessCharacter = fgetc(inputFile);
  uselessCharacter ++;
  for (int index = 0; index < fileValues->width * fileValues->height * sizeof(unsigned char); index++){
    // Retrieve the value for grayValue from file
    grayValue = fgetc(inputFile);
    if (grayValue == -1){
      printErrorStringAndDescription(EXIT_BAD_DATA, filename);
      return EXIT_BAD_DATA;
    }
    // Check if values in file are valid
    if ((grayValue > 255) || (grayValue < 0)){
      printErrorStringAndDescription(EXIT_BAD_DATA, filename);
      free(fileValues->commentLine);
      free(fileValues->imageData);
      closeTheFile(inputFile);
      return EXIT_BAD_DATA;
    }
    // Assign a pointer to imageData, pointing to grayValue
    fileValues->imageData[index] = (unsigned char) grayValue;
    }
  // Check if after finishing reading the file there is more data to read
  int lastValue = fgetc(inputFile);
  if (lastValue != -1){
    printErrorStringAndDescription(EXIT_BAD_DATA, filename);
    return EXIT_BAD_DATA;
  }
  return EXIT_NO_ERRORS;
}
