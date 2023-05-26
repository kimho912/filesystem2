typedef struct 
{
   char *inFile;
   char *outFile;
   char *deleteFile;
   int cArg;
   int vArg;
   //new vvv
   int dArg;
   int xArg;
}argStruct;

argStruct cmd_ops_process(int argc, char *argv[]);