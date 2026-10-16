//
// Created by amirb on 07-05-2026.
//

#include "../include/struct/Grafo.h"

#include <cstdio>
#include <cstdlib>
#include <limits>
#include <string>

Grafo::Grafo() : matrizAdyacencia(nullptr), cantidadVertices(0) {}

Grafo::~Grafo() {
    for (int i = 0; i < cantidadVertices; i++) {
        free(matrizAdyacencia[i]);
    }
    free(matrizAdyacencia);
}

void Grafo::agregarVertice() {
    cantidadVertices++;

    matrizAdyacencia = static_cast<int**>(
        realloc(matrizAdyacencia, cantidadVertices * sizeof(int*)));

    for (int i = 0; i < cantidadVertices - 1; i++) {
        matrizAdyacencia[i] = static_cast<int*>(
            realloc(matrizAdyacencia[i], cantidadVertices * sizeof(int)));
        matrizAdyacencia[i][cantidadVertices - 1] = 0;
    }

    matrizAdyacencia[cantidadVertices - 1] = static_cast<int*>(
        calloc(cantidadVertices, sizeof(int)));

    nombresCiudades.agregar(std::string());
}

int Grafo::agregarVertice(const std::string& nombre) {
    agregarVertice();
    nombresCiudades.obtener(cantidadVertices - 1) = nombre;
    return cantidadVertices - 1;
}

void Grafo::agregarArista(int origen, int destino, int peso) {
    if (origen >= 0 && origen < cantidadVertices && destino >= 0 && destino < cantidadVertices) {
        matrizAdyacencia[origen][destino] = peso;
    }
}

void Grafo::eliminarArista(int origen, int destino) {
    if (origen >= 0 && origen < cantidadVertices && destino >= 0 && destino < cantidadVertices) {
        matrizAdyacencia[origen][destino] = 0;
    }
}

void Grafo::imprimirMatriz() const {
    printf("\nMatriz de Adyacencia (%d vertices):\n", cantidadVertices);
    for (int i = 0; i < cantidadVertices; i++) {
        for (int j = 0; j < cantidadVertices; j++) {
            printf("%4d", matrizAdyacencia[i][j]);
        }
        printf("\n");
    }
}

void Grafo::imprimirRed() const {
    printf("\nRed de Vuelos (Matriz de Adyacencia):\n");
    printf("-------------------------------------------\n");
    for (int i = 0; i < cantidadVertices; i++) {
        const std::string& nombreOrigen = nombresCiudades.obtener(i);
        if (nombreOrigen.empty()) {
            printf("Vuelos desde Vertice %d:\n", i);
        } else {
            printf("Vuelos desde %s (ID: %d):\n", nombreOrigen.c_str(), i);
        }

        bool tieneVuelos = false;
        for (int j = 0; j < cantidadVertices; j++) {
            int peso = matrizAdyacencia[i][j];
            if (peso != 0) {
                const std::string& nombreDestino = nombresCiudades.obtener(j);
                if (nombreDestino.empty()) {
                    printf("  -> Vertice %d (%d km)\n", j, peso);
                } else {
                    printf("  -> %s (%d km)\n", nombreDestino.c_str(), peso);
                }
                tieneVuelos = true;
            }
        }
        if (!tieneVuelos) {
            printf("  (No hay vuelos directos registrados)\n");
        }
    }
}

int Grafo::getCantidadVertices() const {
    return cantidadVertices;
}

const std::string& Grafo::obtenerNombreVertice(int indice) const {
    return nombresCiudades.obtener(indice);
}

std::string Grafo::nombreVertice(int indice) const {
    const std::string& nombre = obtenerNombreVertice(indice);
    if (!nombre.empty()) {
        return nombre;
    }
    return "Vertice " + std::to_string(indice);
}

ArregloDinamico<int> Grafo::dfs(int origen) const {
    ArregloDinamico<int> recorrido;
    ArregloDinamico<int> visitado;
    ArregloDinamico<int> pila;

    // Al principio ningun vertice ha sido visitado.
    // Usamos 0 para "no visitado" y 1 para "visitado".
    for (int i = 0; i < cantidadVertices; ++i) {
        visitado.agregar(0);
    }

    // Si el vertice inicial no existe, no hay nada que recorrer.
    if (origen < 0 || origen >= cantidadVertices) {
        return recorrido;
    }

    // DFS comienza colocando el vertice inicial en la pila.
    // el ultimo elemento que entra es el primero que sale.
    pila.agregar(origen);

    while (pila.getTamanio() > 0) {
        // Tomamos y eliminamos el ultimo elemento de la pila.
        int ultimo = pila.getTamanio() - 1;
        int actual = pila.obtener(ultimo);
        pila.eliminar(ultimo);

        // Un vertice puede haberse agregado mas de una vez a la pila.
        // Si ya fue visitado, simplemente lo ignoramos.
        if (visitado.obtener(actual)) {
            continue;
        }

        // Marcamos el vertice antes de revisar sus vecinos.
        visitado.obtener(actual) = 1;
        recorrido.agregar(actual);

        // Agregamos los vecinos a la pila.
        // Como la pila devuelve primero el ultimo elemento agregado,
        // los recorremos al reves para visitar primero el menor indice.
        for (int vecino = cantidadVertices - 1; vecino >= 0; --vecino) {
            if (matrizAdyacencia[actual][vecino] != 0 &&
                !visitado.obtener(vecino)) {
                pila.agregar(vecino);
            }
        }
    }

    return recorrido;
}

