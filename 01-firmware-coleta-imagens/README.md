# Etapa 1 — Firmware de Coleta de Imagens

Nesta etapa você grava um firmware no ESP32-CAM que abre um servidor web para capturar imagens dos objetos do desafio.

---

## O que você vai precisar

- ESP32-CAM AI Thinker
- Módulo FTDI ou gravador USB compatível
- VS Code com PlatformIO instalado
- Cabo USB (certifique-se que o cabo transmite dados, não só carrega)

---

## Passo a passo

### 1. Abra o projeto no PlatformIO

No VS Code, abra a pasta:

```
aluno/01-firmware-coleta-imagens/
```

O firmware fica em:

```
src/01-coleta-ESP32CAM.ino
```

### 2. Altere o WiFi

No início do arquivo, altere:

```cpp
const char WIFI_SSID[] = "NOME_DA_REDE";
const char WIFI_PASS[] = "SENHA_DA_REDE";
```

Use a rede WiFi **2.4 GHz** da sala. O ESP32-CAM não funciona em redes 5 GHz.

### 3. Compile o projeto

Use o botão **Build** do PlatformIO ou rode no terminal do PlatformIO:

```bash
pio run
```

### 4. Grave o código

Coloque o ESP32-CAM em modo de gravação (conecte GPIO0 ao GND antes de ligar).

Use o botão **Upload** do PlatformIO ou rode no terminal do PlatformIO:

```bash
pio run --target upload
```

Após gravar, desconecte GPIO0 do GND e pressione o botão de reset.

### 5. Abra o monitor serial

Use o botão **Monitor** do PlatformIO ou rode no terminal do PlatformIO:

```bash
pio device monitor
```

O projeto já está configurado com velocidade **115200 baud**.

Você vai ver uma mensagem como:

```
Câmera OK
WiFi OK
Servidor OK
Acesse: http://192.168.0.100
```

### 6. Acesse o servidor no navegador

Abra o endereço indicado no monitor serial do PlatformIO em qualquer navegador do mesmo WiFi.

### 7. Capture as imagens

- Posicione o objeto em frente à câmera.
- Clique no botão de captura.
- Capture pelo menos 50 imagens por objeto, variando ângulo, distância e iluminação.

### 8. Baixe o ZIP

Clique em **Download ZIP** na interface web.

Extraia o arquivo. Você terá todas as imagens prontas para a próxima etapa.

---

## O que fazer a seguir

Vá para `02-organizacao-dataset/` e organize as imagens.

---

## Se der erro

- Câmera não inicializa → verifique se o ESP32-CAM está encaixado corretamente no suporte
- WiFi não conecta → confirme que a rede é 2.4 GHz e que a senha está correta
- Cabo USB → use um cabo que transmita dados (não só de carga)
- Driver → instale o driver CH340 ou CP210x conforme o seu gravador
- Comando `pio` não encontrado → abra o terminal do PlatformIO no VS Code

Veja mais em `docs/troubleshooting.md`.
