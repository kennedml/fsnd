#include "fsnd_utilities.h"

int main(int argc, char **argv)
{
  int rc = 0;
  if((rc = parse_args(argc, argv)))
  {
    printf("%s %d %s print_help() failed. Errno: %d. \n",
        __FILE__,__LINE__,__func__, rc);
  }
  return(0);
}

