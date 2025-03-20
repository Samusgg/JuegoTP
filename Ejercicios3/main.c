#include <stdio.h>
#include <stdlib.h>
/**
EJERCICIO 1

Metodo para intercambiar enteros.
*/
void intercambia(int * a, int * b) {
    int aux = *a;
    *a = *b;
    *b = aux;
}
/**
EJERCICIO 2
*/
int * copia (int datos[], int n) {
    //Calcular cuanta memoria
    //Reservar la memoria
    //Rellenar la memoria
    //Devolver la memoria
    int * copiado = malloc(n*sizeof(int));

    for(int i = 0; i<n; i++) {
        copiado[i] = datos[i];
    }
    return copiado;
}

/**
EJERCICIO 3

Metodo que genera numeros aleatorios entre [a,b]
*/
int * genera (int n, int a, int b) {
    int * nuevo = malloc(n*sizeof(int));
    for(int i = 0; i<n; i++) {
        nuevo[i] = rand() %  (b-a+1)+a;
    }
    return nuevo;
}

/**
EJERCICIO 4
*/
int * invierte(int datos[], int max) {
    //Copiado
    int * nuevo = malloc(max*sizeof(int));
    for(int i = 0, n = max; i<max; i++, n--) {
        nuevo[i] = datos[n-1];
    }
    return nuevo;
}

/**
EJERCICIO 8
*/
char * genera(char * letras, int n) {
     char * texto = malloc((n+1)*sizeof(char));
     int tamanyo = 0;
     while(letras[tamanyo]!= '\0'){
        tamanyo++;
     }
     for(int i = 0; i<n; i++){
        texto[i] = letras[rand()%tamanyo];
     }
      texto[n+1] = '\0';
      return
}

/**

*/
int * positivos(int datos[], int n){
    int cantidad = 0;
    for(int i=0; i<n; i++){
        if(datos[i]>0){
            cantidad++;
        }
    }
    int * nuevo = malloc((cantidad+1)*sizeof(int));

    for(int i = 0, nIndice = 0; i<n; i++){
        if(datos[i]>0){
            nuevo[nIncice] = datos[i];
            nIndice++;
        }
    }
    nuevo[cantidad] = -1;
    return nuevo;
}


/**
Metodo principal.
*/
int main() {

printf("Hola");
    return 0;
}
