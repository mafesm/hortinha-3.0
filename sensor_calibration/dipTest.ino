#define DIP1 7
#define DIP2 6
#define DIP3 5
#define DIP4 4

void setup() {
  Serial.begin(115200);
  pinMode(DIP1, INPUT_PULLUP);
  pinMode(DIP2, INPUT_PULLUP);
  pinMode(DIP3, INPUT_PULLUP);
  pinMode(DIP4, INPUT_PULLUP);

  Serial.println("DIP Teste inicializado");
  Serial.println("Use comandos: switch 1 status | switch 1 on | switch 1 off | switch all");
}

bool lerSwitch(int num) {
  int pin = -1;
  switch (num) {
    case 1: pin = DIP1; break;
    case 2: pin = DIP2; break;
    case 3: pin = DIP3; break;
    case 4: pin = DIP4; break;
    default: return false;
  }
  // Entrada com pullup: 0 = ligado, 1 = desligado
  return digitalRead(pin) == LOW;
}

void imprimeStatus(int num) {
  if (num < 1 || num > 4) {
    Serial.println("Número de switch inválido. Use 1 a 4.");
    return;
  }
  bool estado = lerSwitch(num);
  Serial.print("switch ");
  Serial.print(num);
  Serial.print(" está ");
  Serial.println(estado ? "ON" : "OFF");
}

void imprimeTodos() {
  for (int i = 1; i <= 4; i++) {
    imprimeStatus(i);
  }
}

void loop() {
  if (!Serial.available()) {
    return;
  }

  String line = Serial.readStringUntil('\n');
  line.trim();
  line.toLowerCase();

  if (line.length() == 0) {
    return;
  }

  if (line == "switch all") {
    imprimeTodos();
    return;
  }

  int idx = line.indexOf(' ');
  if (idx < 0) {
    Serial.println("Comando inválido. Exemplo: switch 1 status");
    return;
  }

  String cmd = line.substring(0, idx);
  String rest = line.substring(idx + 1);

  if (cmd != "switch") {
    Serial.println("Comando desconhecido. Use 'switch'.");
    return;
  }

  int idx2 = rest.indexOf(' ');
  if (idx2 < 0) {
    Serial.println("Formato inválido. Use 'switch 1 status' ou 'switch all'.");
    return;
  }

  String numStr = rest.substring(0, idx2);
  String action = rest.substring(idx2 + 1);
  int num = numStr.toInt();

  if (num < 1 || num > 4) {
    Serial.println("Número de switch inválido. Use 1 a 4.");
    return;
  }

  if (action == "status" || action == "on" || action == "off") {
    imprimeStatus(num);
    return;
  }

  Serial.println("Ação inválida. Use 'status', 'on' ou 'off'.");
}
