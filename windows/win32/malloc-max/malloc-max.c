#include <stdlib.h>   // For _MAX_PATH definition
#include <stdio.h>
#include <errno.h>
#include <string.h>

#include "malloc.h"

#define ALLOC_SIZE 0x80000000


int main( void )
{
  char *string;

  // Allocate space for a path name
  string = dlmalloc(ALLOC_SIZE);

  // In a C++ file, explicitly cast malloc's return.  For example, 
  // string = (char *)malloc( _HEAP_MAXREQ );
  if( string == NULL ) {
    printf("strerror: %s\n", strerror(errno));
  }
  else {
    printf( "Memory space allocated for path name\n" );
    free( string );
    printf( "Memory freed\n" );
  }
}
