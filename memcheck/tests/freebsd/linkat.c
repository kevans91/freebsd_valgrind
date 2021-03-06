/*
 * Test this family of functions
 * linkat, unlinkat
 */

#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
    char buff[64];
    char tmpfromfile[] = "/tmp/testlinkat1.XXXXXX";
    char tmptolink[] = "/tmp/testlinkat2.XXXXXX";
    int tmpfd = mkstemp(tmpfromfile);
    mktemp(tmptolink);

    memset(buff, 0, sizeof(buff));
    sprintf(buff, "some data");
    write(tmpfd, buff, strlen(buff)+1);
    close (tmpfd);
    
    DIR* tmpdir = opendir("/tmp");
    if (tmpdir) {
        int tmpdirfd = dirfd(tmpdir);
    
        const char* from = tmpfromfile+5;
        const char* to = tmptolink+5;
        
       if (-1 == linkat(tmpdirfd, from, tmpdirfd, to, 0)) {
           perror("linkat failed");
       }

       unlinkat(tmpdirfd, to, 0);
       
       // now some errors
       char* badstring = strdup(from);
       free(badstring);
       linkat(tmpdirfd, badstring, tmpdirfd, to, 0);
       unlinkat(tmpdirfd, to, 0);
       
       badstring = strdup(to);
       free(badstring);
       linkat(tmpdirfd, from, tmpdirfd, badstring, 0);
       unlinkat(tmpdirfd, badstring, 0);
       
       int uninit;
       linkat(uninit, from, uninit, to, uninit);
       unlinkat(uninit, "dontcare", uninit);

       closedir(tmpdir);
    }
    
    unlink(tmpfromfile);
}
