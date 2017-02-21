#include <unistd.h>
#include <stdlib.h>

int main( int argc, char **argv )
{
    char ch;
    while( read( STDIN_FILENO, &ch, 1 ) > 0 ) {
        write( STDOUT_FILENO, &ch, 1 );
    }
}
