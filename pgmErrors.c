#include "pgmErrors.h"
#include <stdio.h>
#include <string.h>

void printErrorStringAndDescription (int errorCode, char* fileNameWithError){
  // Note: fileNameWithError can be just a description in string format
  // Prints out the error description based on errorCode provided
  switch (errorCode) {
    case 1:
      printf("ERROR: Bad Argument Count \t Programme given wrong # of arguments\n");
      break;
    case 2:
      printf("ERROR: Bad File Name (%s) \t Programme failed to open a file stream\n", fileNameWithError);
      break;
    case 3:
      printf("ERROR: Bad Magic Number (%s)\tProgramme failed on a magic number\n", fileNameWithError);
      break;
    case 4:
      printf("ERROR: Bad Comment Line (%s)\t Programme failed on comment line\n", fileNameWithError);
      break;
    case 5:
      printf("ERROR: Bad Dimensions (%s)\t Programme failed on image dimensions\n", fileNameWithError);
      break;
    case 6:
      printf("ERROR: Bad Max Gray Value (%s)\t Programme failed on max gray values\n", fileNameWithError);
      break;
    case 7:
      printf("ERROR: Image Malloc Failed \t Malloc failed for image allocation\n");
      break;
    case 8:
      printf("ERROR: Bad Data (%s) \t Programme failed when reading in data\n", fileNameWithError);
      break;
    case 9:
      printf("ERROR: Output Failed (%s) \t Programme failed during output\n", fileNameWithError);
      break;
    case 10:
      printf("ERROR: Bad Layout \t Layout file for assembly went wrong\n");
      break;
    case 100:
      printf("ERROR: Miscellaneous \t(%s)\n", fileNameWithError);
      break;
  }

}

void printUsageString (char* programName){
  // Prints out the usage string for every executable program
  if (strcmp("./pgmEcho", programName)==0){
    printf("Usage: ./pgmEcho inputImage.pgm outputImage.pgm\n");
  }
  else if (strcmp("./pgmComp", programName)==0){
    printf("Usage: ./pgmComp inputImage.pgm inputImage.pgm\n");
  }
  else if (strcmp("./pgma2b", programName)==0){
    printf("Usage: ./pgma2b inputImage.pgm outputImage.pgm\n");
  }
  else if (strcmp("./pgmb2a", programName)==0){
    printf("Usage: ./pgmb2a inputImage.pgm outputImage.pgm\n");
  }
  else if (strcmp("./pgmReduce", programName)==0){
    printf("Usage: ./pgmReduce inputImage.pgm reduction_factor outputImage.pgm\n");
  }
  else if (strcmp("./pgmTile", programName)==0){
    printf("Usage: ./pgmTile inputImage.pgm tiling_factor outputImage_<row>_<column>.pgm\n");
  }
  else if (strcmp("./pgmAssemble", programName)==0){
    printf("Usage: ./pgmAssemble outputImage.pgm width height (row column inputImage.pgm)+n\n");
  }
}

void printPGMCompOutput (int count){
  // Prints out a message for pgmComp based on count
  if (count==0){
    printf("IDENTICAL\tpgmComp\tThe two files were identical\n");
  }
  else if (count==1){
    printf("DIFFERENT\tpgmComp\tThe two files were not identical\n");
  }
}

void printSuccessString (char* programName){
  // Prints out the usage string for every executable program
  if (strcmp("./pgmEcho", programName)==0){
    printf("ECHOED\t pgmEcho\t Programme echoed the input\n");
  }
  else if (strcmp("./pgma2b", programName)==0){
    printf("CONVERTED\t %s\t The file was converted\n", programName);
  }
  else if (strcmp("./pgmb2a", programName)==0){
    printf("CONVERTED\t %s\t The file was converted\n", programName);
  }
  else if (strcmp("./pgmReduce", programName)==0){
    printf("REDUCED\t pgmReduce\t The file was successfully reduced in size\n");
  }
  else if (strcmp("./pgmTile", programName)==0){
    printf("TILED\t pgmTile\t The tiles were successfully written\n");
  }
  else if (strcmp("./pgmAssemble", programName)==0){
    printf("ASSEMBLED\t pgmAssemble\t The full image was written\n");
  }
}
