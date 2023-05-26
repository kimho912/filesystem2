#pragma once
#include "extfat.h"

typedef struct
   {
      union
      {
         uint8_t EntryType;
         struct
         {
            uint8_t TypeCode : 5;
            uint8_t TypeImportance : 1;
            uint8_t TypeCategory : 1;
            uint8_t InUse : 1;
         };
      };
      uint8_t CustomDefined[19];
      uint32_t FirstCluster;
      uint64_t DataLength;
   } GenericDirectoryStruct;

void delete_file(Main_Boot* MB,char * filename, char* Mfilename);
