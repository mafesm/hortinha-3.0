#include "sensores.h"
#include "bomba_nft.h"
#include "uvc.h"
#include "modos.h"
#include "led.h"
#include "serial_proto.h"
#include "alertas_local.h"

void setup() {
  
  iniciarSerialArduino();
  iniciarSensores();
  iniciarModos();
  iniciarBombaNFT();
  iniciarUVCLocal();
  iniciarFotoperiodo();
  iniciarAlertasLocais();
  STAR(13, 12, 11);

  Serial.println("\n[SISTEMA] Pronto!\n");
}

void loop() {
  lerSensores();
  lerModos();
  controlarIrrigacao();
  atualizarServo();
  verificarAlertas();
  atualizarFotoperiodo();
  attUVC();
  GEN_RGB();

  if (millis() - ultimoEnvioUART >= 1000) { //envia dados a cada 1 segundo
    ultimoEnvioUART = millis();
    enviarDadosUART();
  }

  processarComandoUART();
  delay(50);
}
