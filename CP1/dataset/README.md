# Dataset (exportado do Roboflow)

Cole aqui o dataset exportado pelo Roboflow no formato **YOLO**. A estrutura
esperada é:

```
dataset/
├── data.yaml
├── train/
│   ├── images/
│   └── labels/
└── valid/
    ├── images/
    └── labels/
```

`data.yaml` deve listar corretamente as classes utilizadas no projeto, por
exemplo:

```yaml
train: train/images
val: valid/images

nc: 2
names: ['classe_1', 'classe_2']
```

Não altere a nomenclatura ou a ordem das classes em relação ao que foi
combinado com o professor.
