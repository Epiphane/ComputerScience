typedef struct Occurrence Occurrence;

struct Occurrence {
    char* line;
    int lineNumber;
    int wordNumber;
    Occurrence* next;
};
