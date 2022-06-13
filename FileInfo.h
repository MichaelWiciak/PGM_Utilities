#ifndef _FILEINFO_
#define _FILEINFO_

typedef struct FileInfo {
  char *commentLine;
  unsigned int width;
  unsigned int height;
  unsigned int maxGray;
  unsigned char *imageData;
  unsigned short *magic_Number;
  unsigned char magic_number[2];
} FileInfo;

#endif
