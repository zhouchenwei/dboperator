#pragma once
#include <map>
#include <string>
#include <list>
#include <vector>
#include <set>
#include <unordered_map>
#include <thread>
#include <memory>
#include <mutex>
#include <sstream>
#include <fstream>
#include <atomic>
#include <future>
#include <iostream>
using namespace std;

#include <string.h>
#include <stdio.h>
#include <stdlib.h>


#ifdef _WIN32 
#include <winsock2.h>
#include <tchar.h>
#pragma comment(lib,"ws2_32.lib")
typedef SOCKET socket_t;
#else
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <sys/resource.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <dlfcn.h>
#include <libgen.h>
#include <errno.h>
#include<iconv.h>
#include <dirent.h>
#include <fnmatch.h>
typedef int socket_t;

#endif // _WIN32 