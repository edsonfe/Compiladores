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
    int value;  // Valor v�lido apenas para tokens num�ricos
} Token;

Token currentToken;
char* inputExpression;
int currentIndex = 0;

// Fun��o para obter o pr�ximo caractere da express�o
char getNextChar() {
    return inputExpression[currentIndex++];
}

// Fun��o para obter o pr�ximo token da sequ�ncia
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

                // Verificar se h� mais d�gitos num�ricos
                ch = getNextChar();
                while (isdigit(ch)) {
                    token.value = token.value * 10 + (ch - '0');
                    ch = getNextChar();
                }

                // Voltar um caractere se n�o for um d�gito num�rico
                currentIndex--;
            } else {
                token.type = ID;
            }
            break;
    }

    return token;
}

// Fun��o para imprimir mensagens de erro
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

// Fun��o para consumir um token da sequ�ncia
void match(TokenType expectedType) {
    if (currentToken.type == expectedType) {
        currentToken = getNextToken();
    } else {
        printError("Token inesperado");
        hasError = true;
        exit(1);
    }
}

// Fun��es para an�lise sint�tica

void fator();
void mult();
void termo();
void soma();
void exp();

// Fun��o para lidar com a varredura � frente para encontrar m�ltiplos erros
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

    // Remover o caractere de nova linha (\n) da express�o
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
