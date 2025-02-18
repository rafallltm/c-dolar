from flask import Flask, render_template, request, jsonify
import os
import requests
from datetime import datetime, timedelta

app = Flask(__name__)

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

@app.route("/")
def index():
    return render_template("index.html")

@app.route("/get_rate", methods=["GET"])
def get_rate():
    chave_api = os.getenv("EXCHANGE_TAXA_API_KEY")
    if not chave_api:
        return jsonify({"error": "Chave da API não encontrada"}), 400

    taxa = obter_taxa_cambio(chave_api)
    if taxa < 0:
        return jsonify({"error": "Erro ao obter a taxa de câmbio"}), 500

    return jsonify({"taxa": taxa})

@app.route("/convert", methods=["POST"])
def convert():
    data = request.json
    reais = data.get("reais")
    taxa = data.get("taxa")

    if not reais or not taxa:
        return jsonify({"error": "Dados inválidos"}), 400

    try:
        reais = float(reais)
        if reais < 0:
            return jsonify({"error": "O valor não pode ser negativo"}), 400

        dolares = reais / float(taxa)
        return jsonify({"dolares": round(dolares, 2)})
    except ValueError:
        return jsonify({"error": "Valor inválido"}), 400

if __name__ == "__main__":
    app.run(debug=True)
