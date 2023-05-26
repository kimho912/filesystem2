#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <stddef.h>
#include <string.h>
#include "../include/copy.h"

void data_memory_output(char* finput, char* fd)
{
    int input = open(finput, O_RDWR);
    if(input == -1)
   {
      perror("File open: ");
      exit(0);
   }

   struct stat sb;

   if(fstat(input, &sb) == -1)
   {
      perror("couldn't get file size");
   }

   FILE* fp;
   fp = fopen(fd, "w+");

   //checking if the file is empty
   if (fp == NULL) 
   {
      printf("Output file did not open.\n");
      exit(0);
   }

   //moving fp to end
   fseek(fp, (sb.st_size) - 1, SEEK_END);

   //fwrite the one byte
   char* byte = (char*)malloc(1); // allocate one byte of memory
   *byte = '\0';
   fwrite(byte, 1, 1, fp);
   free(byte); // free the allocated memory

   int foutput = open(fd, O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR); 
   //close the file
   fclose(fp);

   void* SF = mmap(NULL, sb.st_size, PROT_READ | PROT_WRITE, MAP_PRIVATE, input, 0);
   //mmap'ing the output file 

   if (foutput == -1) 
   {
      perror("file open: ");
      exit(0);
   }

   void* DF = mmap(NULL, sb.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, foutput, 0);

   //memory copying the source file to the destination file
   memcpy(DF, SF, sb.st_size);
   munmap(SF, sb.st_size);
   munmap(DF, sb.st_size);

    // close the file
   close(foutput);
   close(input);
}