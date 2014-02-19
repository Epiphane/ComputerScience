#ifndef COUNTER
#define COUNTER
int counter = -1;
#endif

#ifndef PRINT
#define PRINT 1
counter += 2;
#else
#undef PRINT
#endif
