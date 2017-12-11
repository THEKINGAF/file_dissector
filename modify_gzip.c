#include "modify_gzip.h"

int main(int argc, char *argv[]) {
    FILE *binFile;
    uint32_t dataSize;
    dataSize = strtoul(argv[2], NULL, 10);

    /* Opening file */
    binFile=fopen(argv[1],"r+");
    if (!binFile) {
	printf("Unable to open file !\n");
	return 1;
    }

    /* Modifying last 4 bytes of the file */
    if (fseek(binFile, -4, SEEK_END))
	printf("fseek error !!\n");
    else
	fwrite(&dataSize, sizeof(uint32_t), 1, binFile);

    fclose(binFile);

    return 0;
}
