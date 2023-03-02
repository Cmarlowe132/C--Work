#include <sys/socket.h>  // Unix header for sockets, using socket
#include <sys/un.h>  // defns for Unix domain sockets, using struct sockaddr_un
#include <unistd.h>  // Unix standard header, using close

#include <cassert>  // using assert
#include <cerrno>  // using errno
#include <cstddef>  // using size_t
#include <cstdlib>  // exit
#include <cstring>  // using strncpy, strerror
#include <vector>
#include <sys/mman.h>
#include <fcntl.h>
#include "shm_store.h"
#include <thread>

#include <string>
#include <iostream>


class UnixDomainSocket {
 public:
  explicit UnixDomainSocket(const char *socket_path);

 protected:
  ::sockaddr_un sock_addr_;
  std::string socket_path_;
};

class DomainSocketClient : public UnixDomainSocket {
 public:
  using UnixDomainSocket::UnixDomainSocket;
  void RunClient(std::string file_path, std::string search_string);
  ~DomainSocketClient();
  private: 
    SharedMemoryStore *store_;
};

std::vector<std::string> divideWork(std::vector<std::string> text, std::string searchFor);
const bool searchLine(const std::string line, const std::string searchString);
void findKeyword(std::vector<std::string> file, int start, int end, std::string searchString, std::vector<std::string> &outputVector);