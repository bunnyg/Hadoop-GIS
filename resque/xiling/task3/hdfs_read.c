#include "hdfs.h"

int main(int argc, char **argv) {
    if (argc != 4) {
        fprintf(stderr, "Usage: hdfs_read \n");
        exit(-1);
    }

    hdfsFS fs = hdfsConnect("default", 0);
    if (!fs) {
        fprintf(stderr, "Oops! Failed to connect to hdfs!\n");
        exit(-1);
    }

    const char* rfile = argv[1];
    tSize fileTotalSize = strtoul(argv[2], NULL, 10);
    tSize bufferSize = strtoul(argv[3], NULL, 10);

    hdfsFile readFile = hdfsOpenFile(fs, rfile, O_RDONLY, bufferSize, 0, 0);
    if (!readFile) {
        fprintf(stderr, "Failed to open %s for writing!\n", rfile);
        exit(-2);
    }

    // data to be written to the file
    char* buffer = malloc(sizeof(char) * bufferSize);
    if(buffer == NULL) {
        return -2;
    }

    // read from the file
    tSize curSize = bufferSize;
    for (; curSize == bufferSize;) {
        curSize = hdfsRead(fs, readFile, (void*)buffer, curSize);
    }

    free(buffer);
    hdfsCloseFile(fs, readFile);
    hdfsDisconnect(fs);

    return 0;
}                                                      
