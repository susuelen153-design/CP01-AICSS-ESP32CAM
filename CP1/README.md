# Checkpoint 1 — Construção do Dataset

<!-- Preencha todos os campos abaixo antes de gerar o ZIP de entrega. -->

## Integrantes e RMs

| Nome | RM |
| --- | --- |
| <!-- nome --> | <!-- RM --> |
| <!-- nome --> | <!-- RM --> |

## Classes utilizadas

<!-- Liste exatamente as classes oficiais definidas pelo professor, na ordem correta. -->

1. <!-- classe_1 -->
2. <!-- classe_2 -->

## Quantidade de imagens por classe

| Classe | Quantidade de imagens |
| --- | --- |
| <!-- classe_1 --> | <!-- ex: 120 --> |
| <!-- classe_2 --> | <!-- ex: 105 --> |

## Link do projeto Roboflow

<!-- Cole aqui o link do projeto no Roboflow, quando aplicável. -->

## Descrição do processo realizado

<!--
Breve descrição de como o dataset foi construído: firmware utilizado,
condições de captura (ângulos, distâncias, iluminação, fundos), como as
imagens foram organizadas/renomeadas e como foi feita a anotação no
Roboflow.
-->

---

## Estrutura desta entrega

```
CP1/
├── firmware/       # código utilizado na ESP32-CAM (gerado por tools/montar_entrega_cp1.py)
├── evidencias/
│   ├── imagens-coleta/         # amostra das imagens capturadas
│   ├── esp32cam-funcionando/   # print/foto do serial monitor e da página web
│   └── roboflow/               # print do projeto no Roboflow (dataset, classes, anotações)
├── dataset/        # dataset exportado do Roboflow (data.yaml, train/, valid/)
└── README.md       # este arquivo
```

Veja o README na raiz do repositório para o passo a passo completo e para os
scripts que ajudam a organizar as imagens e montar o ZIP final de entrega.
