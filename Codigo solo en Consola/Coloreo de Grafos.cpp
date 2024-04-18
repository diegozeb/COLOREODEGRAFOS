#include <iostream>
#include <vector>
#include <unordered_set>

using namespace std;

class Grafo {
    int V;
    vector<unordered_set<int>> ady;

public:
    Grafo(int V) : V(V), ady(V) {}

    void agregarArista(int u, int v) {
        ady[u].insert(v);
        ady[v].insert(u);
    }

    int contarColoresAdyacentes(int vertice, const vector<int>& colores) {
        int cuenta = 0;
        for (int verticeAdy : ady[vertice]) {
            if (colores[verticeAdy] != -1) {
                cuenta++;
            }
        }
        return cuenta;
    }

    // Función para seleccionar la Variable Más Restringida (MRV)
    int seleccionarVariableMRV(const vector<int>& colores, const vector<bool>& asignados) {
        int minValoresRestantes = INT_MAX;
        int verticeSeleccionado = -1;

        for (int v = 0; v < V; v++) {
            if (!asignados[v]) {
                int valoresRestantes = contarColoresAdyacentes(v, colores);
                if (valoresRestantes < minValoresRestantes) {
                    minValoresRestantes = valoresRestantes;
                    verticeSeleccionado = v;
                }
            }
        }

        return verticeSeleccionado;
    }

    // Función para seleccionar la Variable Más Restrictiva (MCU)
    int seleccionarVariableMCU(const vector<int>& colores, const vector<bool>& asignados) {
        int verticeMaxRestriccion = -1;
        int maxRestricciones = INT_MIN;

        for (int v = 0; v < V; v++) {
            if (!asignados[v]) {
                int restricciones = contarColoresAdyacentes(v, colores);
                if (restricciones > maxRestricciones) {
                    maxRestricciones = restricciones;
                    verticeMaxRestriccion = v;
                }
            }
        }

        return verticeMaxRestriccion;
    }

    // Función para colorear el grafo utilizando la Variable Más Restringida (MRV)
    void colorearPorMRV(int m) {
        vector<int> colores(V, -1);
        vector<bool> asignados(V, false);

        for (int i = 0; i < V; i++) {
            int vertice = seleccionarVariableMRV(colores, asignados);
            if (vertice == -1) {
                break; // Todos los vértices han sido coloreados
            }
            asignarColor(vertice, colores, asignados, m);
        }

        // Imprimir el resultado del coloreo
        imprimirColoreo(colores);
    }

    // Función para colorear el grafo utilizando la Variable Más Restrictiva (MCU) sin backtracking
    void colorearPorMCU(int m) {
        vector<int> colores(V, -1);
        vector<bool> asignados(V, false);

        for (int i = 0; i < V; i++) {
            int vertice = seleccionarVariableMCU(colores, asignados);
            if (vertice == -1) {
                break;
            }
            asignarColor(vertice, colores, asignados, m);
        }

        imprimirColoreo(colores);
    }

    // Función para colorear el grafo utilizando la Variable Más Restrictiva (MCU) con backtracking
    void colorearPorMCUConBacktracking(int m) {
        vector<int> colores(V, -1);
        vector<bool> asignados(V, false);

        colorearConBacktracking(0, colores, asignados, m);

        imprimirColoreo(colores);
    }

private:
    // Función auxiliar para colorear recursivamente utilizando backtracking
    bool colorearConBacktracking(int vertice, vector<int>& colores, vector<bool>& asignados, int m) {
        if (vertice == V) {
            return true;
        }

        for (int c = 0; c < m; c++) {
            bool esColorDisponible = true;
            for (int verticeAdy : ady[vertice]) {
                if (colores[verticeAdy] == c) {
                    esColorDisponible = false;
                    break;
                }
            }
            if (esColorDisponible) {
                colores[vertice] = c;
                asignados[vertice] = true;
                if (colorearConBacktracking(vertice + 1, colores, asignados, m)) {
                    return true;
                }
                colores[vertice] = -1;
                asignados[vertice] = false;
            }
        }

        return false;
    }

    // Función para asignar un color a un vértice dado
    void asignarColor(int vertice, vector<int>& colores, vector<bool>& asignados, int m) {
        vector<bool> disponibles(m, true);

        for (int verticeAdy : ady[vertice]) {
            if (colores[verticeAdy] != -1) {
                disponibles[colores[verticeAdy]] = false;
            }
        }

        for (int c = 0; c < m; c++) {
            if (disponibles[c]) {
                colores[vertice] = c;
                asignados[vertice] = true;
                break;
            }
        }
    }

    // Función para imprimir el resultado del coloreo
    void imprimirColoreo(const vector<int>& colores) {
        for (int i = 0; i < V; i++) {
            cout << "Vertice " << i << " --> Color ";
            if (colores[i] == -1) {
                cout << "Sin asignar" << endl;
            }
            else if (colores[i] == 0) {
                cout << "Rojo" << endl;
            }
            else if (colores[i] == 1) {
                cout << "Verde" << endl;
            }
            else if (colores[i] == 2) {
                cout << "Amarillo" << endl;
            }
        }
    }
};

int main() {
    int V1 = 5;
    Grafo grafo1(V1);

    grafo1.agregarArista(0, 1);
    grafo1.agregarArista(0, 3);
    grafo1.agregarArista(0, 4);
    grafo1.agregarArista(1, 2);
    grafo1.agregarArista(2, 3);
    grafo1.agregarArista(3, 4);
    grafo1.agregarArista(2, 4);

    int m1 = 3;

    cout << "GRAFO 1" << endl;
    cout << "Coloreo utilizando Variable Mas Restringida (MRV):" << endl;
    grafo1.colorearPorMRV(m1);

    cout << "\nColoreo utilizando Variable Mas Restrictiva (MCU):" << endl;
    grafo1.colorearPorMCU(m1);

    //=============================================================================

    int V2 = 7;
    Grafo grafo2(V2);

    grafo2.agregarArista(0, 1);
    grafo2.agregarArista(0, 3);
    grafo2.agregarArista(1, 2);
    grafo2.agregarArista(1, 6);
    grafo2.agregarArista(2, 3);
    grafo2.agregarArista(3, 4);
    grafo2.agregarArista(3, 5);
    grafo2.agregarArista(2, 5);
    grafo2.agregarArista(4, 5);
    grafo2.agregarArista(4, 6);
    grafo2.agregarArista(5, 6);

    int m2 = 3;

    cout << endl << "GRAFO 2" << endl;
    cout << "Coloreo utilizando Variable Mas Restringida (MRV):" << endl;
    grafo2.colorearPorMRV(m2);

    cout << "\nColoreo utilizando Variable Mas Restrictiva (MCU):" << endl;
    grafo2.colorearPorMCU(m2);

    cout << "\nColoreo utilizando Variable Mas Restrictiva (MCU) con backtracking:" << endl;
    grafo2.colorearPorMCUConBacktracking(m2);

    return 0;
}
