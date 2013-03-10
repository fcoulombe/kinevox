/*
 * Copyright (C) 2011 by Francois Coulombe
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:

 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.

 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
#ifdef OS_WIN32
#include <windows.h>
#include <iostream>
extern int main(int argc, char **argv);


static void UnEscapeQuotes( char *arg )
{
    char *last = NULL;

    while( *arg ) 
    {
        if( *arg == '"' && *last == '\\' ) 
        {
            char *c_curr = arg;
            char *c_last = last;

            while( *c_curr ) 
            {
                *c_last = *c_curr;
                c_last = c_curr;
                c_curr++;
            }
            *c_last = '\0';
        }
        last = arg;
        arg++;
    }
}
/* Parse a command line buffer into arguments */
static int ParseCommandLine(char *cmdline, char **argv)
{
    char *bufp;
    char *lastp = NULL;
    int argc, last_argc;

    argc = last_argc = 0;
    for ( bufp = cmdline; *bufp; ) 
    {
        /* Skip leading whitespace */
        while ( isspace(*bufp) )
        {
            ++bufp;
        }
        /* Skip over argument */
        if ( *bufp == '"' ) 
        {
            ++bufp;
            if ( *bufp ) 
            {
                if ( argv )
                {
                    argv[argc] = bufp;
                }
                ++argc;
            }
            /* Skip over word */
            while ( *bufp && ( *bufp != '"' || (lastp && *lastp == '\\') ) ) 
            {
                lastp = bufp;
                ++bufp;
            }
        } 
        else
        {
            if ( *bufp )
            {
                if ( argv ) 
                {
                    argv[argc] = bufp;
                }
                ++argc;
            }
            /* Skip over word */
            while ( *bufp && ! isspace(*bufp) ) 
            {
                ++bufp;
            }
        }
        if ( *bufp ) 
        {
            if ( argv ) 
            {
                *bufp = '\0';
            }
            ++bufp;
        }

        /* Strip out \ from \" sequences */
        if( argv && last_argc != argc ) 
        {
            UnEscapeQuotes( argv[last_argc] );	
        }
        last_argc = argc;	
    }
    if ( argv )
    {
        argv[argc] = NULL;
    }
    return(argc);
}
int WINAPI WinMain(HINSTANCE /*hInst*/, HINSTANCE /*hPrev*/, LPSTR /*szCmdLine*/, int /*sw*/)
{
    std::cout << "start"<< std::endl;
    char **argv=NULL;
    int argc;
    char *cmdline;
    size_t nLen;
    std::cout << "parse";
    cmdline = GetCommandLine();
    nLen = strlen(cmdline);
    argc = ParseCommandLine(cmdline, NULL);

    argv = new char*[argc+1];
    ParseCommandLine(cmdline, argv);
    std::cout <<"end parse";
    int ret = main(argc, argv); 
    delete [] argv;
    return ret;

}
#endif