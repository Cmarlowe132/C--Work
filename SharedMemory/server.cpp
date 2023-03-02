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

      std::clog << "CLIENT REQUEST RECEIVED" << std::endl; // STEP 2
      bytes_read = read(client_req_sock_fd, read_buffer, kRead_buffer_size); //Reads in what the client has sent
      std::string sharedMemName = "name";
      int sharedMemoryFileDescriptor = ::shm_open("place", O_RDWR, 0); //Opens the shared memory
      std::clog << "\tMEMORY OPEN" << std::endl;//STEP 3

      struct SharedMemoryStore *store = static_cast<SharedMemoryStore*>(mmap(nullptr, sizeof(SharedMemoryStore), PROT_READ | PROT_WRITE, MAP_SHARED, sharedMemoryFileDescriptor, 0));

      if(store == MAP_FAILED) {
        std::cerr << ::strerror(errno) << std::endl;
        exit;
      }
      if (bytes_read < 0) {
        std::cerr << strerror(errno) << std::endl;
        exit(-1);
      }
      std::string buildString = "";
      bool keepGoing = true;
      while (bytes_read > 0) { //Loop that reads what the client's sent file path is 
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
      std::clog << "\tOPENING: " << filePath << std::endl; //STEP 4 Part 1
      std::string outputString = readFile(filePath, searchString); //Reads the file and turns it into one long string STEP 4 part 2
    
      int write_buffer_size = 1; 
      char write_buffer[write_buffer_size];
      write_buffer [0] = 'w';
      int counter = 0;
      const size_t kSharedMemBufferSize = 4096 - 2*sizeof(size_t);
      store->stringLength = outputString.length();
      bool continueOn = true; 
      while(continueOn) { //Writes as much of the file String into the buffer, signals to the client the buffer is full, waits for client response. Continues on until it reaches the end of the String
        //this loop contains STEP 4 part 3
        for (int i = 0; i < kSharedMemBufferSize; i++) {
          if(i+counter < outputString.length()-1) {
            store->buffer[i] = outputString.at(i+counter);
          }
          else {
            store->buffer[i] = outputString.at(i+counter);
            continueOn = false;
            break;
          }
        }

        counter+=kSharedMemBufferSize;
        int bytesSent = write(client_req_sock_fd, write_buffer, write_buffer_size);
        if(bytesSent < 0) {
          std::cerr << strerror(errno) << std::endl;
          exit(-1);
        }
        bytes_read = read(client_req_sock_fd, read_buffer, kRead_buffer_size);
        if (bytes_read < 0) {
          std::cerr << strerror(errno) << std::endl;
          exit(-1);
        } 
      }
      shm_unlink("place"); //unlinks the shared memory
      int result = ::munmap(store, sizeof(SharedMemoryStore));
      if (result < 0) {
        exit;
      }
      close(client_req_sock_fd);
      std::clog << "\tMEMORY CLOSED" << std::endl; //Step 4 Part 5
    } 
}

std::string DomainSocketServer::readFile(const std::string &filePath, const std::string searchString) const{ //Reads the file, turning it into a string
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
  std::clog << "\tFILE CLOSED" << std::endl; //STEP 4 part 4
  return returnString;
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