ArregloDinamico<int> Grafo::bfs(int origen) const {
    ArregloDinamico<int> recorrido;
    ArregloDinamico<int> visitado;
    ArregloDinamico<int> cola;

    // Inicialmente todos los vertices estan sin visitar.
    for (int i = 0; i < cantidadVertices; ++i) {
        visitado.agregar(0);
    }

    // Un origen invalido produce un recorrido vacio.
    if (origen < 0 || origen >= cantidadVertices) {
        return recorrido;
    }

    // BFS comienza agregando el origen a la cola.
    // el primero que entra es el primero que sale.
    cola.agregar(origen);

    // Lo marcamos inmediatamente para no volver a agregarlo.
    visitado.obtener(origen) = 1;

    // "siguiente" indica la posicion del proximo elemento de la cola.
    // No eliminamos los elementos: solo avanzamos por sus posiciones.
    int siguiente = 0;

    while (siguiente < cola.getTamanio()) {
        int actual = cola.obtener(siguiente);
        ++siguiente;

        // El vertice sale de la cola y queda registrado en el recorrido.
        recorrido.agregar(actual);

        // Buscamos todos los vecinos directos del vertice actual.
        for (int vecino = 0; vecino < cantidadVertices; ++vecino) {
            if (matrizAdyacencia[actual][vecino] != 0 &&
                !visitado.obtener(vecino)) {
                // Al marcarlo al momento de agregarlo, evitamos
                // agregar el mismo vertice varias veces.
                visitado.obtener(vecino) = 1;
                cola.agregar(vecino);
            }
        }
    }

    return recorrido;
}

ArregloDinamico<int> Grafo::dijkstra(int origen) const {
    const int INF = std::numeric_limits<int>::max() / 4;
    ArregloDinamico<int> distancias;
    ArregloDinamico<int> visitado;

    // Al principio no conocemos ninguna distancia.
    // Por eso todos los vertices comienzan con INF (infinito).
    // Tambien dejamos todos los vertices como no visitados.
    for (int i = 0; i < cantidadVertices; ++i) {
        distancias.agregar(INF);
        visitado.agregar(0);
    }

    // Si el origen no existe, devolvemos todas las distancias como INF.
    if (origen < 0 || origen >= cantidadVertices) {
        return distancias;
    }

    // Llegar desde el origen hasta el propio origen cuesta 0.
    distancias.obtener(origen) = 0;

    // Repetimos el proceso hasta considerar todos los vertices posibles.
    for (int iter = 0; iter < cantidadVertices; ++iter) {
        int u = -1;
        int mejor = INF;

        // Elegimos el vertice no visitado con la menor distancia conocida.
        // Este es el vertice mas prometedor para continuar el recorrido.
        for (int i = 0; i < cantidadVertices; ++i) {
            if (!visitado.obtener(i) && distancias.obtener(i) < mejor) {
                mejor = distancias.obtener(i);
                u = i;
            }
        }
        // Si no encontramos ninguno, los vertices restantes son inalcanzables.
        if (u == -1) {
            break;
        }

        // La distancia de u ya es la mejor posible, asi que lo dejamos fijo.
        visitado.obtener(u) = 1;

        // Revisamos cada vecino de u para intentar mejorar su distancia.
        for (int v = 0; v < cantidadVertices; ++v) {
            int peso = matrizAdyacencia[u][v];
            if (peso != 0 && !visitado.obtener(v)) {
                // Calculamos cuanto costaria llegar a v pasando por u.
                int candidato = distancias.obtener(u) + peso;

                // Si este camino es mas corto, actualizamos la mejor distancia.
                if (candidato < distancias.obtener(v)) {
                    distancias.obtener(v) = candidato;
                }
            }
        }
    }

    return distancias;
}

void Grafo::imprimirDistancias(const ArregloDinamico<int>& distancias, const char* titulo) const {
    const int INF = std::numeric_limits<int>::max() / 4;
    printf("\n%s\n", titulo);
    for (int i = 0; i < getCantidadVertices(); ++i) {
        std::string nombre = nombreVertice(i);
        if (distancias.obtener(i) >= INF) {
            printf("  -> %s: INF\n", nombre.c_str());
        } else {
            printf("  -> %s: %d\n", nombre.c_str(), distancias.obtener(i));
        }
    }
}
