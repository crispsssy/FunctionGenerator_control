#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cerrno>
#include <cstring>
#include <fstream>

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>

char const* host_ip("192.169.10.55");
int const portNum = 5025;

int SocketConnect();
void SocketQuery(int socketOsc, char const* cmd);
void SendCommand(int socketOsc, char const* cmd);
std::string TranslateCommand(int ch, std::string mode, std::string cmd);
std::string TranslateCommand(int ch, std::string mode, std::string cmd, std::string parameter);
