#include <stdio.h>
#include <time.h>
#include <inttypes.h>

#define GZIP_ID_1 0x1f
#define GZIP_ID_2 0x8b
#define GZIP_FTEXT_FLAG 0x01
#define GZIP_FHCRC_FLAG 0x02
#define GZIP_FEXTRA_FLAG 0x04
#define GZIP_FNAME_FLAG 0x08
#define GZIP_FCOMMENT_FLAG 0x10
#define TAR_SIG1 0x61747375
#define TAR_SIG2 0x72

struct gzip_header {
    uint8_t 	compressionMethod;
    uint8_t 	flags;
    uint32_t 	lastModTime;
    uint8_t 	compressionFlags;
    uint8_t 	operatingSystem;
};
typedef struct gzip_header gzip_header;

struct gzip_footer {
    uint32_t 	checksum;
    uint32_t 	dataSize;
};
typedef struct gzip_footer gzip_footer;


void gzip_dissector(FILE *binFile);
