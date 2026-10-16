#ifndef AYUDANTIA8_ARREGLO_DINAMICO_H
#define AYUDANTIA8_ARREGLO_DINAMICO_H

#include <cstdlib>
#include <iostream>
#include <stdexcept>

// Plantilla simple de arreglo dinamico con manejo manual de memoria.
template <typename T>
class ArregloDinamico {
private:
    T** datos;
    int tamanio;
    int capacidad;

    void expandir() {
        capacidad += 1;
        T** temp = static_cast<T**>(realloc(datos, capacidad * sizeof(T*)));
        if (temp == nullptr) {
            std::cerr << "Error al redimensionar el arreglo." << std::endl;
            exit(1);
        }
        datos = temp;
    }

    void reducir() {
        if (capacidad > 1) {
            capacidad -= 1;
            T** temp = static_cast<T**>(realloc(datos, capacidad * sizeof(T*)));
            if (temp == nullptr && capacidad > 0) {
                std::cerr << "Error al reducir el tamano del arreglo." << std::endl;
                exit(1);
            }
            datos = temp;
        }
    }

public:
    ArregloDinamico(int capacidadInicial = 1)
        : datos(nullptr), tamanio(0), capacidad(capacidadInicial) {
        datos = static_cast<T**>(malloc(capacidad * sizeof(T*)));
        if (datos == nullptr) {
            std::cerr << "Error al asignar memoria inicial." << std::endl;
            exit(1);
        }
    }

    ArregloDinamico(const ArregloDinamico& otro)
        : datos(nullptr), tamanio(otro.tamanio), capacidad(otro.capacidad) {
        datos = static_cast<T**>(malloc(capacidad * sizeof(T*)));
        if (datos == nullptr) {
            std::cerr << "Error al asignar memoria en copia." << std::endl;
            exit(1);
        }
        for (int i = 0; i < tamanio; ++i) {
            datos[i] = new T(*otro.datos[i]);
        }
    }

    ArregloDinamico(ArregloDinamico&& otro) noexcept
        : datos(otro.datos), tamanio(otro.tamanio), capacidad(otro.capacidad) {
        otro.datos = nullptr;
        otro.tamanio = 0;
        otro.capacidad = 0;
    }

    ArregloDinamico& operator=(const ArregloDinamico& otro) {
        if (this == &otro) {
            return *this;
        }
        for (int i = 0; i < tamanio; ++i) {
            delete datos[i];
        }
        free(datos);

        tamanio = otro.tamanio;
        capacidad = otro.capacidad;
        datos = static_cast<T**>(malloc(capacidad * sizeof(T*)));
        if (datos == nullptr) {
            std::cerr << "Error al asignar memoria en copia." << std::endl;
            exit(1);
        }
        for (int i = 0; i < tamanio; ++i) {
            datos[i] = new T(*otro.datos[i]);
        }
        return *this;
    }

    ArregloDinamico& operator=(ArregloDinamico&& otro) noexcept {
        if (this == &otro) {
            return *this;
        }
        for (int i = 0; i < tamanio; ++i) {
            delete datos[i];
        }
        free(datos);

        datos = otro.datos;
        tamanio = otro.tamanio;
        capacidad = otro.capacidad;

        otro.datos = nullptr;
        otro.tamanio = 0;
        otro.capacidad = 0;
        return *this;
    }

    void agregar(const T& elemento) {
        if (tamanio == capacidad) {
            expandir();
        }
        datos[tamanio++] = new T(elemento);
    }

    T& obtener(int indice) const {
        if (indice < 0 || indice >= tamanio) {
            throw std::out_of_range("Indice fuera de rango");
        }
        return *datos[indice];
    }

    void eliminar(int indice) {
        if (indice < 0 || indice >= tamanio) {
            throw std::out_of_range("Indice fuera de rango");
        }
        delete datos[indice];
        for (int i = indice; i < tamanio - 1; ++i) {
            datos[i] = datos[i + 1];
        }
        tamanio--;
        reducir();
    }

    int getTamanio() const {
        return tamanio;
    }

    int getCapacidad() const {
        return capacidad;
    }

    ~ArregloDinamico() {
        for (int i = 0; i < tamanio; ++i) {
            delete datos[i];
        }
        free(datos);
    }
};

#endif // AYUDANTIA8_ARREGLO_DINAMICO_H

