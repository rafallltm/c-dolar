

# Funcionalidades do Código 

### Cache da Taxa de Câmbio

- A taxa de câmbio é armazenada em cache e atualizada apenas se a última requisição foi feita há mais de **10 minutos**. Isso evita múltiplas requisições à API em um curto período.

- O cache é armazenado em uma variável global (`cache_taxa_cambio`), que contém:
  - `taxa`: A taxa de câmbio armazenada.
  - `ultima_atualizacao`: O horário da última atualização.

### Uso de `datetime`

- A biblioteca `datetime` é utilizada para controlar o tempo entre as atualizações da taxa de câmbio.

- O tempo é calculado usando a diferença entre o horário atual (`datetime.now()`) e o horário da última atualização (`ultima_atualizacao`).

- Se a diferença for menor que **10 minutos**, o programa usa a taxa de câmbio em cache.

---

### Chave da API:

   - Obtenha uma chave de API válida no [ExchangeRate-API](https://www.exchangerate-api.com).
  
   - Defina a chave como uma variável de ambiente chamada `EXCHANGE_TAXA_API_KEY`

   - Se a variável de ambiente não estiver configurada, o programa solicitará que insira a chave da sua API ao ser executado.

  