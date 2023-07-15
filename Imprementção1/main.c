#include <stdio.h>

void lexico(char entrada[]) {
    int estado = 1;
    char lexema[100] = "";
    int i = 0;

    while (estado == 1 || estado == 2 || estado == 3 || estado == 4 ) {

        if(entrada[i] == '\0'){
            break;
        }

        char caracter = entrada[i];

        switch (estado) {
            case 1:
                if (caracter == '/') {
                    lexema[0] = '/';
                    estado = 2;
                } else {
                    estado = -1;  // estado de erro
                }
                break;

            case 2:
                if (caracter == '*') {
                    lexema[1] = '*';
                    estado = 3;
                } else {
                    estado = -1;  // estado de erro
                }
                break;

            case 3:
                if (caracter == '*') {
                    lexema[strlen(lexema)] = '*';
                    estado = 4;
                } else {
                    lexema[strlen(lexema)] = caracter;
                }
                break;

            case 4:
                if (caracter == '/') {
                    lexema[strlen(lexema)] = '/';
                    estado = 5; // estado de aceitação
                } else if (caracter == '*') {
                    lexema[strlen(lexema)] = '*';
                    estado = 4;
                } else {
                    lexema[strlen(lexema)] = caracter;
                    estado = 3;
                }
                break;
        }

        i++;
    }

    if (estado == 5) { // estado de aceitação
        printf("%s", lexema);
    } else {
        printf("Nenhum token foi reconhecido.\n");
    }
}

int main() {
    char entrada[100];

    printf("Entrada: ");
    scanf("%s", &entrada);
    lexico(entrada);
    return 0;
}
