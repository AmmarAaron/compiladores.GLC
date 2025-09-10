#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cctype>
using namespace std;

// Verifica identificador
bool esIdentificador(const string &t) {
    if (t.empty()) return false;
    if (!isalpha((unsigned char)t[0])) return false;
    for (size_t i = 1; i < t.size(); ++i) {
        if (!isalnum((unsigned char)t[i])) return false;
    }
    return true;
}

// Verifica número (solo dígitos)
bool esNumero(const string &t) {
    if (t.empty()) return false;
    for (size_t i = 0; i < t.size(); ++i) {
        char c = t[i];
        if (!isdigit((unsigned char)c)) return false;
    }
    return true;
}

// Operadores simples
bool esOperador(const string &t) {
    if (t == "+" || t == "-" || t == "*" || t == "/" ||
        t == ">" || t == "<" || t == "==") return true;
    return false;
}

// Reglas gramaticales (muy simples)

// Declaracion: tipo identificador ;
bool esDeclaracion(const vector<string> &tokens) {
    if (tokens.size() != 3) return false;
    if ((tokens[0] == "int" || tokens[0] == "float") &&
        esIdentificador(tokens[1]) && tokens[2] == ";")
        return true;
    return false;
}

// Expresion aritmetica simple: numero operador numero
bool esExpresion(const vector<string> &tokens) {
    if (tokens.size() != 3) return false;
    if (esNumero(tokens[0]) && esOperador(tokens[1]) && esNumero(tokens[2]))
        return true;
    return false;
}

// Condicional simplificado: if ( id|num operador id|num )
bool esCondicional(const vector<string> &tokens) {
    if (tokens.size() < 5) return false;
    if (tokens[0] != "if") return false;
    if (tokens[1] != "(") return false;
    // tokens[2] := id o num, tokens[3] := operador, tokens[4] := id o num (ejemplo)
    if ((esIdentificador(tokens[2]) || esNumero(tokens[2])) &&
        esOperador(tokens[3]) &&
        (esIdentificador(tokens[4]) || esNumero(tokens[4])))
        return true;
    return false;
}

int main() {
    // Abre entrada.txt (debe estar en la misma carpeta donde ejecutas el .exe)
    ifstream archivo("entrada.txt");
    if (!archivo) {
        cout << "No se pudo abrir el archivo entrada.txt" << endl;
        return 1;
    }

    string linea;
    while (getline(archivo, linea)) {
        if (linea.empty()) continue; // saltar líneas vacías

        cout << "Cadena: " << linea << " -> ";

        // Tokenizar por espacios
        stringstream ss(linea);
        string tok;
        vector<string> tokens;
        while (ss >> tok) {
            tokens.push_back(tok);
        }

        // Validaciones (una regla por línea)
        if (esDeclaracion(tokens)) {
            cout << "Declaracion valida";
        }
        else if (esCondicional(tokens)) {
            cout << "Condicional valida";
        }
        else if (esExpresion(tokens)) {
            cout << "Expresion aritmetica valida";
        }
        else if (tokens.size() == 1 && esIdentificador(tokens[0])) {
            cout << "Identificador valido";
        }
        else {
            cout << "No valido";
        }

        cout << endl;
    }

    archivo.close();

    // Pausa para que en Windows no se cierre la consola inmediatamente
    #ifdef _WIN32
    cout << "\nPresiona ENTER para salir...";
    cin.get();
    #endif

    return 0;
}
