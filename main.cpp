#include <iostream>
#include <fstream>
#include <string>
#include <cctype>
using namespace std;

// 1. Identificador: empieza con letra, puede tener letras o dígitos
bool esIdentificador(const string& s) {
    if (s.empty() || !isalpha(s[0])) return false;
    for (char c : s) {
        if (!isalnum(c)) return false;
    }
    return true;
}

// 2. Expresión aritmética simple: solo dígitos, +, *, y paréntesis
bool esExpresionAritmetica(const string& s) {
    for (char c : s) {
        if (!(isdigit(c) || c=='+' || c=='*' || c=='(' || c==')' || isspace(c)))
            return false;
    }
    return true;
}

// 3. Declaración: ej. "int x;" o "float y;"
bool esDeclaracion(const string& s) {
    if (s.rfind("int ", 0) == 0 || s.rfind("float ", 0) == 0) {
        size_t pos = s.find(' ');
        string id = s.substr(pos + 1, s.size() - pos - 2); // quitar ";"
        if (s.back() == ';' && esIdentificador(id)) {
            return true;
        }
    }
    return false;
}

// 4. Condicional: if(x){x;} o if(x){x;}else{x;}
bool esCondicional(const string& s) {
    if (s == "if(x){x;}" || s == "if(x){x;}else{x;}") {
        return true;
    }
    return false;
}

int main() {
    ifstream archivo("entrada.txt"); // 📌 archivo con las líneas a validar
    string linea;

    if (!archivo.is_open()) {
        cout << "❌ No se pudo abrir el archivo entrada.txt\n";
        return 1;
    }

    while (getline(archivo, linea)) {
        cout << "Probando: " << linea << endl;

        if (esDeclaracion(linea)) {
            cout << "✔ Es una declaracion valida\n";
        }
        else if (esCondicional(linea)) {
            cout << "✔ Es un condicional valido\n";
        }
        else if (esIdentificador(linea)) {
            cout << "✔ Es un identificador valido\n";
        }
        else if (esExpresionAritmetica(linea)) {
            cout << "✔ Es una expresion aritmetica valida\n";
        }
        else {
            cout << "❌ No pertenece a ninguna de las gramaticas\n";
        }
        cout << "----------------------\n";
    }

    archivo.close();
    return 0;
}
