#include <fcntl.h>
#include <unistd.h>
#include "AltStdio.h"

ALT_FILE altFiles[NUM_FILES] = {
   {0, altFiles[0].buffer, FILE_READ, 0}, {1, altFiles[1].buffer, 0, BUF_SIZE},
   {2, altFiles[2].buffer, 0, 1}, {-1}, {-1}, {-1}, {-1}, {-1},
   {-1}, {-1}
};

ALT_FILE *altStdin = altFiles+0;
ALT_FILE *altStdout = altFiles+1;
ALT_FILE *altStderr = altFiles+2;

int altPutc(char c, ALT_FILE *fp) {
   *fp->next++ = c;
   if (fp->next == fp->buffer + fp->bufSize)
      altFflush(fp);
}

int altGetc(ALT_FILE *fp) {
   if (fp->next == fp->buffer + fp->bufSize)
      altFflush(fp);
     
   return fp->flags & FILE_ATEOF ? EOF : *fp->next++;
}

int altFflush(ALT_FILE *fp) {
   int res;
   
   if (fp->fd < 0 || fp->flags & FILE_ATEOF)
      return EOF;

   if (fp->flags & FILE_READ) {
      //if(fp->next > fp->buffer + 2) {
         fp->next = fp->buffer + 2;
         fp->bufSize = 2;
      //}
      fp->bufSize += res = read(fp->fd, fp->next, fp->buffer + BUF_SIZE - fp->next);
      if (!res)
         fp->flags |= FILE_ATEOF;
   }
   else {
      res = write(fp->fd, fp->buffer, fp->next - fp->buffer);
      fp->next = fp->buffer;
   }

   return res < 0 ? EOF : 0;
}
