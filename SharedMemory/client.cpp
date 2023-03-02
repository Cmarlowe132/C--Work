#include "client.h"

UnixDomainSocket::UnixDomainSocket(const char *socket_path) {
    socket_path_ = std::string(socket_path);
    sock_addr_ = {};
    sock_addr_.sun_family = AF_UNIX; 
    strncpy(sock_addr_.sun_path + 1, socket_path, sizeof(sock_addr_.sun_path) - 2);
  }


void DomainSocketClient::RunClient(std::string file_path, std::string search_string) {
  // open nameless Unix socket
  int socket_fd = socket(AF_UNIX, SOCK_STREAM, 0);
  if (socket_fd < 0) {
    std::cerr << strerror(errno) << std::endl;
    exit(-1);
  }
  std::string findThis = search_string;
  int sharedMemoryFileDesc = ::shm_open("place", O_CREAT | O_RDWR, 0660); //STEP 1 Opens shared Memory with name "place"
  const size_t kSharedMemBufferSize = 4096 - 2*sizeof(size_t);
  const size_t kShareMemSize = sizeof(SharedMemoryStore) + kSharedMemBufferSize; //creates the size of the buffer for shared memory and the size of shared memory
  const int kProt = PROT_READ | PROT_WRITE;
  if(::ftruncate(sharedMemoryFileDesc, kShareMemSize) < 0) {
    std::cerr << strerror(errno) << std::endl;
    exit(-1);
  }
  
  store_ = static_cast<SharedMemoryStore*>(::mmap(nullptr, sizeof(SharedMemoryStore), kProt, MAP_SHARED, sharedMemoryFileDesc, 0));
  *store_ = {};
  store_->buffer_size = kSharedMemBufferSize;

  
  //  connect to an existing socket
  int success = connect(socket_fd, reinterpret_cast<const sockaddr*>(&sock_addr_), sizeof(sock_addr_));
  if (success < 0) {
    std::cerr << strerror(errno) << std::endl;
    exit(-1);
  }

  if(sharedMemoryFileDesc < 0) {
    std::cerr << strerror(errno) << std::endl;
    exit(-1);
  }
  std::clog<< "SERVER CONNECTION ACCEPTED" << std::endl; //Client has connected to server

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
  
  ssize_t bytes_wrote = write(socket_fd, write_buffer, kWrite_buffer_size); //Step 2 Write the File path to the server
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
  bool continueOn = true; 
  int counter = 0;
  while(continueOn) { // STEP 3 Once the client receives an update from the server, it knows the shared memory is full goes through this loop
    int bytes_read = read(socket_fd, read_buffer, kRead_buffer_size);
    std::string buildString = "";
    for(int i = 0; i < kSharedMemBufferSize; i++) {
      if(store_->buffer[i] == '\n') {
        outputVector.push_back(buildString);//End of a line, push the currently built string into the output vector
        buildString = "";
      } else if ((counter + i) == store_->stringLength) { //At this point, all of the file has been read and the loop can end
        buildString+= store_->buffer[i];
        outputVector.push_back(buildString);
        continueOn = false; 
        break;
      } else {
        buildString+=store_->buffer[i];//append the next character from the buffer onto the string
      }
    }
    counter += kSharedMemBufferSize;
    if(continueOn == false) {
      break;
    }
    bytes_wrote = write(socket_fd, write_buffer, kWrite_buffer_size); //Write back to the server and tell it that the client is ready for more
    if (bytes_wrote < 0) {
      std::cerr << strerror(errno) << std::endl;
      exit(-1);
    }  
  }

  
  if(searchLine(outputVector.at(0), "INVALID FILE")) { //Invalid file read
    std::cerr << "INVALID FILE" << std::endl;
  }
  else {
    std::vector<std::string> foundLines = divideWork(outputVector, findThis); // STEP 4 divides the process of searching for a specific string
    if(foundLines.size()==0) {
      std::cout << "NO RESULTS FOUND" << std::endl;
    }
    for(int i =0; i < foundLines.size(); i++) {
      std::cout << i+1 << "\t" << foundLines.at(i) << std::endl;// STEP 5
    }
  }
  int result = ::munmap(store_, sizeof(SharedMemoryStore)); //STEP 6 Part 1, other part happens in destructor
  if (result < 0) {
    exit;
  }
}

//Finds a keyword from a set start point to the set endpoint within the passed in vector
void findKeyword(std::vector<std::string> file, int start, int end, std::string searchString, std::vector<std::string> &outputVector){
  if(start < file.size()) {
    for(int i = start; i <= end; i++) {
      if(searchLine(file.at(i), searchString)) {
        outputVector.push_back(file.at(i));
      }
    } 
  }
}

//Searches the current line for the passed in String, returns true if found
const bool searchLine(const std::string line, const std::string searchString){
  if(line.find(searchString) != std::string::npos) {
    return true; 
  }
  return false; 
}

//Divides the work up into the appropriate 4 threads
std::vector<std::string> divideWork(std::vector<std::string> text, std::string searchFor){
  int beginning1 = 0;
  int end1 = text.size()/4;
  int beginning2 = end1+1;
  int end2 = text.size()/2;
  int beginning3 = end2+1;
  int end3 = (text.size()/4) *3;
  int beginning4 = end3+1;
  int end4 = text.size()-1;
  std::vector<std::string> found1;
  std::vector<std::string> found2;
  std::vector<std::string> found3;
  std::vector<std::string> found4;
  std::thread proc1(findKeyword, text, beginning1, end1, searchFor, std::ref (found1));
  std::thread proc2(findKeyword, text, beginning2, end2, searchFor, std::ref (found2));
  std::thread proc3(findKeyword, text, beginning3, end3, searchFor, std::ref (found3));
  std::thread proc4(findKeyword, text, beginning4, end4, searchFor, std::ref (found4));
  proc1.join();
  proc2.join();
  proc3.join();
  proc4.join();
  std::vector<std::string> returnVector;
  for(int i =0; i < found1.size(); i++) {
    returnVector.push_back(found1.at(i));
  }
  for(int i =0; i < found2.size(); i++) {
    returnVector.push_back(found2.at(i));
  }
  for(int i =0; i < found3.size(); i++) {
    returnVector.push_back(found3.at(i));
  }
  for(int i =0; i < found4.size(); i++) {
    returnVector.push_back(found4.at(i));
  }
  return returnVector;
}

//DomainSocketClient Destructor, unlinks the shared memory
DomainSocketClient::~DomainSocketClient() {
  if(::shm_unlink("place") < 0) { //STEP 6, Part 2
    std::cerr << ::strerror(errno) << std::endl;
  }
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        std::cout<< "not enough arguments" << std::endl;
        return 1;
    }
    std::string domainSocketName = "place";
    char char_array[domainSocketName.length()+1];
    strcpy(char_array, domainSocketName.c_str());
    DomainSocketClient dsc(char_array);
    dsc.RunClient(argv[1],argv[2]);
    return 0; //STEP 7 
}