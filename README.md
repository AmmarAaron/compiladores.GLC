# compiladores.GLC

# Proyecto: Analizador de Sintaxis C++

## Descripción
Implementación de gramáticas libres de contexto (GLC) para reconocer instrucciones en C++.

## Gramáticas
- Expresiones aritméticas
- Identificadores
- Declaraciones
- Condicionales

## Ejemplos probados
```cpp
for (int i = 1; i <= 5; i++) {
    std::cout << "Número: " << i << std::endl;
}

if (numero > 0) {
    std::cout << "El número es positivo." << std::endl;
} else {
    std::cout << "El número es menor que cero." << std::endl;
}
