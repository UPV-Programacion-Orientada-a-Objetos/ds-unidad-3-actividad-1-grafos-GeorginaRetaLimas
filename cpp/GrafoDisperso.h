#ifndef GRAFODISPERSO_H
#define GRAFODISPERSO_H

#include "GrafoBase.h"
#include <string>
#include <vector>

class GrafoDisperso : public GrafoBase {
private:
  int numNodos;
  int numAristas;

  // Formato CSR
  // row_ptr: índices donde comienza cada fila en col_indices
  // col_indices: índices de columna para cada valor no nulo
  // values: podríamos no necesitar esto para grafos no ponderados, pero lo
  // mantenemos por estructura estándar CSR si es necesario. Para este problema
  // específico (no ponderado), podemos omitir values o simplemente almacenar
  // 1s. Nos quedaremos con col_indices y row_ptr para la adyacencia.

  std::vector<int> row_ptr;
  std::vector<int> col_indices;

public:
  GrafoDisperso();
  ~GrafoDisperso();

  void cargarDatos(const std::string &archivo) override;
  std::vector<int> BFS(int nodoInicio, int profundidad) override;
  int obtenerGrado(int nodo) override;
  std::vector<int> getVecinos(int nodo) override;
  int getNumNodos() override;
  int getNumAristas() override;

  // Ayudante para obtener el id máximo de nodo durante la carga para
  // redimensionar
  void redimensionar(int maxNodo);
};

#endif // GRAFODISPERSO_H
