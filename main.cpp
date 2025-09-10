#include <iostream>
#include <string>
#include <cctype>
using namespace std;

// Comprueba identificador (seguro: revisa si está vacío)
bool esIdentificador(const string &t) {
    if (t.size() == 0) return false;
    if (!isalpha((unsigned char)t[0])) return false;
    for (size_t i = 1; i < t.size(); ++i) {
        if (!isalnum((unsigned char)t[i])) return false;
    }
    return true;
}

bool esNumero(const string &t) {
    if (t.size() == 0) return false;
    for (size_t i = 0; i < t.size(); ++i) {
        if (!isdigit((unsigned char)t[i])) return false;
    }
    return true;
}

bool esPalabraReservada(const string &t) {
    return (t == "int" || t == "float" || t == "if" || t == "else");
}

bool esOperador(const string &t) {
    return (t == "+" || t == "-" || t == "*" || t == "/" ||
            t == ">" || t == "<" || t == "==" );
}

// Declaración simple: tipo identificador ;
bool esDeclaracionTokens(const string &t1, const string &t2, const string &t3) {
    if ((t1 == "int" || t1 == "float") && esIdentificador(t2) && t3 == ";")
        return true;
    return false;
}

int main() {
    // Cada fila tiene hasta 5 tokens (los strings vacíos son ignorados)
    string ejemplos[5][5] = {
        {"int", "x", ";", "", ""},           // declaración
        {"x1", "", "", "", ""},              // identificador
        {"2", "+", "3", "", ""},             // expresión aritmética (simplificada)
        {"if", "(", "x", ">", "0)"},
        {"error$", "", "", "", ""}           // no válido
    };

    int n = 5; // filas

    for (int i = 0; i < n; ++i) {
        cout << "Cadena: ";
        // imprimir tokens (evitar range-based for)
        for (int j = 0; j < 5; ++j) {
            if (ejemplos[i][j] != "") cout << ejemplos[i][j] << " ";
        }

        cout << "-> ";

        // tomamos los primeros tokens para las comprobaciones simples
        string t1 = ejemplos[i][0];
        string t2 = ejemplos[i][1];
        string t3 = ejemplos[i][2];

        if (esDeclaracionTokens(t1, t2, t3)) {
            cout << "Declaracion valida";
        }
        else if (t1 == "if") {
            cout << "Condicional (detected, simplificado)";
        }
        else if (esIdentificador(t1)) {
            cout << "Identificador valido";
        }
        else if (esNumero(t1) || esOperador(t2)) {
            cout << "Expresion aritmetica valida";
        }
        else {
            cout << "No valido";
        }

        cout << endl;
    }

    return 0;
}
