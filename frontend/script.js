const SERVER = "http://54.172.140.81:5000";

async function atualizarDados() {
  const res = await fetch(`${SERVER}/api/velocidade`);
  const data = await res.json();
  
  const divDados = document.getElementById("dados");
  divDados.innerHTML = "";

  if (!data.todas) {
    divDados.innerHTML = "<p>Aguardando dados das jogadoras...</p>";
    return;
  }

  for (const [nome, vel] of Object.entries(data.todas)) {
    const box = document.createElement("div");
    box.className = "jogadora";
    box.innerHTML = `<strong>${nome}</strong><br>Velocidade: ${vel.toFixed(2)} m/s`;
    divDados.appendChild(box);
  }

  document.getElementById("maisRapida").innerText =
    `${data.mais_rapida} (${data.velocidade.toFixed(2)} m/s)`;
}

setInterval(atualizarDados, 2000);
atualizarDados();