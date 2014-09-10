#ifndef DIRECTORYMONITOR_H
#define DIRECTORYMONITOR_H

#include <vector>
#include <string>

class DirectoryMonitor
{
	public:
		int getDir(std::string dir, std::vector<std::string> &files);
};

#endif