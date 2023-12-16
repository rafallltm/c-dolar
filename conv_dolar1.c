#include <stdio.h>

int main(){
    float valor_real, cotacao_dolar, convert;

        printf("Programa para converter reais em dolares: \n ");
        printf("1-Digitar o valor disponivel em reais. ");
        scanf("%f", &valor_real);
        printf("2- Digitar o valor da cotacao do dolar no dia ");
        scanf("%f", &cotacao_dolar);

        convert = valor_real / cotacao_dolar;

        printf("2$%.2f\n", convert);

    return 0;
}
