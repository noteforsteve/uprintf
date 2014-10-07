/*
 * uPrintf
 */
#ifndef UPRINTF_H
#define UPRINTF_H

typedef 
void 
(*pfuPrintfPutChar_T)(
    char c
    );

void
uPrintfSetPutChar(
    pfuPrintfPutChar_T pfPutChar
    );
    
void
uPrintf(
    const char   *fmt, 
    ... 
    );

#endif

