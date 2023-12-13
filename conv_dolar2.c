#include <stdio.h>

int main() {
    
    float valor_real, cotacao_dolar, convert;

    printf("Programa para converter reais em dolares\n");
    printf("1. Digite o valor disponivel em reais: ");
    scanf("%f", &valor_real);
    printf("2. Digite o valor da cotacao do dolar no dia: ");
    scanf("%f", &cotacao_dolar);

    if (cotacao_dolar <= 0) {
        printf("A cotacao do dolar deve ser maior que zero. Verifique a entrada.\n");
    } else {
        convert = valor_real / cotacao_dolar;
        printf("O valor em dolares: $%.2f\n", convert);
    }

    return 0;
}
