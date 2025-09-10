#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cctype>
using namespace std;

// ====== Funciones auxiliares ======
bool esIdentificador(const string &t) {
    if (t.empty()) return false;
    if (!isalpha((unsigned char)t[0])) return false;
    for (size_t i = 1; i < t.size(); ++i) {
        if (!isalnum((unsigned char)t[i])) return false;
    }
    return true;
}

bool esNumero(const string &t) {
    if (t.empty()) return false;
    for (size_t i = 0; i < t.size(); ++i) {
        if (!isdigit((unsigned char)t[i])) return false;
    }
    return true;
}

bool esOperador(const string &t) {
    return (t == "+" || t == "-" || t == "*" || t == "/" ||
            t == ">" || t == "<" || t == "==");
}

// ====== Reglas gramaticales ======
bool esDeclaracion(const vector<string> &tokens) {
    return (tokens.size() == 3 &&
            (tokens[0] == "int" || tokens[0] == "float") &&
            esIdentificador(tokens[1]) &&
            tokens[2] == ";");
}

bool esExpresion(const vector<string> &tokens) {
    return (tokens.size() == 3 &&
            esNumero(tokens[0]) &&
            esOperador(tokens[1]) &&
            esNumero(tokens[2]));
}

bool esCondicional(const vector<string> &tokens) {
    return (tokens.size() >= 5 &&
            tokens[0] == "if" &&
            tokens[1] == "(" &&
            (esIdentificador(tokens[2]) || esNumero(tokens[2])) &&
            esOperador(tokens[3]) &&
            (esIdentificador(tokens[4]) || esNumero(tokens[4])));
}

// ====== Tokenizar con separación de símbolos ======
vector<string> tokenizar(const string &linea) {
    vector<string> tokens;
    string actual = "";

    for (size_t i = 0; i < linea.size(); i++) {
        char c = linea[i];

        // Si es espacio, guardar lo acumulado
        if (isspace((unsigned char)c)) {
            if (!actual.empty()) {
                tokens.push_back(actual);
                actual.clear();
            }
        }
        // Si es símbolo especial (;, (), {})
        else if (c == ';' || c == '(' || c == ')' || c == '{' || c == '}') {
            if (!actual.empty()) {
                tokens.push_back(actual);
                actual.clear();
            }
            string simbolo(1, c);
            tokens.push_back(simbolo);
        }
        else {
            actual += c;
        }
    }

    if (!actual.empty()) tokens.push_back(actual);
    return tokens;
}

// ====== Programa principal ======
int main() {
    ifstream archivo("entrada.txt");
    if (!archivo) {
        cout << "No se pudo abrir el archivo entrada.txt" << endl;
        return 1;
    }

    string linea;
    while (getline(archivo, linea)) {
        if (linea.empty()) continue;

        cout << "Cadena: " << linea << " -> ";

        vector<string> tokens = tokenizar(linea);

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
    return 0;
}
