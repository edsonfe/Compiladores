#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>

bool hasError = false;

typedef enum {
    ID,
    NUM,
    PLUS,
    MINUS,
    MULT,
    LPAREN,
    RPAREN,
    END
} TokenType;

typedef struct {
    TokenType type;
    int value;  // Valor válido apenas para tokens numéricos
} Token;

Token currentToken;
char* inputExpression;
int currentIndex = 0;

// Função para obter o próximo caractere da expressão
char getNextChar() {
    return inputExpression[currentIndex++];
}

// Função para obter o próximo token da sequência
Token getNextToken() {
    char ch = getNextChar();

    while (isspace(ch)) {
        ch = getNextChar();
    }

    Token token;

    switch (ch) {
        case '+':
            token.type = PLUS;
            break;
        case '-':
            token.type = MINUS;
            break;
        case '*':
            token.type = MULT;
            break;
        case '(':
            token.type = LPAREN;
            break;
        case ')':
            token.type = RPAREN;
            break;
        case '\0':
            token.type = END;
            break;
        default:
            if (isdigit(ch)) {
                token.type = NUM;
                token.value = ch - '0';

                // Verificar se há mais dígitos numéricos
                ch = getNextChar();
                while (isdigit(ch)) {
                    token.value = token.value * 10 + (ch - '0');
                    ch = getNextChar();
                }

                // Voltar um caractere se não for um dígito numérico
                currentIndex--;
            } else {
                token.type = ID;
            }
            break;
    }

    return token;
}

// Função para imprimir mensagens de erro
void printError(const char* message) {
    printf("Erro ");

    switch (currentToken.type) {
        case ID:
            printf("'ID'");
            break;
        case NUM:
            printf("'NUM'");
            break;
        case PLUS:
            printf("'+'");
            break;
        case MINUS:
            printf("'-'");
            break;
        case MULT:
            printf("'*'");
            break;
        case LPAREN:
            printf("'('");
            break;
        case RPAREN:
            printf("')'");
            break;
        case END:
            printf("Fim de expressao");
            break;
    }
    printf(".%s\n", message);

    hasError = true;

}

// Função para consumir um token da sequência
void match(TokenType expectedType) {
    if (currentToken.type == expectedType) {
        currentToken = getNextToken();
    } else {
        printError("Token inesperado");
        hasError = true;
        exit(1);
    }
}

// Funções para análise sintática

void fator();
void mult();
void termo();
void soma();
void exp();

// Função para lidar com a varredura à frente para encontrar múltiplos erros
void modoPanico(const TokenType* syncTokens, int numSyncTokens) {
    while (currentToken.type != EOF) {
        for (int i = 0; i < numSyncTokens; i++) {
            if (currentToken.type == syncTokens[i]) {
                return;
            }
        }

        currentToken = getNextToken();
    }
}

void fator() {
    if (currentToken.type == LPAREN) {
        match(LPAREN);
        exp();
        match(RPAREN);
    } else if (currentToken.type == NUM) {
        match(NUM);
    } else {
        printError("Esperado ID ou NUM");
        hasError = true;
        const TokenType syncTokens[] = {PLUS, MINUS, MULT, LPAREN, RPAREN, END};
        modoPanico(syncTokens, sizeof(syncTokens) / sizeof(syncTokens[0]));
    }
}

void mult() {
    if (currentToken.type == MULT) {
        match(MULT);
        fator();
        mult();
    }
}

void termo() {
    fator();
    mult();
}

void soma() {
    if (currentToken.type == PLUS || currentToken.type == MINUS) {
        match(currentToken.type);
        termo();
        soma();
    }
}

void exp() {
    termo();
    soma();
}

int main() {

    printf("Digite a expressao aritmetica: ");

    inputExpression = malloc(sizeof(char) * 100);
    fgets(inputExpression, 100, stdin);

    // Remover o caractere de nova linha (\n) da expressão
    inputExpression[strcspn(inputExpression, "\n")] = '\0';

    currentToken = getNextToken();

    exp();

    if (currentToken.type != END) {
        printError("Fim de expressao esperado");
    }
    if (hasError){
        printf("expressao sintaticamente incorreta");
    }else{
        printf("expressao sintaticamente correta");
    }

    free(inputExpression);

    return 0;
}
