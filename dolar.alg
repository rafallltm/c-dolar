Algoritmo "ConversorReaisParaDolares"
Var
   valor_real, cotacao_dolar, convert: Real

Inicio
   Escreval("Programa para converter reais em dolares")
   Escreval("1 Digite o valor disponivel em reais: ")
   Leia(valor_real)
   Escreval("2- Digite o valor da cotacao do dolar no dia: ")
   Leia(cotacao_dolar)

   convert <- valor_real / cotacao_dolar

   Escreva("O valor em dolares em $:", convert)
FimAlgoritmo
