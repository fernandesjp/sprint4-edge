from flask import Flask, request, jsonify
from flask_cors import CORS

app = Flask(__name__)
CORS(app)

velocidade_dados = {}

@app.route('/api/velocidade', methods=['POST'])
def receber_dados():
    global velocidade_dados
    dados = request.get_json()
    if not dados:
        return jsonify({"erro": "Nenhum dado recebido"}), 400

    velocidade_dados = dados
    print(f"📥 Dados recebidos: {dados}")
    return jsonify({"mensagem": "Dados recebidos com sucesso!"}), 200

@app.route('/api/velocidade', methods=['GET'])
def enviar_dados():
    if not velocidade_dados:
        return jsonify({"mensagem": "Sem dados ainda"})
    return jsonify(velocidade_dados)

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000)