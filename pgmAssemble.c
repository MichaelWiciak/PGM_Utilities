#include <stdio.h>
#include <stdlib.h>
#include "pgmCheckingValue.h"
#include "pgmErrors.h"
#include "pgmRead.h"
#include "pgmWrite.h"
#include <math.h>

int main(int argc, char **argv){
  // Check argument count supplied is correct
  int code = checkArgsErrorsForAssemble(argc, argv[0]);
  //int isASCII=0;
  if (code!=999){
    // If not, terminate with error code
    return code;}
  code = 0;
  // Calculate output Image width and height
  int outputImageWidth = atoi(argv[2]);
  int outputImageHeight = atoi(argv[3]);
  div_t numberOfImages = div((argc - 4),3);
  // Calculate the number of images
  int imageNumber = numberOfImages.quot;
  // Check all the read files are read accessible
  for (int imageIndex=0; imageIndex<imageNumber; imageIndex++){
    FILE *inputFile = fopen(argv[4 + 2 +(imageIndex*3)], "r");
    if (inputFile==NULL){
      // If not, terminate with error code
      printErrorStringAndDescription(EXIT_BAD_INPUT_FILE, argv[4 + 2 +(imageIndex*3)]);
      return EXIT_BAD_INPUT_FILE;
    }
    closeTheFile(inputFile);
  }
  // Initialise output file in structure
  FILE *outputFile = fopen(argv[1], "w");
  FileInfo *fileValues = initialisationOfReadFileStruct();
  fileValues->magic_number[0] = 'P';
  fileValues->magic_number[1] = '2';
  fileValues->width = outputImageWidth;
  fileValues->height = outputImageHeight;
  fileValues->maxGray = 255;
  // Initialise the imageData to 0 at every coordinate;
  long nImageBytes = fileValues->width * fileValues->height * sizeof(unsigned char);
  fileValues->imageData = (unsigned char *) malloc(nImageBytes);
  if (fileValues->imageData == NULL){
    printErrorStringAndDescription(EXIT_BAD_IMAGE_MALLOC_FAILED, argv[1]);
    return EXIT_BAD_IMAGE_MALLOC_FAILED;
  }
  for (unsigned char *nextGrayValue = fileValues->imageData; nextGrayValue < fileValues->imageData + nImageBytes; nextGrayValue++){
    int grayValue = 255;
    *nextGrayValue = (unsigned char) grayValue;
    }

  int rowStart[imageNumber], rowEnd[imageNumber], colStart[imageNumber], colEnd[imageNumber];
  for (int imageIndex=0; imageIndex<imageNumber; imageIndex++){
    // Read the image at index
    FileInfo *fileValuesInput = initialisationOfReadFileStruct();
    FILE *inputFile = fopen(argv[4 + 2 +(imageIndex*3)], "r");
    code = readFile(argv[4 + 2 +(imageIndex*3)],fileValuesInput, inputFile);
    // Check if that image can be read
    if (code!=0){
      // If not, terminate with error code
      return code;
    }
    long nImageBytesInput = fileValuesInput->width * fileValuesInput->height * sizeof(unsigned char);
    if (*(fileValuesInput->magic_Number) == MAGIC_NUMBER_ASCII_PGM){
      code = readImageData(fileValuesInput, nImageBytesInput, inputFile, argv[4 + 2 +(imageIndex*3)]);}
    else if (*(fileValuesInput->magic_Number) == MAGIC_NUMBER_RAW_PGM){
      code = readImageDataBinary(fileValuesInput, nImageBytesInput, inputFile, argv[4 + 2 +(imageIndex*3)]);}
    // Check if successfully written imageData
    if (code !=0){
      return code;}
    closeTheFile(inputFile);
    // Store the image values in arrays
    //storage[imageIndex] = fileValuesInput;
    rowStart[imageIndex] = atoi(argv[4 + 0 +(imageIndex*3)]);
    rowEnd[imageIndex] = atoi(argv[4 + 0 +(imageIndex*3)])+fileValuesInput->height;
    colStart[imageIndex] = atoi(argv[4 + 1 +(imageIndex*3)]);
    colEnd[imageIndex] = atoi(argv[4 + 1 +(imageIndex*3)]) + fileValuesInput->width;
    // Write the content of that image into the output file
    int row=0, column=0, inputImageIndex=0;
    for (unsigned char *nextGrayValue = fileValues->imageData; nextGrayValue < fileValues->imageData + nImageBytes; nextGrayValue++)
    {
      int grayValue = 255;
      // if the coordinates are in range, write them
      if (row>=rowStart[imageIndex] && row<rowEnd[imageIndex] && column>=colStart[imageIndex] && column<colEnd[imageIndex]){
        grayValue = fileValuesInput->imageData[inputImageIndex];
        *nextGrayValue = (unsigned char) grayValue;
        inputImageIndex++;
      }
      *nextGrayValue = *nextGrayValue;
      //  Update the index values accordingly
      if (column==(fileValues->width-1)){
        column=0;
        row+=1;
      }
      else{
        column+=1;
      }
    }
  }
  // Try to write the image data into the output file
  code = writeToFile(outputFile, fileValues, argv[1], nImageBytes);
  if (code!=0){
    // If error, return the error code
    return code;
  }
  // Program successfully performed assemble
  closeTheFile(outputFile);
  printSuccessString(argv[0]);
  return 0;
}
