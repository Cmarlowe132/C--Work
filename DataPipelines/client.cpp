#include "client.h"

UnixDomainSocket::UnixDomainSocket(const char *socket_path) {
    socket_path_ = std::string(socket_path);
    sock_addr_ = {};
    sock_addr_.sun_family = AF_UNIX; 
    strncpy(sock_addr_.sun_path + 1, socket_path, sizeof(sock_addr_.sun_path) - 2);
  }


void DomainSocketClient::RunClient(std::string file_path, std::string search_string) {
  // (1) open nameless Unix socket
  int socket_fd = socket(AF_UNIX, SOCK_STREAM, 0);
  if (socket_fd < 0) {
    std::cerr << strerror(errno) << std::endl;
    exit(-1);
  }

  // (2) connect to an existing socket
  int success = connect(socket_fd, reinterpret_cast<const sockaddr*>(&sock_addr_), sizeof(sock_addr_));
  if (success < 0) {
    std::cerr << strerror(errno) << std::endl;
    exit(-1);
  }
  std::clog<< "SERVER CONNECTION ACCEPTED" << std::endl;

  // (3) write to socket
  char file_path_char[file_path.size()+1];
  for(int i = 0; i < file_path.size(); i++) {
    file_path_char[i] = file_path[i];
  } 

  file_path_char[file_path.size()] = '\037';
  char search_string_char[search_string.size()+1];
  
  for(int i = 0; i < search_string.size(); i++) {
    search_string_char[i] = search_string[i];
  }

  search_string_char[search_string.size()] = '\004';
  search_string+=4;
  const ssize_t kWrite_buffer_size = sizeof(search_string_char) + sizeof(file_path_char);
  char write_buffer[kWrite_buffer_size];
  
  for(int i = 0; i < file_path.length()+1; i++) {
    write_buffer[i] = file_path_char[i];
  }
  
  for(int i = 0; i < search_string.length()+1; i++) {
    write_buffer[i+file_path.length()+1] = search_string_char[i];
  }
  ssize_t bytes_wrote = write(socket_fd, write_buffer, kWrite_buffer_size);
  
  if (bytes_wrote < 0) {
    std::cerr << strerror(errno) << std::endl;
    exit(-1);
  }  

  if (bytes_wrote == 0) {
    std::clog << "Server dropped connection!" << std::endl;
    exit(-2);
  }

  std::vector<std::string> outputVector;
  int totalBytesReceived = 0;
  const int kRead_buffer_size = 32;
  char read_buffer[kRead_buffer_size];
  int bytes_read = read(socket_fd, read_buffer, kRead_buffer_size);
  std::string buildString = "";
  bool keepGoing = true;
  
  while (bytes_read > 0) {
    totalBytesReceived += bytes_read;
    
    for(int i = 0; i < kRead_buffer_size; i++) {
      if(read_buffer[i] == '\037') {
        outputVector.push_back(buildString);
        buildString = "";
      } else if (read_buffer[i] == '\004') {
        outputVector.push_back(buildString);
        keepGoing = false; 
        break;
      } else {
        buildString+=read_buffer[i];
      }
    }
    
    if(keepGoing=false) {
      break;
    }
    
    bytes_read = read(socket_fd, read_buffer, kRead_buffer_size);
  }
  
  for(int i = 0; i < outputVector.size(); i++) {
    std::cout << i+1 << "\t" << outputVector.at(i) << std::endl;
  }
  
  std::clog << "BYTES RECEIVED: " << totalBytesReceived << std::endl;
}

int main(int argc, char *argv[]) {
  if (argc != 4) {
    std::cout<< "not enough arguments" << std::endl;
    return 1;
  }
    
  DomainSocketClient dsc(argv[1]);
  dsc.RunClient(argv[2],argv[3]);
  return 0;
}