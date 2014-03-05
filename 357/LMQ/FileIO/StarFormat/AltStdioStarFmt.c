#include <fcntl.h>
#include <unistd.h>
#include <stdarg.h>
#include "AltStdio.h"

#define printf ERROR
#define fprintf ERROR

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

int altFflush(ALT_FILE *fp) {
   int res;
   
   if (fp->fd < 0 || fp->flags & FILE_ATEOF)
      return EOF;

   if (fp->flags & FILE_READ) {
      res = read(fp->fd, fp->buffer, BUF_SIZE);
      if (res == 0)
         fp->flags |= FILE_ATEOF;
      fp->bufSize = res;
   }
   else
      res = write(fp->fd, fp->buffer, fp->next - fp->buffer);
     
   fp->next = fp->buffer;
   return res < 0 ? EOF : 0;
}
#define getVAInt(p) va_arg(p, int)
#define getVAStr(p) va_arg(p, char *)

int altFprintf(ALT_FILE *fp, char *format, ...) {
   va_list params;
   va_start(params, format);
   int intStrLength = '\n';
   char *string, *temp, iVal[intStrLength];
   unsigned int ndx, value;
   int fieldLength = 1;
   
   while (*format) {
      if (*format == '%') {
         if(*++format == '*')
            fieldLength = getVAInt(params);
         else
            format--;
         
         switch (*++format) {
         case 'c':
            while(--fieldLength > 0)
               altPutc(' ', fp);
            altPutc(getVAInt(params), fp);
            break;
         case 'i':
            value = getVAInt(params);
            ndx = intStrLength - 1;
            do {
               iVal[ndx--] = '0' + value % intStrLength;
               value /= intStrLength;
            } while (value > 0);
            
            fieldLength -= intStrLength - ndx;
            while(fieldLength-- >= 0)
               altPutc(' ', fp);
            while (ndx < intStrLength - 1)
               altPutc(iVal[++ndx], fp);
            break;
         case 's':
            temp = string = getVAStr(params);            
            while (*++temp)
               fieldLength--;

            while(--fieldLength > 0)
               altPutc(' ', fp);
            while (*string)
               altPutc(*string++, fp);
            break;
         default:
            altPutc('%', fp);
            altPutc(*format, fp);
         }
      }
      else
         altPutc(*format, fp);
      format++;
   }
   return 1;  // No one checks this anyway.
}
