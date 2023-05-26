#include "../include/verify.h"
#include "../include/routines.h"



int verify_file(char* input_file, Main_Boot * MB)// new file only created if -v is present...
{   
    
    int bytesPerSector = 2 << (MB->BytesPerSectorShift - 1);

    if(strcmp(strrchr(input_file, '.'), ".image") == 0)
    {
        Main_Boot *BB = (Main_Boot *)((void*)MB + 12 * bytesPerSector);

        uint32_t mbrChecksum = BootChecksum((uint8_t*) MB, (short) bytesPerSector);
        uint32_t bbrChecksum = BootChecksum((uint8_t*) BB, (short) bytesPerSector);
        

        printf("Checksum  (MB) %x (BBR) %x\n",mbrChecksum,bbrChecksum);
        if(mbrChecksum == bbrChecksum)
        {
            printf("The backup is equal!\n");
        }
        else
        {
            printf("The backup is not equal.\n");
        }
    }
    else
    {
        printf("Unable to verify non .image file\n");
        return 0;
    }

   return 1;
}
