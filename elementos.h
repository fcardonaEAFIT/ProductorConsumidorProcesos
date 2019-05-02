#pragma once

#define N_BUFFER 10

struct elemento {
  int elemento;
};

struct Buffer {
  int entra;
  int sale;
  int cantidad;
  int tamano;
  struct elemento buffer[N_BUFFER];
};
