#include "server.h"


UnixDomainSocket::UnixDomainSocket(const char *socket_path) {
    socket_path_ = std::string(socket_path);
    sock_addr_ = {}; 
    sock_addr_.sun_family = AF_UNIX;  
    strncpy(sock_addr_.sun_path + 1, socket_path, sizeof(sock_addr_.sun_path) - 2); 
  }

void DomainSocketServer::RunServer() const {
    std::string filePath, searchString = "";
    int sock_fd;
    int client_req_sock_fd;
    sock_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if ( sock_fd < 0 ) {
      std::cerr << strerror(errno) << std::endl;
      exit(-1);
    }

    unlink(socket_path_.c_str());
    int success = bind(sock_fd,reinterpret_cast<const sockaddr*>(&sock_addr_),sizeof(sock_addr_));
    if (success < 0) {
      std::cerr << strerror(errno) << std::endl;
      exit(-1);
    }

    std::clog << "SERVER STARTED" << std::endl;
    size_t kMax_client_conns = get_nprocs_conf() - 1;
    std::clog << "\tMAX CLIENTS AMOUNT: " << kMax_client_conns << std::endl;
    success = listen(sock_fd, kMax_client_conns);
    if (success < 0) {
      std::cerr << strerror(errno) << std::endl;
      exit(-1);
    }

    const size_t kRead_buffer_size = 32;
    char read_buffer[kRead_buffer_size];
    int bytes_read;
    while (true) {
      client_req_sock_fd = accept(sock_fd, nullptr, nullptr);
      if (client_req_sock_fd  < 0) {
        std::cerr << strerror(errno) << std::endl;
        continue;
      }

      std::clog << "CLIENT CONNECTED" << std::endl;
      bytes_read = read(client_req_sock_fd, read_buffer, kRead_buffer_size);
      if (bytes_read < 0) {
        std::cerr << strerror(errno) << std::endl;
        exit(-1);
      }
      std::string buildString = "";
      bool keepGoing = true;
      while (bytes_read > 0) {
        for(int i = 0; i < kRead_buffer_size; i++) {
          if(read_buffer[i] == '\037') {
            filePath = buildString;
            buildString = "";
          } else if (read_buffer[i] == '\004') {
            searchString = buildString;
            keepGoing = false; 
            break;
          }
          else {
            buildString+=read_buffer[i];
          }
        }
        if (keepGoing == false) {
          bytes_read = 0;
          break;
        }
        bytes_read = read(client_req_sock_fd, read_buffer, kRead_buffer_size);
        if (bytes_read < 0) {
          std::cerr << strerror(errno) << std::endl;
          exit(-1);
        } 
      }
      std::clog << "PATH: " << filePath << std::endl;
      std::clog << "SEEKING: " << searchString << std::endl;
      std::vector<std::string> outputStrings = searchThroughFile(filePath, searchString);
      ssize_t bytesSent = 1;
      int totalBytesSent = 0;
      int counter = 0;
      while(bytesSent > 0 && counter < outputStrings.size()) {
        std::string next = outputStrings.at(counter);
        counter++;
        ssize_t write_buffer_size = next.size()+1;
        char write_buffer[write_buffer_size];
        for(int i = 0; i < write_buffer_size-1; i++) {
          write_buffer[i] = next[i];
        }
          if(counter == outputStrings.size()) {
          write_buffer[write_buffer_size-1] = '\004';
        } else { 
          write_buffer[write_buffer_size-1] = '\037';
        }
        bytesSent = write(client_req_sock_fd, write_buffer, write_buffer_size);
        if(bytesSent < 0) {
          std::cerr << strerror(errno) << std::endl;
          exit(-1);
        }
        totalBytesSent += bytesSent;
      }
      std::clog << "BYTES SENT: " << totalBytesSent << std::endl;
      close(client_req_sock_fd);
    } 
}

std::vector<std::string> DomainSocketServer::searchThroughFile(const std::string &filePath, const std::string searchString) const{
  std::vector<std::string> returnVector; 
  std::fstream workingFile(filePath);
  if(!workingFile.is_open()) {
    std::string fileError = "INVALID FILE";
    returnVector.push_back(fileError);
    return returnVector;
  }

  std::string line;
  while(getline(workingFile, line)) {
    if(searchLine(line, searchString)) {
      returnVector.push_back(line);
    }
  }
  workingFile.close();
  return returnVector;
}

const bool DomainSocketServer::searchLine(const std::string line, const std::string searchString) const{
  if(line.find(searchString) != std::string::npos) {
    return true; 
  }
  return false; 
}


int main(int argc, char *argv[]) {
  if (argc != 2) {
    return 1;
  }
    DomainSocketServer dss(argv[1]);
    dss.RunServer();
  return 0;
}
