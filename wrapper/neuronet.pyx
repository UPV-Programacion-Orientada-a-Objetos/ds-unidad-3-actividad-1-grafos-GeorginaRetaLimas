# distutils: language = c++

from neuronet_core cimport GrafoDisperso
from libcpp.vector cimport vector
from libcpp.string cimport string

cdef class NeuroNetEngine:
    cdef GrafoDisperso* thisptr

    def __cinit__(self):
        self.thisptr = new GrafoDisperso()

    def __dealloc__(self):
        del self.thisptr

    def load_data(self, str filename):
        self.thisptr.cargarDatos(filename.encode('utf-8'))

    def bfs(self, int start_node, int depth):
        return self.thisptr.BFS(start_node, depth)

    def get_degree(self, int node):
        return self.thisptr.obtenerGrado(node)

    def get_neighbors(self, int node):
        return self.thisptr.getVecinos(node)

    def get_num_nodes(self):
        return self.thisptr.getNumNodos()

    def get_num_edges(self):
        return self.thisptr.getNumAristas()
