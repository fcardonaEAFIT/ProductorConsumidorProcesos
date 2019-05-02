#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <semaphore.h>
#include "elementos.h"
#include <iostream>
#include <cstdlib>
#include <cstring>

using namespace std;

int
main(void) {

  sem_t *vacios = sem_open("vacios", O_CREAT | O_EXCL, 0660, N_BUFFER);
  sem_t *llenos = sem_open("llenos", O_CREAT | O_EXCL, 0660, 0);
  sem_t *mutex  = sem_open("mutex", O_CREAT | O_EXCL, 0660, 1);

  int fd = shm_open("buffer", O_RDWR | O_CREAT | O_EXCL, 0660);

  if (fd < 0) {
    cerr << "Error creando la memoria compartida: "
	 << errno << strerror(errno) << endl;
    exit(1);
  }

  if (ftruncate(fd, sizeof(struct Buffer)) != 0) {
    cerr << "Error creando la memoria compartida: "
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
  pBuffer->entra = 0;
  pBuffer->sale = 0;
  pBuffer->cantidad = 0;
  pBuffer->tamano = N_BUFFER;

  close(fd);

  return EXIT_SUCCESS;
}
