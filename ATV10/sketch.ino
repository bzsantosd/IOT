const int pinoLED = 13; // O pino ao qual o LED está conectado
String mensagem = "ola mundo"; 

const int tempoPonto = 200; // Duração de um "ponto" em milissegundos
const int tempoTraco = tempoPonto * 3; // Duração de um "traço"
const int tempoEntreElementos = tempoPonto; // Pausa entre pontos e traços de uma letra
const int tempoEntreLetras = tempoPonto * 3; // Pausa entre letras
const int tempoEntrePalavras = tempoPonto * 7; // Pausa entre palavras

// Dicionário de Código Morse
String morseCode[] = {
  ".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....", "..", ".---", "-.-", ".-..", "--", "-.", "---", ".--.", "--.-", ".-.", "...", "-", "..-", "...-", ".--", "-..-", "-.--", "--..",
  "-----", ".----", "..---", "...--", "....-", ".....", "-....", "--...", "---..", "----.", " "
};

char alphabet[] = {
  'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
  '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', ' '
};

void setup() {
  pinMode(pinoLED, OUTPUT);
}

void loop() {
  transmitirMorse(mensagem);
  delay(5000); // Espera 5 segundos antes de repetir a mensagem
}

// Função principal para transmitir a mensagem em Morse
void transmitirMorse(String texto) {
  texto.toLowerCase(); // Converte a mensagem para minúsculas

  for (int i = 0; i < texto.length(); i++) {
    char caractere = texto.charAt(i);

    // Encontra o código Morse para o caractere
    String codigo = "";
    for (int j = 0; j < sizeof(alphabet); j++) {
      if (caractere == alphabet[j]) {
        codigo = morseCode[j];
        break;
      }
    }

    // Transmite o código Morse do caractere
    for (int k = 0; k < codigo.length(); k++) {
      char elemento = codigo.charAt(k);
      if (elemento == '.') {
        ponto();
      } else if (elemento == '-') {
        traco();
      }
      delay(tempoEntreElementos);
    }
    
    // Pausa entre as letras (ou entre palavras)
    if (caractere == ' ') {
      delay(tempoEntrePalavras - tempoEntreElementos);
    } else {
      delay(tempoEntreLetras - tempoEntreElementos);
    }
  }
}

// Funções para os elementos básicos do Código Morse
void ponto() {
  digitalWrite(pinoLED, HIGH); 
  delay(tempoPonto);
  digitalWrite(pinoLED, LOW); 
}

void traco() {
  digitalWrite(pinoLED, HIGH); 
  delay(tempoTraco);
  digitalWrite(pinoLED, LOW); 
}
