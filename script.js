// Firebase SDK
import { initializeApp } from "https://www.gstatic.com/firebasejs/11.6.1/firebase-app.js";
import { getDatabase, ref, set, onValue } from "https://www.gstatic.com/firebasejs/11.6.1/firebase-database.js";

// Configuração do seu Firebase
const firebaseConfig = {
  apiKey: "AIzaSyD3kW55aQ6SaLyz54CcL5MsTUXuPu5FT_c",
  authDomain: "lampada-b9268.firebaseapp.com",
  projectId: "lampada-b9268",
  storageBucket: "lampada-b9268.firebasestorage.app",
  messagingSenderId: "176380806851",
  appId: "1:176380806851:web:a9236414b431af1dbdc476",
  databaseURL: "https://lampada-b9268-default-rtdb.firebaseio.com"
};

// Inicialização
const app = initializeApp(firebaseConfig);
const database = getDatabase(app);
const luzRef = ref(database, "luz/status");

// Funções para alterar o estado da luz
window.ligar = function () {
  atualizarEstadoVisual("ligado");     // Feedback imediato
  set(luzRef, "ligado");
};

window.desligar = function () {
  atualizarEstadoVisual("desligado");  // Feedback imediato
  set(luzRef, "desligado");
};

// Função para atualizar a interface visualmente
function atualizarEstadoVisual(estado) {
  const display = document.getElementById("estado");
  const lampada = document.getElementById("lampada");

  if (estado === "ligado") {
    display.textContent = "Ligado 🔆";
    display.style.color = "cyan";
    lampada.classList.add("ligada");
  } else if (estado === "desligado") {
    display.textContent = "Desligado 🌑";
    display.style.color = "red";
    lampada.classList.remove("ligada");
  } else {
    display.textContent = "Desconhecido";
    display.style.color = "white";
    lampada.classList.remove("ligada");
  }
}

// Mantém sincronização com o Firebase
onValue(luzRef, (snapshot) => {
  const estado = snapshot.val();
  atualizarEstadoVisual(estado);
});
