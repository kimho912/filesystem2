#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <stdint.h>
#include "../include/extfat.h"
#include "../include/command.h"
#include "../include/copy.h"
#include "../include/verify.h"
#include "../include/delete.h"
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <limits.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include "extfat.h"
#include "routines.h"
#define _GNU_SOURCE

#ifndef NOMAIN
//#define _POSIX_SOURCE
//#undef _POSIX_SOURCE
char file_name[100];

void* file_info(fileInfo *infile);
void copy_to_file(char* input_file, char* file_ptr);
int main(int argc,char *argv[])
{
   // struct for user input
   // initialize and define the flag structure
   //flag is handled by command.c/h
   argStruct flag;
   
   // Create structs for files 
   fileInfo *infile = NULL;
   infile = (fileInfo *)malloc(sizeof(fileInfo));
   fileInfo *outfile = NULL;
   outfile = (fileInfo *)malloc(sizeof(fileInfo));
   //for mapping
   void * fp1;
   void * fp2;


   // Pass the struct to read the command line
   //(-i,-o, -h and -d)
   flag = cmd_ops_process(argc,argv);

   // obtain the file name 
   infile->fileName = flag.inFile;
   outfile->fileName = flag.outFile;
   //filing out the fileinfo
   printf("[%s] Info:\n",infile->fileName);
   fp1 = file_info(infile);


   //add if statement if ouput file is not defined vvv
   printf("input file name: %s \noutput file name: %s \n\n",flag.inFile,flag.outFile);

   // check if -c, -v, or -d flag is on
   if (flag.cArg == 1)
   {
      // copy input file to output file
      printf("\ncopying input file to output file...\n");
      data_memory_output(infile->fileName,outfile->fileName);
      printf("[%s] info:\n",outfile->fileName);
      fp2 = file_info(outfile);
   }
   if (flag.vArg == 1)
   {
      // verify inputfile if the main and the backup are matching.
      printf("\nverifying input file...\n");
      printf("File name: %s\n",infile->fileName);
      fp2 = file_info(outfile);
      int stat = verify_file(infile->fileName,infile->M_Boot);
      if (stat)
      {
         printf("Success!\n");
      }
   }
   if(flag.dArg == 1)
   {
      printf("Directory:\n");
      explore("/workspaces/cse3310_s002_group_22", 0);
   }
   if(flag.deleteFile != NULL)
   {
      printf("Deleting file\n");
      system("sudo umount /dev/loop2");
      delete_file(infile->M_Boot,flag.deleteFile,infile->fileName);
      system("sudo mount /dev/loop2 /tmp/d");
   }
   if(flag.xArg == 1)
   {
      char* name1 = extract_file_name();
      printf("\n\nFile extracted: %s\n", name1);
      copy_to_file(infile->fileName,outfile->fileName);
   }
   //unmap and free
   munmap(fp1,infile->FileLength);
   munmap(fp2,outfile->FileLength);
   free (infile);
   free (outfile);
   return 0;
}
#endif
void* file_info(fileInfo *file)
{
   // struct for file size
   struct stat statbuf;

   // open the file system image file
   file->fd = open(file->fileName, O_RDWR);
   if (file->fd == -1)
   {
      perror("file open: ");
      exit(0);
   }

   // FileLength
   if (fstat(file->fd, &statbuf))
   {
      perror("stat of file:");
      exit(0);
   }
   file->FileLength = statbuf.st_size;
   printf("FileLength: %dbytes\n", file->FileLength);
   
   // Maping the file
   void *fp = (void *)mmap(NULL,
                           file->FileLength,
                           PROT_WRITE,
                           MAP_PRIVATE,
                           file->fd,
                           0);

   if (fp == (void *)-1)
   {
      perror("mmap:");
      exit(0);
   }
   file->M_Boot = (Main_Boot *)fp;
   printf("MasterBootRecord %p\n", file->M_Boot);
   //SectorSize
   file->SectorSize = 2  << (file->M_Boot->BytesPerSectorShift - 1);
   printf("SectorSize: %d \n", file->SectorSize);
   //The Backup Boot record
   file->B_Boot = (Main_Boot *)(fp + 12 * file->SectorSize);
   printf("BackUpBootRecord %p\n", file->B_Boot);
   //FAT area
   file->FAT = ((void *)file->M_Boot + (file->M_Boot->FatOffset * file->SectorSize));
   printf("FAT area: %p \n", file->FAT);
   //DATA area
   file->Data = ((file->M_Boot + file->M_Boot->ClusterHeapOffset * file->SectorSize));
   printf("Data area: %p \n", file->Data);

   close(file->fd);
   
   return fp;
}
