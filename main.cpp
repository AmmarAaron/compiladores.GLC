#include <iostream>
#include <string>
#include <cctype>
using namespace std;

// Función para verificar si un token es un identificador
bool esIdentificador(string t) {
    if (!isalpha(t[0])) return false; 
    for (int i = 1; i < t.size(); i++) {
        if (!isalnum(t[i])) return false;
    }
    return true;
}

// Verificar si un token es número
bool esNumero(string t) {
    for (char c : t) {
        if (!isdigit(c)) return false;
    }
    return true;
}

// Verificar si es palabra reservada (ejemplo básico)
bool esPalabraReservada(string t) {
    return (t == "int" || t == "if" || t == "else");
}

// Verificar si es operador
bool esOperador(string t) {
    return (t == "+" || t == "-" || t == "*" || t == "/" || t == ">");
}

// Verificar si es símbolo especial
bool esSimbolo(string t) {
    return (t == ";" || t == "{" || t == "}");
}

// Analizar la regla de declaración: int x;
bool esDeclaracion(string t1, string t2, string t3) {
    return (esPalabraReservada(t1) && esIdentificador(t2) && t3 == ";");
}

// Analizar la regla de condicional: if (x > 0) { ... }
bool esCondicional(string t1) {
    return (t1 == "if");
}

int main() {
    // Ejemplos de prueba (cada token separado con espacio)
    string ejemplos[][5] = {
        {"int", "x", ";"},           // declaración
        {"x1"},                      // identificador
        {"2", "+", "3"},             // expresión aritmética
        {"if", "(", "x", ">", "0)"}, // condicional (simplificada)
        {"error$"}                   // no válido
    };

    int n = 5; // cantidad de pruebas

    for (int i = 0; i < n; i++) {
        cout << "Cadena: ";
        for (string token : ejemplos[i]) {
            if (token != "") cout << token << " ";
        }
        cout << "-> ";

        // Tomar los primeros tokens (para simplificar)
        string t1 = ejemplos[i][0];
        string t2 = ejemplos[i][1];
        string t3 = ejemplos[i][2];

        if (esDeclaracion(t1, t2, t3)) {
            cout << "Declaracion valida";
        } else if (esCondicional(t1)) {
            cout << "Condicional valida";
        } else if (esIdentificador(t1)) {
            cout << "Identificador valido";
        } else if (esNumero(t1) || esOperador(t2)) {
            cout << "Expresion aritmetica valida";
        } else {
            cout << "No valido";
        }
        cout << endl;
    }

    return 0;
}
