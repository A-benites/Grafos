#include <cstdio>

#include "include/struct/Grafo.h"

int main() {
    Grafo redDeVuelos;

    printf("Agregando ciudades a la red...\n");
    int santiagoID = redDeVuelos.agregarVertice("Santiago");
    int limaID = redDeVuelos.agregarVertice("Lima");
    int buenosAiresID = redDeVuelos.agregarVertice("Buenos Aires");
    int bogotaID = redDeVuelos.agregarVertice("Bogota");

    printf("\nCreando rutas de vuelo...\n");
    redDeVuelos.agregarArista(santiagoID, limaID, 2490);
    redDeVuelos.agregarArista(limaID, santiagoID, 2490);

    redDeVuelos.agregarArista(santiagoID, buenosAiresID, 1140);
    redDeVuelos.agregarArista(buenosAiresID, santiagoID, 1140);

    redDeVuelos.agregarArista(limaID, bogotaID, 1880);
    redDeVuelos.agregarArista(bogotaID, limaID, 1880);

    redDeVuelos.imprimirRed();

    // DFS (Depth-First Search) explora un camino lo mas profundo posible
    // antes de volver atras. Internamente utiliza una pila.
    ArregloDinamico<int> recorridoDFS = redDeVuelos.dfs(santiagoID);
    printf("\nDFS desde Santiago: ");
    for (int i = 0; i < recorridoDFS.getTamanio(); ++i) {
        printf("%s", redDeVuelos.nombreVertice(recorridoDFS.obtener(i)).c_str());
        if (i < recorridoDFS.getTamanio() - 1) {
            printf(" -> ");
        }
    }
    printf("\n");

    // BFS (Breadth-First Search) visita primero los vecinos directos,
    // luego los vecinos de esos vecinos. Internamente utiliza una cola.
    ArregloDinamico<int> recorridoBFS = redDeVuelos.bfs(santiagoID);
    printf("BFS desde Santiago: ");
    for (int i = 0; i < recorridoBFS.getTamanio(); ++i) {
        printf("%s", redDeVuelos.nombreVertice(recorridoBFS.obtener(i)).c_str());
        if (i < recorridoBFS.getTamanio() - 1) {
            printf(" -> ");
        }
    }
    printf("\n");

    // Dijkstra no solo recorre el grafo: tambien suma los pesos de las rutas.
    // En este ejemplo, los pesos representan kilometros.
    ArregloDinamico<int> distanciasDijkstra = redDeVuelos.dijkstra(santiagoID);
    redDeVuelos.imprimirDistancias(distanciasDijkstra, "Dijkstra desde Santiago");
    return 0;
}
