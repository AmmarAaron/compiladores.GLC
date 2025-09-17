#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cctype>
#include <locale>
#include <clocale>
using namespace std;

// Funciones auxiliares
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
    for (char c : t) {
        if (!isdigit((unsigned char)c)) return false;
    }
    return true;
}

bool esOperador(const string &t) {
    return (t == "+" || t == "-" || t == "*" || t == "/" ||
            t == ">" || t == "<" || t == "==");
}

// Funcion para validar expresion aritmetica con paréntesis y operadores

bool esExpresionRec(const vector<string> &tokens, size_t inicio, size_t fin, size_t &posFin) {
	
    // Esta funcion valida una expresion en tokens[inicio..fin-1]
    // posFin devuelve hasta donde se reconocio la expresion

    int balanceParentesis = 0;
    bool esperandoOperando = true;  // Si esperamos un operando (identificador o numero o subexpresion)
    size_t i = inicio;

    while (i < fin) {
        const string &tok = tokens[i];

        if (tok == "(") {
            // abrir paréntesis: balanceamos
            balanceParentesis++;
            size_t subFin;
            if (!esExpresionRec(tokens, i + 1, fin, subFin)) return false;
            i = subFin;
            if (i >= fin || tokens[i] != ")") return false;
            balanceParentesis--;
            i++;
            esperandoOperando = false;
        }
        else if (esperandoOperando) {
            // Esperamos operando: debe ser numero o identificador
            if (esNumero(tok) || esIdentificador(tok)) {
                esperandoOperando = false;
                i++;
            } else {
                return false;
            }
        }
        else {
            // Esperamos operador o fin expresion
            if (esOperador(tok)) {
                esperandoOperando = true;
                i++;
            } else {
                // no es operador => fin expresion
                break;
            }
        }
    }

    if (balanceParentesis != 0) return false;
    if (esperandoOperando) return false;  // termino esperando un operando (incompleto)

    posFin = i;
    return true;
}

// Wrapper para validar toda la expresion

bool esExpresion(const vector<string> &tokens) {
    size_t posFin = 0;
    if (!esExpresionRec(tokens, 0, tokens.size(), posFin)) return false;
    
    // la expresion debe cubrir todos los tokens
    
    return (posFin == tokens.size());
}

bool esDeclaracion(const vector<string> &tokens) {
    return (tokens.size() == 3 &&
            (tokens[0] == "int" || tokens[0] == "float") &&
            esIdentificador(tokens[1]) &&
            tokens[2] == ";");
}

bool esSentenciaSimple(const vector<string> &tokens) {
	
    // Identificador + ;
    if (tokens.size() == 2 && esIdentificador(tokens[0]) && tokens[1] == ";")
        return true;

    // Declaracion
    if (esDeclaracion(tokens)) return true;

    // Expresion + ;
    if (tokens.size() >= 3 && tokens.back() == ";") {
        vector<string> expr(tokens.begin(), tokens.end() - 1);
        return esExpresion(expr);
    }

    return false;
}

bool esCondicional(const vector<string> &tokens) {
    if (tokens.size() < 5 || tokens[0] != "if" || tokens[1] != "(") return false;

    size_t posCierre = 0;
    for (size_t i = 2; i < tokens.size(); i++) {
        if (tokens[i] == ")") {
            posCierre = i;
            break;
        }
    }
    if (posCierre == 0) return false;

    vector<string> condicion(tokens.begin() + 2, tokens.begin() + posCierre);
    bool condicionValida = false;

    if (condicion.size() == 1 &&
        (esIdentificador(condicion[0]) || esNumero(condicion[0]))) {
        condicionValida = true;
    } else if (esExpresion(condicion)) {
        condicionValida = true;
    }

    if (!condicionValida) return false;

    if (posCierre + 1 >= tokens.size() || tokens[posCierre + 1] != "{") return false;

    size_t posCierreBloque = 0;
    for (size_t i = posCierre + 2; i < tokens.size(); i++) {
        if (tokens[i] == "}") {
            posCierreBloque = i;
            break;
        }
    }
    if (posCierreBloque == 0) return false;

    vector<string> bloque(tokens.begin() + posCierre + 2, tokens.begin() + posCierreBloque);
    if (!esSentenciaSimple(bloque)) return false;

    if (posCierreBloque + 1 < tokens.size() && tokens[posCierreBloque + 1] == "else") {
        if (posCierreBloque + 2 >= tokens.size() || tokens[posCierreBloque + 2] != "{") return false;

        size_t posCierreElse = 0;
        for (size_t i = posCierreBloque + 3; i < tokens.size(); i++) {
            if (tokens[i] == "}") {
                posCierreElse = i;
                break;
            }
        }
        if (posCierreElse == 0) return false;

        vector<string> bloqueElse(tokens.begin() + posCierreBloque + 3, tokens.begin() + posCierreElse);
        if (!esSentenciaSimple(bloqueElse)) return false;
    }

    return true;
}

vector<string> tokenizar(const string &linea) {
    vector<string> tokens;
    string actual = "";

    for (size_t i = 0; i < linea.size(); i++) {
        char c = linea[i];

        if (isspace((unsigned char)c)) {
            if (!actual.empty()) {
                tokens.push_back(actual);
                actual.clear();
            }
        }
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

bool esComentario(const string &linea) {
    string trimmed = linea;
    while (!trimmed.empty() && isspace(trimmed[0])) {
        trimmed.erase(0, 1);
    }

    if (trimmed.size() >= 2) {
        if (trimmed[0] == '/' && trimmed[1] == '/') return true;
        if (trimmed.size() >= 4 && trimmed.substr(0, 2) == "/*" && trimmed.substr(trimmed.size() - 2, 2) == "*/") return true;
    }
    return false;
}

int main() {
    setlocale(LC_ALL, "");

    ifstream archivo("entrada.txt");
    if (!archivo) {
        cout << "No se pudo abrir el archivo entrada.txt" << endl;
        return 1;
    }

    string linea;
    while (getline(archivo, linea)) {
        if (linea.empty()) continue;

        cout << "Cadena: " << linea << " -> ";

        if (esComentario(linea)) {
            cout << "Comentario valido";
        }
        else {
            vector<string> tokens = tokenizar(linea);

            if (esDeclaracion(tokens)) {
                cout << "Declaracion valida";
            }
            else if (esCondicional(tokens)) {
                cout << "Condicional valido";
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
        }
        cout << endl;
    }

    archivo.close();
    return 0;
}
