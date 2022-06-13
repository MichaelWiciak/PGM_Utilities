// Error code to be used when returing an error code or successul execution code
#define EXIT_NO_ERRORS 0
#define EXIT_WRONG_ARG_COUNT 1
#define EXIT_BAD_INPUT_FILE 2
#define EXIT_BAD_MAGIC_NUMBER 3
#define EXIT_BAD_COMMENT_LINE 4
#define EXIT_BAD_DIMENTION 5
#define EXIT_BAD_MAX_GRAY_VALUE 6
#define EXIT_BAD_IMAGE_MALLOC_FAILED 7
#define EXIT_BAD_DATA 8
#define EXIT_OUTPUT_FAILED 9
#define EXIT_BAD_LAYOUT 10
#define EXIT_MISCELLANEOUS 100

#define MAGIC_NUMBER_RAW_PGM 0x3550
#define MAGIC_NUMBER_ASCII_PGM 0x3250
#define MIN_IMAGE_DIMENSION 1
#define MAX_IMAGE_DIMENSION 65536
#define MAX_COMMENT_LINE_LENGTH 128

#include "pgmErrors.h"
#include "FileInfo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// Constructors for functions in pgmChecking
int checkArgs (int argCount, int targetCount);
int checkArgsErrors(int args, char* filename);
int checkIfTwoPGMFilesAreLogicallyIdentical(FileInfo *fileValues1, FileInfo *fileValues2);
int checkArgsErrorsForReduce(int argc, char* filename);
int checkIfinteger(char* string);
int checkIfMagicNumberInBinaryandSwitchToASCII(FileInfo *fileValues, char* filename);
int checkIfMagicNumberInASCIIandSwitchToBinary(FileInfo *fileValues, char* filename);
int checkArgcUsageString(int argc, char* filename);
int checkArgsErrorsForAssemble(int argc, char* filename);
