#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <iostream>
#include <cstdlib>
#include <cerrno>
#include <cstring>
#include "elementos.h"

using namespace std;

int
main(void) {

  sem_t *vacios, *llenos, *mutex;

  vacios = sem_open("vacios", 0);
  llenos = sem_open("llenos", 0);
  mutex  = sem_open("mutex", 0);

  int fd = shm_open("/buffer", O_RDWR, 0660);

  if (fd < 0) {
    cerr << "Error abriendo la memoria compartida: "
	 << errno << strerror(errno) << endl;
    exit(1);
  }

  void *dir;

  if ((dir = mmap(NULL, sizeof(struct elemento), PROT_READ | PROT_WRITE, MAP_SHARED,
		  fd, 0)) == MAP_FAILED) {
    cerr << "Error mapeando la memoria compartida: "
	 << errno << strerror(errno) << endl;
    exit(1);
  }

  struct Buffer *pBuffer = (struct Buffer *) dir;
  int item = 0;

  for(;;) {

    sem_wait(llenos);
    sem_wait(mutex);

    item = pBuffer->buffer[pBuffer->sale].elemento;
    pBuffer->sale = (pBuffer->sale + 1) % pBuffer->tamano;
    pBuffer->cantidad--;

    sem_post(mutex);
    sem_post(vacios);
    cout << item << endl;
  }

  return EXIT_SUCCESS;
}
