#include <GL/glut.h>
#include <iostream>
#include <vector>
#include <unordered_set>
#include <cmath>

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
	
	int numVertices() const {
		return V;
	}
	
	int contarColoresAdyacentes(int vertice, const vector<int>& colores) {
		int cuenta = 0;
		for (int verticeAdy : adyacentes(vertice)) {
			if (colores[verticeAdy] != -1) {
				cuenta++;
			}
		}
		return cuenta;
	}
	
	int seleccionarVariableMRV(const vector<int>& colores, const vector<bool>& asignados) {
		int minValoresRestantes = 99999999999;
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
	
	int seleccionarVariableMCU(const vector<int>& colores, const vector<bool>& asignados) {
		int verticeMaxRestriccion = -1;
		int maxRestricciones = -2147483648;
		
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
	
	void colorearPorMRV(int m, vector<int>& colores) {
		vector<bool> asignados(V, false);
		
		for (int i = 0; i < V; i++) {
			int vertice = seleccionarVariableMRV(colores, asignados);
			if (vertice == -1) {
				break; // Todos los vértices han sido coloreados
			}
			asignarColor(vertice, colores, asignados, m);
			asignados[vertice] = true;
		}
		
	}
	
	void colorearPorMCU(int m, vector<int>& colores) {
		vector<bool> asignados(V, false);
		
		for (int i = 0; i < V; i++) {
			int vertice = seleccionarVariableMCU(colores, asignados);
			if (vertice == -1) {
				break;
			}
			asignarColor(vertice, colores, asignados, m);
			asignados[vertice] = true; // Actualizar la lista de asignados
		}
	}
	
	const unordered_set<int>& adyacentes(int vertice) const {
		return ady[vertice];
	}
	
private:
		void asignarColor(int vertice, vector<int>& colores, vector<bool>& asignados, int m) {
			vector<bool> disponibles(m, true);
			
			for (int verticeAdy : adyacentes(vertice)) {
				if (colores[verticeAdy] != -1) {
					disponibles[colores[verticeAdy]] = false;
				}
			}
			
			for (int c = 0; c < m; c++) {
				if (disponibles[c]) {
					colores[vertice] = c;
					asignados[vertice] = true;
					
					// Actualizar la lista de colores asignados para los vértices adyacentes
					for (int verticeAdy : adyacentes(vertice)) {
						if (colores[verticeAdy] == -1) {
							disponibles[c] = false;
						}
					}
					
					break;
				}
			}
		}
};

Grafo grafo1(5);
Grafo grafo12(5);
Grafo grafo2(7);

void init() {
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-10.0, 10.0, -10.0, 10.0);
	glEnable(GL_LINE_SMOOTH); // Habilitar suavizado de líneas
}

void drawGraph(const Grafo& grafo, const vector<int>& colores) {
	glPointSize(8.0);
	
	for (int i = 0; i < grafo.numVertices(); i++) {
		float x = static_cast<float>(i * 2 - 6);
		float y = 0.0;
		if (i >= 3) y = -5.0;
		
		// Dibujar el vértice como un círculo
		glBegin(GL_POLYGON);
		if (colores[i] == 0)
			glColor3f(1.0, 0.0, 0.0); // Rojo
		else if (colores[i] == 1)
			glColor3f(0.0, 1.0, 0.0); // Verde
		else if (colores[i] == 2)
			glColor3f(1.0, 1.0, 0.0); // Amarillo

		else
			glColor3f(0.5, 0.5, 0.5); // Sin asignar
		for (int j = 0; j < 360; j++) {
			float theta = j * M_PI / 180;
			float xi = x + 0.5 * cos(theta);
			float yi = y + 0.5 * sin(theta);
			glVertex2f(xi, yi);
		}
		glEnd();
		
		// Dibujar la etiqueta del vértice
		glColor3f(0.0, 0.0, 0.0);
		glRasterPos2f(x - 0.1, y - 0.1);
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, '0' + i);
		
		// Dibujar las aristas
		glColor3f(0.0, 0.0, 1.0);
		for (int verticeAdy : grafo.adyacentes(i)) {
			float x2 = static_cast<float>(verticeAdy * 2 - 6);
			float y2 = 0.0;
			if (verticeAdy >= 3) y2 = -5.0;
			glBegin(GL_LINES);
			glVertex2f(x, y);
			glVertex2f(x2, y2);
			glEnd();
		}
	}
	
	glFlush();
}

void display() {
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0.0, 0.0, 0.0); // Establecer el color de dibujo a negro
	
	vector<int> colores1(grafo1.numVertices(), -1);
	vector<int> colores2(grafo2.numVertices(), -1);
	

	
	// Colorear los grafos
	grafo2.colorearPorMRV(3, colores2);
	
	// Mostrar los grafos coloreados
	glViewport(0, 0, 400, 400);
	drawGraph(grafo2, colores2);
	
	grafo2.colorearPorMCU(3, colores1);
	
	// Mostrar los grafos coloreados
	glViewport(400, 0, 400, 400);
	drawGraph(grafo2, colores1);

	
	glutSwapBuffers();
}

int main(int argc, char** argv) {
	// Grafo 1
	grafo1.agregarArista(0, 1);
	grafo1.agregarArista(0, 3);
	grafo1.agregarArista(0, 4);
	grafo1.agregarArista(1, 2);
	grafo1.agregarArista(2, 3);
	grafo1.agregarArista(3, 4);
	grafo1.agregarArista(2, 4);

	grafo12.agregarArista(0, 1);
	grafo12.agregarArista(0, 3);
	grafo12.agregarArista(0, 4);
	grafo12.agregarArista(1, 2);
	grafo12.agregarArista(2, 3);
	grafo12.agregarArista(3, 4);
	grafo12.agregarArista(2, 4);
	
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
	
	
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(800, 400);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Coloreo de Grafos con OpenGL");
	init();
	glutDisplayFunc(display);
	glutMainLoop();
	return 0;
}
