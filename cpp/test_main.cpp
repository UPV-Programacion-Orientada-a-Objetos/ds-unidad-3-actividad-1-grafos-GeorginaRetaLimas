#include "GrafoDisperso.h"
#include <cassert>
#include <iostream>

int main() {
  GrafoDisperso g;
  g.cargarDatos("test_data.txt");

  std::cout << "Nodos: " << g.getNumNodos() << std::endl;
  std::cout << "Aristas: " << g.getNumAristas() << std::endl;

  // Test Degree
  std::cout << "Grado nodo 0: " << g.obtenerGrado(0) << std::endl;

  // Test Neighbors
  std::vector<int> vecinos = g.getVecinos(0);
  std::cout << "Vecinos nodo 0: ";
  for (int v : vecinos)
    std::cout << v << " ";
  std::cout << std::endl;

  // Test BFS
  std::cout << "BFS desde 0, prof 2: ";
  std::vector<int> bfs = g.BFS(0, 2);
  for (int n : bfs)
    std::cout << n << " ";
  std::cout << std::endl;

  return 0;
}
