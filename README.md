# Conversor de Moedas 

Este é um aplicativo desenvolvido em C para converter valores de Reais (BRL) para Dólares (USD). Ele utiliza a [ExchangeRate-API](https://www.exchangerate-api.com)  para obter taxas de câmbio em tempo real e fornecer conversões precisas.


## Funcionalidades


✔️ Conversão atual dos valores BRL para USD.


## Requisitos

1. **Bibliotecas Necessárias**:
  
   - `libcurl`: Biblioteca para fazer solicitações HTTP.
   
   Se estiver utilizando uma distribuição baseada em Debian:
   
   ```bash
   sudo apt install libcurl4-openssl-dev
   ```
   


2. **Chave da API**:
   - Obtenha uma chave de API válida no [ExchangeRate-API](https://www.exchangerate-api.com).
   - Defina a chave como uma variável de ambiente chamada `EXCHANGE_RATE_API_KEY`.

   No Linux, adicione a variável:

   ```bash
   export EXCHANGE_RATE_API_KEY=SuaChaveDaAPI
   ```

## Compilação e Execução

1. Compile com o GCC e inclua a biblioteca `libcurl`:

   ```bash
   gcc -Wall dolar.c -lcurl -o dolar
   ```

2. Execute o programa:

   ```bash
   ./dolar
   ```


#