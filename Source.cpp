#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <string>

using namespace std;


const int INF = 999999;

void limpiarPantalla() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void pausar() {
    cout << "\n  [Presiona Enter para continuar...]";
    cin.ignore(10000, '\n');
    cin.get();
}

void linea(char c = '-', int largo = 60) {
    cout << string(largo, c) << "\n";
}

int leerEntero(const string& msg, int minimo, int maximo) {
    int v;
    while (true) {
        cout << msg;
        if (cin >> v && v >= minimo && v <= maximo) {
            cin.ignore();
            return v;
        }
        cin.clear();
        cin.ignore(10000, '\n');
        cout << "  !! Valor invalido. Ingresa un entero entre "
            << minimo << " y " << maximo << ".\n";
    }
}

struct Grafo {
    int n;
    vector<vector<int>> mat;

    Grafo(int n) : n(n), mat(n, vector<int>(n, INF)) {
        for (int i = 0; i < n; i++) mat[i][i] = 0;
    }

    void agregarArista(int u, int v, int w) {
        mat[u][v] = w;
        mat[v][u] = w;
    }

    bool tieneArista(int u, int v) const {
        return mat[u][v] != INF;
    }
};


void mostrarMatriz(const Grafo& g) {
    cout << "\n  MATRIZ DE COSTOS (INF = sin conexion):\n\n";
    cout << "       ";
    for (int j = 0; j < g.n; j++)
        cout << setw(6) << j;
    cout << "\n       ";
    linea('-', 6 * g.n + 1);

    for (int i = 0; i < g.n; i++) {
        cout << "  " << setw(3) << i << " | ";
        for (int j = 0; j < g.n; j++) {
            if (g.mat[i][j] == INF)
                cout << setw(6) << "INF";
            else
                cout << setw(6) << g.mat[i][j];
        }
        cout << "\n";
    }
    cout << "\n";
}

void mostrarGrafo(const Grafo& g) {
    cout << "\n  LISTA DE ARISTAS DEL GRAFO:\n\n";
    bool hayArista = false;
    for (int i = 0; i < g.n; i++) {
        for (int j = i + 1; j < g.n; j++) {
            if (g.tieneArista(i, j)) {
                cout << "    Nodo " << i << " <--[" << setw(3)
                    << g.mat[i][j] << "]--> Nodo " << j << "\n";
                hayArista = true;
            }
        }
    }
    if (!hayArista) cout << "    (sin aristas)\n";
    cout << "\n";
}

void mostrarRutaResaltada(const Grafo& g, const vector<int>& ciclo) {
    vector<pair<int, int>> aristasOptimas;
    for (int k = 0; k + 1 < (int)ciclo.size(); k++) {
        int u = min(ciclo[k], ciclo[k + 1]);
        int v = max(ciclo[k], ciclo[k + 1]);
        aristasOptimas.push_back({ u, v });
    }

    cout << "\n  GRAFO CON RUTA OPTIMA RESALTADA [***]:\n\n";
    for (int i = 0; i < g.n; i++) {
        for (int j = i + 1; j < g.n; j++) {
            if (g.tieneArista(i, j)) {
                int u = min(i, j), v = max(i, j);
                bool esOptima = false;
                for (auto& p : aristasOptimas)
                    if (p.first == u && p.second == v) { esOptima = true; break; }

                if (esOptima)
                    cout << "  *** Nodo " << i << " <--[" << setw(3)
                    << g.mat[i][j] << "]--> Nodo " << j << " ***\n";
                else
                    cout << "      Nodo " << i << " <--[" << setw(3)
                    << g.mat[i][j] << "]--> Nodo " << j << "\n";
            }
        }
    }
    cout << "\n";
}


void generarManual(Grafo& g) {
    cout << "\n  GENERACION MANUAL\n";
    cout << "  Nodos disponibles: 0 a " << g.n - 1 << "\n";
    cout << "  Ingresa las aristas (peso entre 1 y 999).\n";
    cout << "  Para terminar ingresa -1 -1 -1\n\n";

    while (true) {
        int u, v, w;
        cout << "  Arista (u v peso): ";
        cin >> u;
        if (u == -1) { cin.ignore(); break; }
        cin >> v >> w;
        cin.ignore();

        if (u < 0 || u >= g.n || v < 0 || v >= g.n || u == v) {
            cout << "  !! Nodos invalidos.\n"; continue;
        }
        if (w < 1 || w > 999) {
            cout << "  !! Peso debe estar entre 1 y 999.\n"; continue;
        }
        if (g.tieneArista(u, v)) {
            cout << "  !! Esa arista ya existe (peso " << g.mat[u][v] << ").\n"; continue;
        }
        g.agregarArista(u, v, w);
        cout << "    -> Arista agregada: " << u << " -- " << v
            << " (peso " << w << ")\n";
    }
}

