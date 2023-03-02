#include <sys/socket.h>  // Unix header for sockets, using socket
#include <sys/un.h>  // defns for Unix domain sockets, using struct sockaddr_un
#include <unistd.h>  // Unix standard header, using close
#include <sys/sysinfo.h>

#include <cassert>  // using assert
#include <cerrno>  // using errno
#include <cstddef>  // using size_t
#include <cstdlib>  // exit
#include <cstring>  // using strncpy, strerror
#include <vector> 
#include <fstream>
#include <sys/mman.h>
#include <fcntl.h>
#include <tuple>
#include <sys/stat.h>

#include <string>
#include <iostream>

class UnixDomainSocket {
 public:
  explicit UnixDomainSocket(const char *socket_path);

 protected:
  ::sockaddr_un sock_addr_;  // socket address from sys/un.h
  std::string socket_path_;  // let std::string manage char *
};

class DomainSocketServer : public UnixDomainSocket {
 public:
  using ::UnixDomainSocket::UnixDomainSocket;
  void RunServer() const;
  const bool searchLine(const std::string line, const std::string searchString) const;
  std::string searchThroughFile(const std::string &filePath, const std::string searchString) const;
  std::string readFile(const std::string &filePath, const std::string searchString) const;
  private:
};

std::tuple<int, off_t> OpenFile(const char file_name[], int flags, bool&);
void HandleError(const char msg[]);
