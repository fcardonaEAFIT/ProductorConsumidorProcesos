#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <semaphore.h>

using namespace std;

int
main(void) {

  sem_unlink("vacios");
  sem_unlink("llenos");
  sem_unlink("mutex");
  shm_unlink("/buffer");

  return EXIT_SUCCESS;
}