void generarAleatorio(Grafo& g) {
    srand((unsigned)time(nullptr));
    cout << "\n  Generando grafo aleatorio...\n";

    vector<int> perm(g.n);
    for (int i = 0; i < g.n; i++) perm[i] = i;
    for (int i = g.n - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        swap(perm[i], perm[j]);
    }
    for (int i = 0; i < g.n; i++) {
        int u = perm[i], v = perm[(i + 1) % g.n];
        int w = 10 + rand() % 91;
        g.agregarArista(u, v, w);
    }

    for (int i = 0; i < g.n; i++) {
        for (int j = i + 1; j < g.n; j++) {
            if (!g.tieneArista(i, j) && (rand() % 10) < 6) {
                int w = 10 + rand() % 91;
                g.agregarArista(i, j, w);
            }
        }
    }
    cout << "  Grafo generado exitosamente.\n";
}


bool verificarHamiltoniano(const Grafo& g) {
    vector<int> grado(g.n, 0);
    for (int i = 0; i < g.n; i++)
        for (int j = 0; j < g.n; j++)
            if (i != j && g.tieneArista(i, j)) grado[i]++;

    bool ok = true;
    cout << "\n  VERIFICACION DE GRADOS (minimo requerido: 2):\n\n";
    for (int i = 0; i < g.n; i++) {
        cout << "    Nodo " << i << ": grado = " << grado[i];
        if (grado[i] < 2) { cout << "  !! INSUFICIENTE"; ok = false; }
        cout << "\n";
    }

    if (!ok) {
        cout << "\n  ARISTAS SUGERIDAS PARA COMPLETAR EL GRAFO:\n";
        for (int i = 0; i < g.n; i++) {
            if (grado[i] < 2) {
                for (int j = 0; j < g.n; j++) {
                    if (i != j && !g.tieneArista(i, j)) {
                        cout << "    -> Agregar arista (" << i << ", " << j << ")\n";
                        break;
                    }
                }
            }
        }
        return false;
    }

    vector<bool> visitado(g.n, false);
    vector<int> cola;
    cola.push_back(0);
    visitado[0] = true;
    int idx = 0;
    while (idx < (int)cola.size()) {
        int u = cola[idx++];
        for (int v = 0; v < g.n; v++)
            if (!visitado[v] && g.tieneArista(u, v)) {
                visitado[v] = true;
                cola.push_back(v);
            }
    }
    for (int i = 0; i < g.n; i++) {
        if (!visitado[i]) {
            cout << "\n  !! El grafo NO es conexo. Nodo " << i
                << " no tiene camino al resto.\n";
            cout << "     Sugerencia: agregar arista (0, " << i << ")\n";
            return false;
        }
    }

    cout << "\n  Grafo apto para buscar ciclos hamiltonianos.\n";
    return true;
}


struct ResultadoTSP {
    vector<vector<int>> todosCiclos;
    vector<int>         costos;
    int                 indicOptimo;
};


ResultadoTSP fuerzaBruta(const Grafo& g, bool modoPasoAPaso) {
    ResultadoTSP res;
    res.indicOptimo = -1;
    int mejorCosto = INF;

    vector<int> perm(g.n - 1);
    for (int i = 0; i < g.n - 1; i++) perm[i] = i + 1;

    int numeroPerm = 0;

    cout << "\n";
    linea('=');
    cout << "  EXPLORACION PASO A PASO — FUERZA BRUTA\n";
    linea('=');
    cout << "  Nodo fijo de inicio/fin: 0\n\n";

    do {
        numeroPerm++;


        vector<int> ciclo;
        ciclo.push_back(0);
        for (int x : perm) ciclo.push_back(x);
        ciclo.push_back(0);

        int costo = 0;
        bool valido = true;
        for (int k = 0; k + 1 < (int)ciclo.size(); k++) {
            int u = ciclo[k], v = ciclo[k + 1];
            if (!g.tieneArista(u, v)) { valido = false; break; }
            costo += g.mat[u][v];
        }

        cout << "  Permutacion #" << numeroPerm << ": ";
        for (int k = 0; k < (int)ciclo.size(); k++) {
            cout << ciclo[k];
            if (k + 1 < (int)ciclo.size()) cout << " -> ";
        }
        cout << "\n";

        if (valido) {
            cout << "    Estado : VALIDO  | Costo total: " << costo << "\n";
            res.todosCiclos.push_back(ciclo);
            res.costos.push_back(costo);
            if (costo < mejorCosto) {
                mejorCosto = costo;
                res.indicOptimo = (int)res.costos.size() - 1;
            }
        }
        else {
            cout << "    Estado : INVALIDO (arista faltante)\n";
        }

        if (modoPasoAPaso) pausar();
        else cout << "\n";

    } while (next_permutation(perm.begin(), perm.end()));

    return res;
}


