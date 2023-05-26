#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/mman.h>
#include <stddef.h>
#include <stdint.h>
#include "../include/delete.h"

#define cluster(N) (((void *)MB + MB->ClusterHeapOffset * bytesPerSector) + ((N - 2) * bytesPerSector * sectorsPerCluster))

char tempfile[64] = "";

void delete_file(Main_Boot *MB, char *filename, char *Mfilename)
{

    int bytesPerSector = 2 << (MB->BytesPerSectorShift - 1);
    int sectorsPerCluster = 2 << (MB->SectorsPerClusterShift - 1);
    printf("addre: %p\n", (void *)MB);
    printf("filename: %s\n", filename);
    printf("The size of GenericDirectoryStruct %ld %lx\n", sizeof(GenericDirectoryStruct), sizeof(GenericDirectoryStruct));
    assert(sizeof(GenericDirectoryStruct) == 32);

    int fd = open(Mfilename, O_RDWR);
    printf("fd:%d\n", fd);
    // directory
    GenericDirectoryStruct *GDS = cluster(MB->FirstClusterOfRootDirectory);
    printf("FirstClusterofRootDirectory %d\n", MB->FirstClusterOfRootDirectory);

    printf("The directory listing\n");
    // The directory listings are complicated.  This code just extracts the file names.
    // it does not handle directories.
    int i = 0;
    int k = 0;
    uint32_t FindCluster = 0;
    uint64_t DataLeng = 0;
    long int Data_offset = 0;
    long int filename_offset = 0;

    for (k = MB->FirstClusterOfRootDirectory + 1; k < ((MB->ClusterCount) - 1) / 512; k++)
    {
        for (i = 0; i < (MB->ClusterCount - 1) / 512; i++)
        {
            while (GDS[i].EntryType)
            {
                printf("%8lx\tThe Entry Type is %x  \tDataLength %8ld \tFirstCluster %d \n", (void *)&GDS[i] - (void *)MB, GDS[i].EntryType, GDS[i].DataLength, GDS[i].FirstCluster);
                if (GDS[i].InUse && GDS[i].EntryType == 0xc0)
                {
                    FindCluster = GDS[i].FirstCluster;
                    printf("FindCluster= %d\n", FindCluster);
                    printf("address[%d]=%p\n", FindCluster, cluster(FindCluster));
                    Data_offset = (long int)cluster(FindCluster) - (long int)(void *)MB;
                    DataLeng = GDS[i].DataLength;
                }
                if (GDS[i].InUse && GDS[i].EntryType == 0x85)
                {
                    filename_offset = (long int)(void *)&GDS[i].EntryType - (long int)(void *)MB;
                    printf("GDS[i].EntryType at 85: %p\n", &GDS[i].EntryType);
                    printf("filename_offset = %lx\n", filename_offset);
                }
                if (GDS[i].InUse && GDS[i].EntryType == 0xc1)
                {
                    struct x
                    {
                        uint8_t EntryType : 8;
                        uint8_t GeneralSecondaryFlags : 8;
                        uint8_t FileName[30];
                    };
                    struct x *X = (struct x *)&GDS[i];
                    char *ch = (char *)X->FileName;
                    printf("\t");
                    int j = 0;
                    while (*ch)
                    {
                        tempfile[j] = *ch;
                        // go to the next, and skip one
                        ch++;
                        ch++;
                        j++;
                    }
                    printf("tempfile= %s\n", tempfile);
                }

                if (strcmp(tempfile, filename) == 0)
                {
                    // GDS[i].InUse=0;
                    printf("cluster(FindCluster)= %p, %ld\n", cluster(FindCluster), DataLeng);

                    // get the offset
                    //  Data_offset = (long int)cluster(i) - (long int)(void*)MB;
                    //  memset((void *)cluster(FindCluster),0,DataLeng);

                    // move the pointer to the content address
                    lseek(fd, Data_offset, SEEK_SET);
                    printf("Data_offset: %lx, DataLeng: %ld\n", Data_offset, DataLeng);

                    // set the content's bytes to 0
                    for (uint64_t i = 0; i < DataLeng; i++)
                    {
                        write(fd, "\0", sizeof(char));
                    }
                    printf("removing the content of the file is done!\n");

                    // move the pointer to the filename address
                    lseek(fd, filename_offset, SEEK_SET);
                    printf("filename_offset: %lx\n", filename_offset);

                    for (uint64_t i = 0; i < 96; i++)
                    {
                        write(fd, "\0", sizeof(char));
                    }
                    printf("removing the filename is done!\n");
                    return;
                }
                break;
                // i++;
            }
        }
        GDS = cluster(k);
    }

    if (strcmp(tempfile, filename) != 0)
    {
        printf("there is no file name [%s] in the filesystem\n", filename);
        return;
    }
}
