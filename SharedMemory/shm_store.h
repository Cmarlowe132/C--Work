#ifndef SHM_LOGGER_INC_SHM_STORE_H_
#define SHM_LOGGER_INC_SHM_STORE_H_


#include <cstddef>  // size_t


struct SharedMemoryStore {
  size_t buffer_size;
  size_t stringLength; 
  char buffer[1];  // this will be extended by external code
};


#endif  // SHM_LOGGER_INC_SHM_STORE_H_