void mostrarResultados(const Grafo& g, const ResultadoTSP& res) {
    limpiarPantalla();
    linea('=');
    cout << "  RESULTADOS FINALES — PROBLEMA DEL AGENTE VIAJERO\n";
    linea('=');

    if (res.todosCiclos.empty()) {
        cout << "\n  No se encontro ningun ciclo hamiltoniano valido.\n";
        return;
    }

    int total = (int)res.todosCiclos.size();
    cout << "\n  Total de ciclos hamiltonianos encontrados: " << total << "\n\n";

    cout << "  " << left << setw(6) << "#"
        << setw(10) << "Costo"
        << "Ciclo\n";
    linea('-', 60);

    for (int i = 0; i < total; i++) {
        string marca = (i == res.indicOptimo) ? " <-- OPTIMO" : "";
        cout << "  " << left << setw(6) << (i + 1)
            << setw(10) << res.costos[i];
        for (int k = 0; k < (int)res.todosCiclos[i].size(); k++) {
            cout << res.todosCiclos[i][k];
            if (k + 1 < (int)res.todosCiclos[i].size()) cout << "->";
        }
        cout << marca << "\n";
    }

    linea('=');
    cout << "\n  CICLO HAMILTONIANO OPTIMO:\n";
    cout << "    Ruta  : ";
    const auto& opt = res.todosCiclos[res.indicOptimo];
    for (int k = 0; k < (int)opt.size(); k++) {
        cout << opt[k];
        if (k + 1 < (int)opt.size()) cout << " -> ";
    }
    cout << "\n";
    cout << "    Costo : " << res.costos[res.indicOptimo] << "\n\n";

    mostrarRutaResaltada(g, opt);
}


void menuPrincipal() {
    limpiarPantalla();
    linea('*');
    linea('*');

    int n = leerEntero("\n  Ingresa el numero de nodos (5 a 10): ", 5, 10);

    Grafo g(n);

    cout << "\n  Como deseas generar el grafo?\n";
    cout << "    1. Manual\n";
    cout << "    2. Aleatorio\n";
    int modo = leerEntero("  Opcion: ", 1, 2);

    if (modo == 1) generarManual(g);
    else           generarAleatorio(g);

    limpiarPantalla();
    linea('=');
    cout << "  GRAFO GENERADO\n";
    linea('=');
    mostrarGrafo(g);
    mostrarMatriz(g);
    pausar();

    limpiarPantalla();
    linea('=');
    cout << "  VERIFICACION DE CICLO HAMILTONIANO\n";
    linea('=');

    if (!verificarHamiltoniano(g)) {
        cout << "\n  Deseas agregar las aristas sugeridas manualmente? (1=Si / 0=No): ";
        int op = leerEntero("  Opcion: ", 0, 1);
        if (op == 1) {
            generarManual(g);
            if (!verificarHamiltoniano(g)) {
                cout << "\n  El grafo sigue sin cumplir las condiciones. Saliendo...\n";
                pausar();
                return;
            }
        }
        else {
            cout << "\n  Saliendo sin ejecutar el algoritmo.\n";
            pausar();
            return;
        }
    }
    pausar();

    limpiarPantalla();
    cout << "\n  Modo de ejecucion del algoritmo:\n";
    cout << "    1. Paso a paso (presionas Enter en cada permutacion)\n";
    cout << "    2. Automatico (muestra todo de corrido)\n";
    int modoEjec = leerEntero("  Opcion: ", 1, 2);
    bool pasoAPaso = (modoEjec == 1);

    ResultadoTSP resultado = fuerzaBruta(g, pasoAPaso);

    pausar();
    mostrarResultados(g, resultado);
    pausar();
}


int main() {
    while (true) {
        menuPrincipal();

        cout << "\n  Deseas ejecutar otro caso? (1=Si / 0=No): ";
        int op = leerEntero("  Opcion: ", 0, 1);
        if (op == 0) break;
    }

    cout << "\n  Gracias por usar el programa. Hasta luego!\n\n";
    return 0;
}