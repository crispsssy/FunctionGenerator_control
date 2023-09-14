/***************************************
*                                      *
* Siglent SDG6032X function generator  *
* Control software                     *
*                                      *
* 2023/09/10 Siyuan Sun                *
* use standard c++ compiler to compile *
*                                      *
***************************************/

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cerrno>
#include <cstring>
#include <fstream>
#include <sstream>

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>

char const* host_ip("192.169.10.55");
int const portNum = 5025;

int SocketConnect();
void SocketQuery(int socketOsc, std::string const& cmd);
void SendCommand(int socketOsc, std::string const& cmd);
std::string TranslateCommand(int ch, std::string const& mode, std::string const& cmd);
std::string TranslateCommand(int ch, std::string const& mode, std::string const& cmd, std::string const& parameter);
