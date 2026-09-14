/**
 * NEXT 2026 - Firmware simples de coleta de imagens
 *
 * A pagina captura exatamente o frame exibido no streaming. As imagens ficam
 * no navegador e o download gera um .zip sem salvar nada no ESP32-CAM.
 */

#include "esp_camera.h"
#include "soc/rtc_cntl_reg.h"
#include "web_page.h"
#include <WebServer.h>
#include <WiFi.h>
#include "config.h"

// =========================
// CONFIGURACOES DO ALUNO
// =========================
namespace Aluno {
  const char WIFI_SSID[] = "UFO-PC";      // altere para o nome da sua rede WiFi
  const char WIFI_PASS[] = "987654321";  // altere para a senha da sua rede WiFi
  const char HOSTNAME[]  = "NOMEESP32CAM";   // nome do dispositivo na rede

  // Veja ao final do arquivo os tamanhos suportados para a captura das imagens
  const framesize_t FRAME_SIZE = FRAMESIZE_VGA;

  const int FLASH_LED_PIN = 4;         // LED branco da ESP32-CAM AI Thinker

  // **Atenção!** Não use brilho acima de 40, pois pode queimar o FLASH e o ESP32CAM!!!
  const int FLASH_LED_BRIGHTNESS = 20; // brilho baixo, escala 0 a 255
}

// =========================
// CONFIGURACOES WEBSERVER
// =========================
WebServer server(80);

void setupFlashLed() {
  pinMode(Aluno::FLASH_LED_PIN, OUTPUT);
  analogWrite(Aluno::FLASH_LED_PIN, 0);
}

void conectarWiFi() {
  WiFi.setHostname(Aluno::HOSTNAME);
  WiFi.begin(Aluno::WIFI_SSID, Aluno::WIFI_PASS);

  Serial.print("Conectando ao WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
}

void handleIndex() {
  server.send_P(200, "text/html", INDEX_HTML);
}

void handleStream() {
  WiFiClient client = server.client();
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: multipart/x-mixed-replace; boundary=frame");
  client.println("Cache-Control: no-cache");
  client.println("Connection: close");
  client.println();

  while (client.connected()) {
    auto fb = esp_camera_fb_get();
    if (!fb) {
      Serial.println("Falha ao capturar frame");
      break;
    }

    client.println("--frame");
    client.println("Content-Type: image/jpeg");
    client.printf("Content-Length: %u\r\n\r\n", fb->len);
    client.write(fb->buf, fb->len);
    client.println();
    esp_camera_fb_return(fb);

    if (!client.connected()) break;
    delay(50);
  }
}

void setup() {
  // Evita reset por oscilação de alimentação no ESP32-CAM.
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);

  delay(1000);
  Serial.begin(115200);
  Serial.println();
  Serial.println("___SERVIDOR DE COLETA DE IMAGENS___");
  setupFlashLed();

  if (!iniciarCamera(Aluno::FRAME_SIZE)) {
    Serial.println("Camera não iniciou. Reinicie e confira alimentação/pinout.");
    return;
  }

  conectarWiFi();

  analogWrite(Aluno::FLASH_LED_PIN, Aluno::FLASH_LED_BRIGHTNESS);

  server.on("/", HTTP_GET, handleIndex);
  server.on("/stream", HTTP_GET, handleStream);
  server.begin();

  Serial.println();
  Serial.println("Câmera OK");
  Serial.println("WiFi OK");
  Serial.println("Servidor OK");
  Serial.print("Acesse: http://");
  Serial.println(WiFi.localIP());
}

void loop() {
  server.handleClient();
}

// Opcoes comuns para Aluno::FRAME_SIZE:
// FRAMESIZE_QQVGA  -> 160x120
// FRAMESIZE_QVGA   -> 320x240
// FRAMESIZE_HVGA   -> 480x320
// FRAMESIZE_VGA    -> 640x480
// FRAMESIZE_SVGA   -> 800x600
// FRAMESIZE_XGA    -> 1024x768
// FRAMESIZE_HD     -> 1280x720
// FRAMESIZE_UXGA   -> 1600x1200
