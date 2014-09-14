#include "DirectoryMonitor.h"
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>
#include <vector>
#include <string>
#include <iostream>

using namespace std;

/*function... might want it in some class?*/
int DirectoryMonitor::getDir (string dir, vector<string> &files)
{  
    DIR *dp;
    struct dirent *dirp;

    files.clear();

    if((dp  = opendir(dir.c_str())) == NULL) {
        cout << "Error(" << errno << ") opening " << dir << endl;
        return errno;
    }

    while ((dirp = readdir(dp)) != NULL) {
        if( dirp->d_name[0] != '.' )
        {
            files.push_back(string(dirp->d_name));
        }
    }
    closedir(dp);
    return 0;
}