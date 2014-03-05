#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

/*
void InsertSort(int vals[], int size) {
   int ndx, insNdx, toInsert;

   for (ndx = 1; ndx < size; ndx++) {
      toInsert = vals[insNdx = ndx];
      while (insNdx > 0 && toInsert < vals[insNdx-1]) {
         vals[insNdx] = vals[insNdx-1];
         insNdx--;
      }
      vals[insNdx] = toInsert;
   }
}
*/

void InsertSort(int fd) {
   size_t ndx, limit;
   int toInsert, temp;   

   limit = lseek(fd, 0, SEEK_END);
   for(ndx = sizeof(int); ndx < limit; ndx += sizeof(int)) {
      lseek(fd, ndx, SEEK_SET);
      
      read(fd, &toInsert, sizeof(int));
      lseek(fd, -sizeof(int), SEEK_CUR);

      while(lseek(fd, -sizeof(int), SEEK_CUR) >= 0 && read(fd, &temp, sizeof(int)) && toInsert < temp) {
         write(fd, &temp, sizeof(int));
         lseek(fd, -2 * sizeof(int), SEEK_CUR);
      }
      write(fd, &toInsert, sizeof(int));
   }
}

int main() {
   int fd, data;

   fd = open("temp.txt", O_CREAT|O_RDWR|O_TRUNC, 0600);
   while (EOF != scanf("%d", &data))
      write(fd, &data, sizeof(int));

   InsertSort(fd);

   lseek(fd, 0, SEEK_SET);
   while (0 != read(fd, &data, sizeof(int)))
      printf("%d ", data);
   printf("\n");

   return 0;
}
