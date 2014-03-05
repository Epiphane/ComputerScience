#define BUF_SIZE 16   // Why #defines?
#define NUM_FILES 10
#define FILE_READ 0x1
#define FILE_ATEOF 0x2
#define EOF -1

typedef struct {
   int fd;                // File descriptor, or -1 if this FILE is unused
   char *next;            // Next byte to read or write -- walks thru buffer
   int flags;             // Flags for read status, and eof status
   int bufSize;           // Number of bytes of buffer that are usable
   char buffer[BUF_SIZE]; // Buffer to read from or write into
} ALT_FILE;

extern ALT_FILE altFiles[];

extern ALT_FILE *altStdin;
extern ALT_FILE *altStdout;
extern ALT_FILE *altStderr;

int altPutc(char, ALT_FILE *fp);
int altGetc(ALT_FILE *);
int altFflush(ALT_FILE *fp);

#define altPutchar(c) (altPutc((c), altStdout))
#define altGetchar()  (altGetc(altStdin))
#define altUngetc(c, fp) ((fp)->next > (fp)->buffer && (c) != EOF ? \
 *--(fp)->next = (c) : EOF)
