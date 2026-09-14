# Projeto: Visão Computacional com ESP32-CAM e YOLO

Projeto desenvolvido de forma incremental em três Checkpoints, utilizando
uma ESP32-CAM para aquisição de imagens e um modelo YOLO para detecção de
objetos. Ao final dos três checkpoints, a solução deve ser capaz de:

- Capturar imagens utilizando uma ESP32-CAM
- Construir um dataset próprio
- Realizar a anotação dos objetos
- Treinar um modelo de detecção utilizando YOLO
- Avaliar o desempenho do modelo
- Disponibilizar imagens pela ESP32-CAM através de HTTP
- Executar inferência local utilizando Python
- Identificar objetos em imagens capturadas pela câmera em tempo real

## Checkpoint 1 — Construção do Dataset

Etapas: firmware de coleta de imagens → organização do dataset → labeling
no Roboflow. Ver [`CP1/README.md`](CP1/README.md) para o template de
entrega e [`01-firmware-coleta-imagens/README.md`](01-firmware-coleta-imagens/README.md)
para o passo a passo de gravação do firmware.

### 1. Gravar o firmware na ESP32-CAM

Isso é feito **localmente**, com a ESP32-CAM conectada por USB/FTDI ao seu
computador (esta sessão não tem acesso à porta serial da sua máquina):

```bash
cd 01-firmware-coleta-imagens
pio run --target upload   # grave com GPIO0 no GND, depois solte e resete
pio device monitor        # acompanhe em 115200 baud
```

O Wi-Fi já está configurado no firmware para a rede `Suellen WIfi`
(namespace `Aluno` em `src/01-coleta-ESP32CAM.ino`). **Confirme que essa
rede é 2.4 GHz** — o ESP32-CAM não conecta em redes 5 GHz.

### 2. Capturar as imagens

Acesse o endereço IP mostrado no monitor serial, capture no mínimo 100
imagens válidas por classe (mínimo 2 classes), variando ângulo, distância,
iluminação, posição e fundo, e baixe o ZIP pela interface web.

### 3. Organizar as imagens

Extraia o ZIP baixado e rode, para cada classe:

```bash
python3 tools/organizar_imagens.py --entrada ./bruto/garrafa --classe garrafa
python3 tools/organizar_imagens.py --entrada ./bruto/controle --classe controle
```

As imagens são copiadas, renomeadas e sanitizadas (`classe_001.jpg`,
`classe_002.jpg`, ...) para `CP1/evidencias/imagens-coleta/<classe>/`.

### 4. Anotar no Roboflow

Suba as imagens organizadas para um projeto no Roboflow, defina as classes
exatamente como combinado com o professor, faça as anotações (bounding
boxes) e exporte no formato **YOLO**. Coloque o conteúdo exportado
(`data.yaml`, `train/`, `valid/`) em `CP1/dataset/`.

### 5. Preencher a documentação e montar o ZIP final

Preencha `CP1/README.md` (integrantes/RMs, classes, quantidade de imagens
por classe, link do Roboflow, descrição do processo) e as evidências em
`CP1/evidencias/`. Depois monte o ZIP de entrega:

```bash
python3 tools/montar_entrega_cp1.py --grupo "NOME_DO_GRUPO"
```

O script junta `firmware/` (a partir de `01-firmware-coleta-imagens/`),
`evidencias/`, `dataset/` e `README.md` em `CP1_NOME_DO_GRUPO.zip`, e
avisa sobre pendências (menos de 100 imagens por classe, `data.yaml`
ausente, campos do README não preenchidos, etc.).

## Estrutura do repositório

```
01-firmware-coleta-imagens/   # projeto PlatformIO do firmware da ESP32-CAM
CP1/                          # entrega do Checkpoint 1 (evidências, dataset, README)
tools/                        # scripts de organização de imagens e montagem do ZIP
```
