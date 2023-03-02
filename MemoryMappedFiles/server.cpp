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
    sock_fd = socket(AF_UNIX, SOCK_STREAM, 0); //Creates unnamed socket
    if ( sock_fd < 0 ) {
      std::cerr << strerror(errno) << std::endl;
      exit(-1);
    }

    unlink(socket_path_.c_str()); 
    //binds the socket, starting up the server
    int success = bind(sock_fd,reinterpret_cast<const sockaddr*>(&sock_addr_),sizeof(sock_addr_));
    if (success < 0) {
      std::cerr << strerror(errno) << std::endl;
      exit(-1);
    }

    std::cout << "SERVER STARTED" << std::endl; //STEP 1
    size_t kMax_client_conns = get_nprocs_conf() - 1;
    success = listen(sock_fd, kMax_client_conns); //Begins listening for connections
    if (success < 0) {
      std::cerr << strerror(errno) << std::endl;
      exit(-1);
    }

    const size_t kRead_buffer_size = 32;
    char read_buffer[kRead_buffer_size];
    int bytes_read;
    while (true) {
      client_req_sock_fd = accept(sock_fd, nullptr, nullptr); //Accepts client 
      if (client_req_sock_fd  < 0) {
        std::cerr << strerror(errno) << std::endl;
        continue;
      }

      std::clog << "CLIENT REQUEST RECEIVED" << std::endl; //STEP 2 Part a
      bytes_read = read(client_req_sock_fd, read_buffer, kRead_buffer_size);
    
      if (bytes_read < 0) {
        std::cerr << strerror(errno) << std::endl;
        exit(-1);
      }
      std::string buildString = "";
      bool keepGoing = true;
      while (bytes_read > 0) { 
        for(int i = 0; i < kRead_buffer_size; i++) {
        if (read_buffer[i] == '\004') {
            filePath = buildString;
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
      std::clog << "\tOPENING: " << filePath << std::endl; //Step 2 part b (i)
      int fileDescriptor;
      long int fileSize;
      char filePathCharArr[filePath.length()];
      strcpy(filePathCharArr, filePath.c_str());
      bool canOpen = true;
      std::tie(fileDescriptor, fileSize) = OpenFile(filePathCharArr, O_RDWR, canOpen);//STEP 2 part b (ii)
      if(!canOpen) {
        int write_buffer_size = 1; 
        char write_buffer[write_buffer_size];
        write_buffer [0] = 'f';
        int bytesSent = write(client_req_sock_fd, write_buffer, write_buffer_size);
      }
      else {
        char *buf_file_addr = static_cast<char *>(::mmap(nullptr, fileSize, PROT_READ | PROT_WRITE, MAP_SHARED, fileDescriptor, 0));//STEP 2 part b (iii)
        if(buf_file_addr == MAP_FAILED) {
          HandleError("Open File Map Fail");
        }
        else{
          std::clog << "\tFILE MAPPED TO SHARED MEMORY" <<std::endl; //STEP 2 Part b (iii) bullet point 1
        }
        if(::close(fileDescriptor) < 0) {
          HandleError("File Map Close Fail");
        }
        else {
          std::clog << "\tFILE CLOSED" << std::endl; //STEP 2 part b (iii) bullet point 2
        }
    
        int write_buffer_size = 1; 
        char write_buffer[write_buffer_size];
        write_buffer [0] = 'w';
        int bytesSent = write(client_req_sock_fd, write_buffer, write_buffer_size);
        if(bytesSent < 0) {
          std::cerr << strerror(errno) << std::endl;
          exit(-1);
        }
        if(::munmap(buf_file_addr, fileSize)) {
          HandleError("File Memory Unmap");
        }
      }
      close(client_req_sock_fd); //STEP 3, closes the client's connect and goes back to waiting for the next
    }
}

std::string DomainSocketServer::readFile(const std::string &filePath, const std::string searchString) const{
  std::string returnString = "";
  std::fstream workingFile(filePath);
  if(!workingFile.is_open()) {
    std::string fileError = "INVALID FILE ";
    returnString.append(fileError);
    workingFile.close();
    std::clog << "\tFILE CLOSED" << std::endl;
    return returnString;
  }

  std::string line;
  while(getline(workingFile, line)) {
      returnString.append(line);
      returnString.append("\n");
  }
  workingFile.close();
  std::clog << "\tFILE CLOSED" << std::endl; 
  return returnString;
}

std::tuple<int, off_t> OpenFile(const char file_name[], int flags, bool &canOpen) {
  // open and capture file descriptor
  int fd = ::open(file_name, flags);
  if (fd < 0) {
    canOpen = false;
    return {0,0};
  }
  // get size of file
  struct ::stat f_stats;
  if(::fstat(fd, &f_stats) < 0) {
    std::cout << "FILE ERROR" << std::endl;
  } 
  return {fd, f_stats.st_size};
}

void HandleError(const char msg[]) {
  std::cerr << "Source: " << msg << std::endl;
  std::cerr << '\t' << ::strerror(errno) << std::endl;
}

int main(int argc, char *argv[]) {
  if (argc != 1) {
    return 1;
  }
    std::string domainSocketName = "place";
    char char_array[domainSocketName.length()+1];
    strcpy(char_array, domainSocketName.c_str());
    DomainSocketServer dss(char_array);
    dss.RunServer(); //This proces keeps running until forcibly stopped, making this STEP 5
  return 0;
}
