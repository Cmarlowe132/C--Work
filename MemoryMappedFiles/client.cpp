#include "client.h"

UnixDomainSocket::UnixDomainSocket(const char *socket_path) {
    socket_path_ = std::string(socket_path);
    sock_addr_ = {};
    sock_addr_.sun_family = AF_UNIX; 
    strncpy(sock_addr_.sun_path + 1, socket_path, sizeof(sock_addr_.sun_path) - 2);
  }


void DomainSocketClient::RunClient(std::string file_path) {
  // open nameless Unix socket
  int socket_fd = socket(AF_UNIX, SOCK_STREAM, 0);
  if (socket_fd < 0) {
    std::cerr << strerror(errno) << std::endl;
    exit(-1);
  }
   
  //  connect to an existing socket
  int success = connect(socket_fd, reinterpret_cast<const sockaddr*>(&sock_addr_), sizeof(sock_addr_));
  if (success < 0) {
    std::cerr << strerror(errno) << std::endl;
    exit(-1);
  }

  char file_path_char[file_path.size()+1];
  for(int i = 0; i < file_path.size(); i++) {
    file_path_char[i] = file_path[i];
  } 

  file_path_char[file_path.size()] = '\004';

  const ssize_t kWrite_buffer_size = sizeof(file_path_char);
  char write_buffer[kWrite_buffer_size];
  
  for(int i = 0; i < file_path.length()+1; i++) {
    write_buffer[i] = file_path_char[i];
  }
  
  ssize_t bytes_wrote = write(socket_fd, write_buffer, kWrite_buffer_size); //STEP 1 Passes the filepath to the server
  if (bytes_wrote < 0) {
    std::cerr << strerror(errno) << std::endl;
    exit(-1);
  }  

  if (bytes_wrote == 0) {
    std::clog << "Server dropped connection!" << std::endl;
    exit(-2);
  }

  int totalBytesReceived = 0;
  const int kRead_buffer_size = 32;
  char read_buffer[kRead_buffer_size];
  int bytes_read = read(socket_fd, read_buffer, kRead_buffer_size);
  if(read_buffer[0] == 'f') {
    std::cerr << "INVALID FILE" << std::endl; //STEP 3 If the server cannot open the file, it will be written here
  }

  int fileDescriptor;
  long int fileSize;
  char filePathArr[file_path.length()+1];
  strcpy(filePathArr, file_path.c_str());
  bool canOpen = true;
  std::tie(fileDescriptor, fileSize) = OpenFile(filePathArr, O_RDWR, canOpen);
  if(canOpen) {
    char *file_addr = static_cast<char *>(::mmap(nullptr, fileSize, PROT_READ | PROT_WRITE, MAP_SHARED, fileDescriptor, 0));
    if(file_addr == MAP_FAILED) {
      HandleError("Transfer file map");
    }
    if(::close(fileDescriptor)) {
      HandleError("Transfer file map close");
    }
    divideWork(file_addr, fileSize);// STEP 2 Divides the work into 4 threads and converts from lower to uppercase
    if(::munmap(file_addr, fileSize)) {
      HandleError("File Unmap Error");
    }
  }
}

void convertFromLowerToUpper(char *fileArr, int beginning, int end, int fileSize) {
  if(end >= fileSize) {
    end = fileSize-1;
  }
  for(int i = beginning; i <= end; i++) {
    fileArr[i] = toupper(fileArr[i]);
  }
}


//Divides the work up into the appropriate 4 threads
void divideWork(char *fileArray, long int fileSize){
  int beginning1 = 0;
  int end1 = fileSize/4;
  int beginning2 = end1+1;
  int end2 = fileSize/2;
  int beginning3 = end2+1;
  int end3 = (fileSize/4) *3;
  int beginning4 = end3+1;
  int end4 = fileSize-1;
  std::thread proc1(convertFromLowerToUpper, fileArray, beginning1, end1, fileSize);
  std::thread proc2(convertFromLowerToUpper, fileArray, beginning2, end2, fileSize);
  std::thread proc3(convertFromLowerToUpper, fileArray, beginning3, end3, fileSize);
  std::thread proc4(convertFromLowerToUpper, fileArray, beginning4, end4, fileSize);
  proc1.join();
  proc2.join();
  proc3.join();
  proc4.join();
}


std::tuple<int, off_t> DomainSocketClient::OpenFile(const char file_name[], int flags, bool &couldOpen) {
  // open and capture file descriptor
  int fd = ::open(file_name, flags);

  // get size of file
  struct ::stat f_stats;
  if (::fstat(fd, &f_stats) < 0) {
    couldOpen =false;
  }
  return {fd, f_stats.st_size};
}

void DomainSocketClient::HandleError(const char msg[]) {
  std::cerr << "Source: " << msg << std::endl;
  std::cerr << '\t' << ::strerror(errno) << std::endl;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cout<< "not enough arguments" << std::endl;
        return 1;
    }
    std::string domainSocketName = "place";
    char char_array[domainSocketName.length()+1];
    strcpy(char_array, domainSocketName.c_str());
    DomainSocketClient dsc(char_array);
    dsc.RunClient(argv[1]);
    return 0; //STEP 4 Terminates and returns 0
}