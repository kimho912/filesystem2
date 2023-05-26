#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <stddef.h>
#include <getopt.h>
#include <string.h>
#include "../include/command.h"


argStruct cmd_ops_process(int argc, char *argv[])
{
   //added extra 0 vvv
   argStruct cmdargs = {NULL, NULL, 0, 0, 0, 0, 0};
   int opt;

   // i:o:hcv --> i:o:hcvd (adding option for the "-d" flag)
   // maybe i:o:hcv:d so that wanted file is verified
   while ((opt = getopt(argc, argv, "i:o:D:hcvdx")) != -1)
   {
      switch (opt)
      {
         case 'i':
            cmdargs.inFile = optarg;

         break;
         case 'o':
            cmdargs.outFile = optarg;
         break;
         case 'D':
            cmdargs.deleteFile = optarg;
         break;
         case 'h':
            printf("\nOptions:\n"
                     "-i    Create or specify an input file (1 argument)\n"
                     "-o    Create or specify an output file (Set to same as input file by default) (1 argument)\n"
                     "-h    Print list of arguments and their meanings\n"
                     "-c    Copy the input file to the output file without change\n"
                     "-v    Check if the main and backup boot sectors are the same\n"
                     "-d    Stdout the file directory\n"
                     "-x    extract a file from the filesystem.\n"
                     "-D    Delete the file.\n");
         break;
         case 'c':
            cmdargs.cArg = 1;
         break;
         case 'v':
            cmdargs.vArg = 1;
         break;
         //new vvv
         case 'd':
            cmdargs.dArg = 1;
         break;
         case 'x':
            cmdargs.xArg = 1;
         break;
         
         default:
         perror("Argument invalid!\n");
         break;
      }
   }
   if(cmdargs.inFile == NULL)
   {
      printf("Sorry, you need to include -i along with an input file name! (Use -h for help)\n");
      exit(EXIT_FAILURE);
   }
   if(cmdargs.outFile == NULL)
   {
      cmdargs.outFile = cmdargs.inFile;
   }
   

   return cmdargs;
}
