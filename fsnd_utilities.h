#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
/******************************************************************************

				Global Variables

******************************************************************************/

extern bool verbose;
extern bool listen;
extern int port;
extern int bytes;
extern int offset;

typedef enum
{
    HELP='h',
    VERBOSE='v',
    PORT='p',
    BYTES='n',
    OFFSET='o',
    LISTEN='l'
} flags;


/******************************************************************************

				   Functions

******************************************************************************/
int parse_args(int argc, char **argv);
int print_help();
