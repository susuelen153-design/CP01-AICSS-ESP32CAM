#!/usr/bin/env python3
"""Monta o ZIP de entrega do Checkpoint 1 (CP1_NOME_GRUPO.zip).

Junta o firmware (01-firmware-coleta-imagens/src + platformio.ini), as
evidências e o dataset preenchidos em CP1/, e o README.md do checkpoint,
gerando o arquivo final no formato exigido pelo enunciado:

CP1/
├── firmware/
├── evidencias/
├── dataset/
└── README.md

Uso:
    python3 tools/montar_entrega_cp1.py --grupo "NOME_DO_GRUPO"
"""

import argparse
import shutil
import sys
from pathlib import Path

REPO_ROOT = Path(__file__).resolve().parent.parent
FIRMWARE_SRC = REPO_ROOT / "01-firmware-coleta-imagens"
CP1_DIR = REPO_ROOT / "CP1"

FIRMWARE_IGNORE = shutil.ignore_patterns(".pio", ".vscode", "*.o", "*.d")


def contar_imagens(pasta: Path) -> dict:
    exts = {".jpg", ".jpeg", ".png"}
    contagem = {}
    if not pasta.is_dir():
        return contagem
    for classe_dir in sorted(pasta.iterdir()):
        if classe_dir.is_dir():
            n = sum(1 for p in classe_dir.iterdir() if p.suffix.lower() in exts)
            contagem[classe_dir.name] = n
    return contagem


def validar(saida_dir: Path) -> list:
    avisos = []

    imagens = contar_imagens(CP1_DIR / "evidencias" / "imagens-coleta")
    if len(imagens) < 2:
        avisos.append("Menos de 2 classes encontradas em CP1/evidencias/imagens-coleta/.")
    for classe, n in imagens.items():
        if n < 100:
            avisos.append(f"Classe '{classe}' tem {n} imagens (mínimo exigido: 100).")

    dataset_dir = saida_dir / "dataset"
    if not (dataset_dir / "data.yaml").is_file():
        avisos.append("dataset/data.yaml não encontrado (exporte o dataset do Roboflow para CP1/dataset/).")
    for pasta in ["train/images", "train/labels", "valid/images", "valid/labels"]:
        if not (dataset_dir / pasta).is_dir():
            avisos.append(f"dataset/{pasta} não encontrado.")

    if "<!--" in (saida_dir / "README.md").read_text(encoding="utf-8"):
        avisos.append("README.md ainda contém campos de preenchimento (<!-- ... -->) não respondidos.")

    return avisos


def montar(grupo: str, destino: Path) -> Path:
    nome_zip_base = f"CP1_{grupo}"
    with_dir = destino / nome_zip_base
    if with_dir.exists():
        shutil.rmtree(with_dir)
    with_dir.mkdir(parents=True)

    shutil.copytree(FIRMWARE_SRC, with_dir / "firmware", ignore=FIRMWARE_IGNORE)
    shutil.copytree(CP1_DIR / "evidencias", with_dir / "evidencias")
    shutil.copytree(CP1_DIR / "dataset", with_dir / "dataset")
    shutil.copy2(CP1_DIR / "README.md", with_dir / "README.md")

    avisos = validar(with_dir)

    zip_path_base = destino / nome_zip_base
    zip_path = shutil.make_archive(str(zip_path_base), "zip", root_dir=destino, base_dir=nome_zip_base)
    shutil.rmtree(with_dir)

    return Path(zip_path), avisos


def main():
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("--grupo", required=True, help="Nome do grupo (usado no nome do arquivo, ex: EQUIPE_A)")
    parser.add_argument("--destino", type=Path, default=REPO_ROOT, help="Pasta onde o ZIP será salvo")
    args = parser.parse_args()

    grupo = args.grupo.strip().replace(" ", "_")
    zip_path, avisos = montar(grupo, args.destino)

    print(f"ZIP gerado: {zip_path}")
    if avisos:
        print("\nPendências antes da entrega final:")
        for a in avisos:
            print(f"  - {a}")
        sys.exit(1)
    else:
        print("Nenhuma pendência encontrada nas verificações automáticas.")


if __name__ == "__main__":
    main()
