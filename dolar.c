#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <math.h>

#define API_KEY "fb3853d161ff08b6c851ee78"

// Estrutura para armazenar a resposta da API
struct MemoryStruct {
    char *memory;
    size_t size;
};

// Função de callback para lidar com a resposta HTTP
static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t realsize = size * nmemb;
    struct MemoryStruct *mem = (struct MemoryStruct *)userp;

    char *ptr = realloc(mem->memory, mem->size + realsize + 1);
    if (ptr == NULL) {
        // Sem memória!
        printf("Not enough memory (realloc returned NULL)\n");
        return 0;
    }

    mem->memory = ptr;
    memcpy(&(mem->memory[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;

    return realsize;
}

// Função para obter a taxa de conversão de USD para BRL da API
float getExchangeRate() {
    CURL *curl_handle;
    CURLcode res;
    struct MemoryStruct chunk;

    chunk.memory = malloc(1);  // Será expandido conforme necessário pelo realloc
    chunk.size = 0;  // Inicialmente não há dados

    curl_global_init(CURL_GLOBAL_ALL);

    curl_handle = curl_easy_init();

    char url[100];
    sprintf(url, "https://v6.exchangerate-api.com/v6/%s/latest/USD", API_KEY);

    curl_easy_setopt(curl_handle, CURLOPT_URL, url);
    curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
    curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)&chunk);
    curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "libcurl-agent/1.0");

    res = curl_easy_perform(curl_handle);

    if(res != CURLE_OK) {
        fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        curl_easy_cleanup(curl_handle);
        free(chunk.memory);
        curl_global_cleanup();
        return -1.0;  // Retorna um valor inválido em caso de erro
    }

    // Analisar a resposta JSON para obter a taxa de câmbio BRL/USD
    float exchangeRate = -1.0;  // Valor padrão para indicar falha na obtenção da taxa
    const char *delimiter = "\"BRL\":";  // Delimitador para encontrar o valor BRL

    char *start = strstr(chunk.memory, delimiter);
    if (start != NULL) {
        start += strlen(delimiter);
        exchangeRate = strtof(start, NULL);  // Converte a string para float
    }

    curl_easy_cleanup(curl_handle);
    free(chunk.memory);
    curl_global_cleanup();

    return exchangeRate;
}

// Função principal para testar a conversão
int main() {
    float rate = getExchangeRate();
    if (rate < 0) {
        printf("Erro ao obter a taxa de câmbio.\n");
        return 1;
    }

    float reais;
    printf("Digite o valor em Reais (BRL): ");
    scanf("%f", &reais);

    float dolares = reais / rate;
    printf("Valor em Dólares (USD): %.2f\n", dolares);

    return 0;
}
