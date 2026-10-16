//
// Created by amirb on 07-05-2026.
//

#ifndef AYUDANTIA8_GRAFO_H
#define AYUDANTIA8_GRAFO_H

#include <string>

#include "ArregloDinamico.h"

class Grafo {
private:
    int** matrizAdyacencia;
    ArregloDinamico<std::string> nombresCiudades;
    int cantidadVertices;

public:
    Grafo();
    ~Grafo();

    void agregarVertice();
    int agregarVertice(const std::string& nombre);
    void agregarArista(int origen, int destino, int peso);
    void eliminarArista(int origen, int destino);
    void imprimirMatriz() const;
    void imprimirRed() const;

    int getCantidadVertices() const;
    const std::string& obtenerNombreVertice(int indice) const;
    std::string nombreVertice(int indice) const;

    // Recorre el grafo en profundidad usando una pila.
    // Devuelve los vertices en el orden en que fueron visitados.
    ArregloDinamico<int> dfs(int origen) const;

    // Recorre el grafo por niveles usando una cola.
    // Devuelve los vertices en el orden en que fueron visitados.
    ArregloDinamico<int> bfs(int origen) const;

    // Calcula la distancia minima desde el origen hasta cada vertice.
    // En este grafo, el peso de cada arista representa kilometros.
    ArregloDinamico<int> dijkstra(int origen) const;
    void imprimirDistancias(const ArregloDinamico<int>& distancias, const char* titulo) const;
};

#endif //AYUDANTIA8_GRAFO_H
