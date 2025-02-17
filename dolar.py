import os
import requests
from datetime import datetime, timedelta

# Cache para armazenar a taxa de câmbio e o horário da última atualização
cache_taxa_cambio = {"taxa": None, "ultima_atualizacao": None}

# Função para obter a taxa de câmbio USD para BRL
def obter_taxa_cambio(chave_api):
    global cache_taxa_cambio

    # Verifica se a taxa de câmbio está em cache e se foi atualizada há menos de 10 minutos
    if cache_taxa_cambio["taxa"] and (datetime.now() - cache_taxa_cambio["ultima_atualizacao"]) < timedelta(minutes=10):
        return cache_taxa_cambio["taxa"]

    url = f"https://v6.exchangerate-api.com/v6/{chave_api}/latest/USD"
    
    try:
        resposta = requests.get(url)
        resposta.raise_for_status()  
        dados = resposta.json()  
        
        # Extrai a taxa de câmbio BRL
        if "conversion_rates" not in dados:
            print("Erro: A chave 'conversion_rates' não foi encontrada na resposta da API.")
            return -1.0
        if "BRL" not in dados["conversion_rates"]:
            print("Erro: A chave 'BRL' não foi encontrada na resposta da API.")
            return -1.0
        
        taxa = dados["conversion_rates"]["BRL"]
        cache_taxa_cambio["taxa"] = round(taxa, 4)  # Arredonda para 4 casas decimais
        cache_taxa_cambio["ultima_atualizacao"] = datetime.now()
        return cache_taxa_cambio["taxa"]
    except requests.exceptions.RequestException as e:
        print(f"Erro na requisição HTTP: {e}")
        return -1.0

# Função principal
def main():
    # Obtém a chave da API da variável de ambiente ou solicita ao usuário
    chave_api = os.getenv("EXCHANGE_TAXA_API_KEY")
    if not chave_api:
        chave_api = input("Digite a chave da API: ")

    # Obtém a taxa de câmbio
    taxa = obter_taxa_cambio(chave_api)
    if taxa < 0:
        print("Erro ao obter a taxa de câmbio.")
        return

    print(f"Taxa de câmbio USD-BRL: {taxa}")

    # Loop para permitir várias conversões
    while True:
        try:
            reais = float(input("Digite o valor em Reais (BRL) ou '0' para sair: "))
            if reais == 0:
                print("Saindo...")
                break
            if reais < 0:
                print("Erro: O valor não pode ser negativo.")
                continue

            dolares = reais / taxa
            print(f"Valor em Dólares (USD): {dolares:.2f}")
        except ValueError:
            print("Erro: Valor inválido. Por favor, insira um número.")


if __name__ == "__main__":
    main()
