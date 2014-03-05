#include "myGrep.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
    // ERROR: Wrong number of arguments
    if(argc != 3) {
        printf("myGrep: improper number of arguments\n");
        printf("Usage: ./a.out <filename> <word>\n");
        return 1;
    }

    // Print the command line
    int i = 0;
    for(i = 0; i < argc; i ++) {
        printf("%s ",argv[i]);
    }
    printf("\n");

    // Open the file
    FILE *fp = fopen(argv[1], "r");
    
    // ERROR: File doesn't exist
    if(fp == NULL) {
        printf("Error: File does not exist\n");
        return 1;
    }

    // Set up the linked list
    Occurrence* head = NULL;
    Occurrence* tail;
    int occurrences = 0;

    // Start greppin'
    char* longestLine = (char *) malloc(100 * sizeof(char));
    int longestLineLength = 0;
    int totalLines = 0;
    char* line = (char *) malloc(100 * sizeof(char));
    char* currentLine = (char *) malloc(100 * sizeof(char));
    while(fgets(line, 100, fp) != NULL) {
        // Pull this line up
        int thisLen = strlen(line);
        if(thisLen > longestLineLength) {
            longestLineLength = thisLen;
            strcpy(longestLine,line);
        }
       
        // Save the full line so it doesn't get trashed
        strcpy(currentLine, line);
  
        // Start looking around 
        int wordNumber = 0;
        char* token = strtok(line, "\n ");
        while(token != NULL) {
            // Remove a possible newline from the end
            if(token[strlen(token)-1] == '\n')
                token[strlen(token)-1] = 0;

            // Is this the string you're looking for?
            if(strcmp(token, argv[2]) == 0) {
                if(head == NULL) {
                    head = (Occurrence *) malloc(sizeof(Occurrence));
                    tail = head;
                }
                else {
                    tail->next = (Occurrence *) malloc(sizeof(Occurrence));
                    tail = tail->next;
                }
                Occurrence* thisOccurrence = tail;
                thisOccurrence->line = (char *) malloc(100 * sizeof(char));
                strcpy(thisOccurrence->line, currentLine);
                thisOccurrence->lineNumber = totalLines;
                thisOccurrence->wordNumber = wordNumber; 
                occurrences ++;
            }
            wordNumber ++;
            token = strtok(NULL, " ");
        }

        totalLines ++;
    }

    printf("longest line: %s",longestLine);
    printf("num chars: %d\n",longestLineLength);
    printf("num lines: %d\n",totalLines);
    printf("total occurrences of word: %d\n",occurrences);
    while(head != NULL) {
        printf("line %d; word %d; %s", head->lineNumber, head->wordNumber, head->line);
        head = head->next;
    }
}
