#include "GrafoDisperso.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <sstream>

GrafoDisperso::GrafoDisperso() : numNodos(0), numAristas(0) {
  row_ptr.push_back(0);
}

GrafoDisperso::~GrafoDisperso() {}

void GrafoDisperso::redimensionar(int maxNodo) {
  if (maxNodo >= numNodos) {
    int oldSize = numNodos;
    numNodos = maxNodo + 1;
    row_ptr.resize(numNodos + 1, row_ptr.back());
  }
}

void GrafoDisperso::cargarDatos(const std::string &archivo) {
  std::ifstream file(archivo);
  if (!file.is_open()) {
    std::cerr << "Error: No se pudo abrir el archivo " << archivo << std::endl;
    return;
  }

  std::cout << "[C++ Core] Cargando dataset '" << archivo << "'..."
            << std::endl;

  // Primera pasada: contar grados para construir row_ptr (array de
  // desplazamiento) Realmente, para CSR desde lista de aristas, es más fácil
  // cargar en listas de adyacencia primero si la memoria lo permite, O leer dos
  // veces. Leer dos veces es más seguro para la memoria si el grafo es
  // verdaderamente masivo y no está ordenado. Sin embargo, el enfoque estándar
  // para "masivo" en 16GB RAM:
  // 1. Leer aristas, almacenar en vector temporal de pares.
  // 2. Ordenar pares.
  // 3. Construir CSR.

  // Intentemos el enfoque del vector de pares.
  // 500k nodos, ~5M aristas. 5M pares de ints = 5M * 8 bytes = 40MB. Muy
  // seguro.

  std::vector<std::pair<int, int>> edges;
  std::string line;
  int maxId = 0;

  while (std::getline(file, line)) {
    if (line.empty() || line[0] == '#')
      continue;
    std::stringstream ss(line);
    int u, v;
    if (ss >> u >> v) {
      edges.push_back({u, v});
      // Asumiendo grafo dirigido según formato CSR estándar.
      // Si se necesita no dirigido, agregaríamos {v, u} también.
      // README dice "Diferencia entre grafos dirigidos y no dirigidos".
      // Trataremos como dirigido por ahora basado en el formato de entrada.
      if (u > maxId)
        maxId = u;
      if (v > maxId)
        maxId = v;
    }
  }

  numAristas = edges.size();
  redimensionar(maxId);

  // Ordenar aristas por origen, luego destino
  std::sort(edges.begin(), edges.end());

  // Construir CSR
  row_ptr.assign(numNodos + 1, 0);
  col_indices.resize(numAristas);

  // Llenar row_ptr con conteos primero
  std::vector<int> counts(numNodos, 0);
  for (const auto &edge : edges) {
    counts[edge.first]++;
  }

  // Suma acumulativa para row_ptr
  row_ptr[0] = 0;
  for (int i = 0; i < numNodos; ++i) {
    row_ptr[i + 1] = row_ptr[i] + counts[i];
  }

  // Llenar col_indices
  std::vector<int> current_pos = row_ptr;
  for (const auto &edge : edges) {
    int u = edge.first;
    int v = edge.second;
    col_indices[current_pos[u]] = v;
    current_pos[u]++;
  }

  std::cout << "[C++ Core] Carga completa. Nodos: " << numNodos
            << " | Aristas: " << numAristas << std::endl;
}

int GrafoDisperso::obtenerGrado(int nodo) {
  if (nodo < 0 || nodo >= numNodos)
    return 0;
  return row_ptr[nodo + 1] - row_ptr[nodo];
}

std::vector<int> GrafoDisperso::getVecinos(int nodo) {
  std::vector<int> vecinos;
  if (nodo < 0 || nodo >= numNodos)
    return vecinos;

  int start = row_ptr[nodo];
  int end = row_ptr[nodo + 1];

  for (int i = start; i < end; ++i) {
    vecinos.push_back(col_indices[i]);
  }
  return vecinos;
}

std::vector<int> GrafoDisperso::BFS(int nodoInicio, int profundidad) {
  std::vector<int> visitados;
  if (nodoInicio < 0 || nodoInicio >= numNodos)
    return visitados;

  std::queue<std::pair<int, int>> q; // nodo, nivel
  std::set<int> visited_set;

  q.push({nodoInicio, 0});
  visited_set.insert(nodoInicio);
  visitados.push_back(nodoInicio);

  while (!q.empty()) {
    int u = q.front().first;
    int nivel = q.front().second;
    q.pop();

    if (nivel >= profundidad)
      continue;

    int start = row_ptr[u];
    int end = row_ptr[u + 1];

    for (int i = start; i < end; ++i) {
      int v = col_indices[i];
      if (visited_set.find(v) == visited_set.end()) {
        visited_set.insert(v);
        visitados.push_back(v);
        q.push({v, nivel + 1});
      }
    }
  }

  return visitados;
}

int GrafoDisperso::getNumNodos() { return numNodos; }

int GrafoDisperso::getNumAristas() { return numAristas; }
