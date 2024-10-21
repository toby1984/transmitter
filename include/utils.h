#ifndef MYUTILS_H
#define MYUTILS_H

// function that takes no arguments and returns nothing
typedef void (*Runnable)(void);

// macros to allow printing values of macros during preprocessing by doing
// #pragma message "The value of ABC: " XSTR(ABC)
// see https://stackoverflow.com/questions/1562074/how-do-i-show-the-value-of-a-define-at-compile-time
#define XSTR(x) STR(x)
#define STR(x) #x

#endif
