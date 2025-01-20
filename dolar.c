#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <math.h>

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
float getExchangeRate(const char *api_key) {
    CURL *curl_handle;
    CURLcode res;
    struct MemoryStruct chunk;

    chunk.memory = malloc(1);  
    chunk.size = 0; 

    curl_global_init(CURL_GLOBAL_ALL);

    curl_handle = curl_easy_init();

    char url[256];
    sprintf(url, "https://v6.exchangerate-api.com/v6/%s/latest/USD", api_key);

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
        return -1.0; 
    }

    // Analisar a resposta JSON para obter a taxa de câmbio BRL/USD
    float exchangeRate = -1.0; 
    const char *delimiter = "\"BRL\":";  

    char *start = strstr(chunk.memory, delimiter);
    if (start != NULL) {
        start += strlen(delimiter);
        exchangeRate = strtof(start, NULL);  
    }

    curl_easy_cleanup(curl_handle);
    free(chunk.memory);
    curl_global_cleanup();

    return exchangeRate;
}

// Função principal para testar a conversão
int main() {
    const char *api_key = getenv("EXCHANGE_RATE_API_KEY");
    if (api_key == NULL) {
        fprintf(stderr, "Erro: Variável de ambiente EXCHANGE_RATE_API_KEY não definida.\n");
        return EXIT_FAILURE;
    }

    float rate = getExchangeRate(api_key);
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
