#include <stdio.h>
#include <stdarg.h>
#include "uPrintf.h"

typedef char 			int8;
typedef unsigned char 	uint8;
typedef short 			int16;
typedef unsigned short 	uint16;
typedef int 			int32;
typedef unsigned 		uint32;

static const char hex_table[] = "0123456789ABCDEF";

void
uPrintfPutChar(
	char 		c
	);

static pfuPrintfPutChar_T gpfuPrintfPutChar = uPrintfPutChar;

void
uPrintfSetPutChar(
    pfuPrintfPutChar_T pf
    )
{
    gpfuPrintfPutChar = pf;
}

void
uPrintfPutChar(
    char         c
    )
{
}

int 
uPrintfStrLen(
    const char   *s
    )
{
    int i;

    for (i = 0; *s; i = i + 1)
    {
        s = s + 1;
    }

    return i;
}

void
uPrintfPutStringWithPad(
    const char   *s,
    int16        npad,
    int16        cpad
    )
{
    /* Pad the string if requested */
    if (npad)
    {
        int len = uPrintfStrLen(s);

        if (npad > len)
        {
            npad = npad - len;
        
            for ( ; npad; npad = npad - 1)
            { 
                gpfuPrintfPutChar(cpad);
            }
        }
    }

    for ( ; *s; s = s + 1)
    {
        gpfuPrintfPutChar(*s);
    }
}

void
uPrintfPutLong(
	uint32   Number,
	int16    Radix,
    int16    npad,
    int16    cpad
	)
{
	char szBuff[16];
	char *p;
    int len;

    /* We only support two radix values */
   	if (Radix == 10 || Radix == 16 || Radix == -10)
	{
        p = szBuff;

        /* Are we dealing with a signed value */
        if (Radix == -10)
        {
            Radix = 10;

            if (0 > (int32)Number)
            {
                Number = -((int32)Number);
                gpfuPrintfPutChar('-');
            }
        }

	    /* The buffer is being reversed filled. */
		do 
        {
			*p = hex_table[Number % Radix];

            p = p + 1;

			Number = Number / Radix;
			
		} while (Number);

        /* Output pad if needed */		
        if (npad)
        {
            len = p - szBuff;

            if (npad > len)
            {
                npad = npad - len;;
            
                for ( ; npad; npad = npad - 1)
                { 
                    gpfuPrintfPutChar(cpad);
                }
            }
        }

	    /* Output the string in correct order */
		for ( ; p != szBuff; )
		{
            p = p - 1;

            gpfuPrintfPutChar(*p);
        }
	}
}

/* 
 * %[0|' '][pad]specifier 
 * 
 * Supported options are:
 * %c   - character 
 * %s   - string 
 * %p   - pointer, always prefix with 0x
 * %u   - unsigned decimal integer 
 * %d   - signed decimal integer 
 * %x   - unsigned hex integer upper case
 * %lu  - signed decimal long integer
 * %ld  - signed decimal long integer
 * %lx  - unsigned hex long integer uppper case 
 * %02x - pad up to 2 with leading zeros
 * % 2d - pad up to 2 with leading space
 */
void
uPrintf(
    const char   *fmt, 
    ... 
    )
{
    int16 npad;
    int16 cpad;
	va_list args;
	
	va_start(args, fmt);

    for ( ; *fmt; )
    {
        npad = 0;
        cpad = ' ';

        if (*fmt == '%')
        {
            fmt = fmt + 1;
            
            if (*fmt == ' ' || *fmt == '0')
            {
                cpad = *fmt;

                fmt = fmt + 1;
			}
    
            for ( ; *fmt && (*fmt >= '0') && (*fmt <= '9'); fmt = fmt + 1)
            {   
 	        	npad = npad * 10;
                npad = npad + *fmt - '0';
            }

            switch(*fmt)
            {
            case '%':
                gpfuPrintfPutChar(*fmt);
                break;

            case 'c':
                gpfuPrintfPutChar(va_arg(args, int));
                break;

            case 's':
                uPrintfPutStringWithPad(va_arg(args, char *), npad, cpad);
                break;

            case 'p':
                uPrintfPutStringWithPad("0x", 0, '0');
                uPrintfPutLong((size_t)va_arg(args, char *), 16, sizeof(size_t)*2, '0');
                break;

            case 'u':
                uPrintfPutLong(va_arg(args, int), 10, npad, cpad);
                break;

            case 'd':
                uPrintfPutLong(va_arg(args, int), -10, npad, cpad);
                break;

            case 'x':
            case 'X':
                uPrintfPutLong(va_arg(args, int), 16, npad, cpad);
                break;

            case 'l':

                fmt = fmt + 1;

                switch(*fmt)
                {
                case 'u':
                    uPrintfPutLong(va_arg(args, unsigned long), 10, npad, cpad);
                    break;

                case 'd':
                    uPrintfPutLong(va_arg(args, long), -10, npad, cpad);
                    break;

                case 'x':
                case 'X':
                    uPrintfPutLong(va_arg(args, unsigned long), 16, npad, cpad);
                    break;

                case 0:
                    break;

                default:
                    break;
                }
                break;

			case 0:
				break;

            default:
                break;
            }
        }
        else
        {
            gpfuPrintfPutChar(*fmt);
        }

		if (*fmt)
		{
			fmt = fmt + 1;	
		}
    }

	va_end(args);
}
