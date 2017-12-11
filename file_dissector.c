#include "file_dissector.h"

int main(int argc, char *argv[]) {
    FILE *binFile;
    gzip_header header;
    uint8_t byte;
    uint32_t sig1, sig2;

    /* Opening file */
    binFile=fopen(argv[1],"rb");
    if (!binFile) {
	printf("Unable to open file !\n");
	return 1;
    }

    /* searching for GZip signature at offset 0 */
    fread(&byte, sizeof(uint8_t), 1, binFile);
    if (byte == GZIP_ID_1) {
    	fread(&byte, sizeof(uint8_t), 1, binFile);
    	if (byte == GZIP_ID_2) {
		printf("GZip signature found !\n");	
		gzip_dissector(binFile);
	}
    }

    /* searching for TAR signtature at offset 0x101 */
    fseek(binFile, 0x101, SEEK_SET);
    fread(&sig1, sizeof(uint32_t), 1, binFile);
    fread(&sig2, sizeof(uint32_t), 1, binFile);

    if ((sig1 == TAR_SIG1) && ((sig2 & 0XFF) == TAR_SIG2)) {
	printf("TAR archive signature found !\n");	
    }

    fclose(binFile);

    return 0;
}

void gzip_dissector(FILE *binFile) {
    gzip_header header;
    uint16_t header_checksum;
    gzip_footer footer;
    char buffer[255];
    time_t time;
    unsigned int i;

    /* Header dissector */
    fread(&(header.compressionMethod), sizeof(uint8_t), 1, binFile);
    fread(&(header.flags), sizeof(uint8_t), 1, binFile);
    fread(&(header.lastModTime), sizeof(uint32_t), 1, binFile);
    fread(&(header.compressionFlags), sizeof(uint8_t), 1, binFile);
    fread(&(header.operatingSystem), sizeof(uint8_t), 1, binFile);
	
    time = (time_t) header.lastModTime;
    strftime(buffer, 255, "%Y-%m-%d %H:%M:%S", localtime(&time));

    printf("GZip Header :\n");
    printf("	compression method : 	%02x\n", header.compressionMethod);
    printf("	flags : 		%02x\n", header.flags);
    printf("	last mod. time :	%s\n", buffer);
    printf("	compression flags :	%02x\n", header.compressionFlags);
    printf("	operating system : 	%02x\n", header.operatingSystem);

    /* Optional header fields */
    if (header.flags & GZIP_FNAME_FLAG) {
	i = 0;

	do {
		fread(&(buffer[i]), sizeof(char), 1, binFile);
		i++;
	} while (buffer[i-1] != '0');

	printf("	file name :		%s\n", buffer);
    }


    if (header.flags & GZIP_FCOMMENT_FLAG) {
	i = 0;

	do {
		fread(&(buffer[i]), sizeof(char), 1, binFile);
		i++;
	} while (buffer[i-1] != '0');

	printf("	file comment :		%s\n", buffer);
    }

    if (header.flags & GZIP_FCOMMENT_FLAG) {
	fread(&header_checksum, sizeof(uint16_t), 1, binFile);
	printf("	header checksum :	%d", header_checksum);
    }

    /* Footer dissector */
    printf("GZip Footer :\n");

    fseek(binFile, -8, SEEK_END);
    fread(&(footer), sizeof(uint32_t), 2, binFile);

    printf("	checksum : 		%02x\n", footer.checksum);
    printf("	data size : 		%d bytes\n", footer.dataSize);
}
