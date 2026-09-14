#!/usr/bin/env python3
"""Organiza e renomeia as imagens capturadas pela ESP32-CAM.

Recebe uma pasta com as imagens brutas de uma classe (por exemplo, o ZIP
baixado da interface web do firmware, já extraído) e copia as imagens para
a pasta de evidências com nomes padronizados: classe_001.jpg, classe_002.jpg...

Uso:
    python3 tools/organizar_imagens.py --entrada ./bruto/garrafa --classe garrafa
    python3 tools/organizar_imagens.py --entrada ./bruto/controle --classe "controle remoto"
"""

import argparse
import re
import shutil
import unicodedata
from pathlib import Path

IMAGE_EXTENSIONS = {".jpg", ".jpeg", ".png"}
REPO_ROOT = Path(__file__).resolve().parent.parent
DEFAULT_SAIDA_BASE = REPO_ROOT / "CP1" / "evidencias" / "imagens-coleta"


def sanitizar_nome_classe(nome: str) -> str:
    nome = unicodedata.normalize("NFKD", nome).encode("ascii", "ignore").decode("ascii")
    nome = nome.strip().lower()
    nome = re.sub(r"[^a-z0-9]+", "_", nome)
    nome = re.sub(r"_+", "_", nome).strip("_")
    if not nome:
        raise ValueError("Nome de classe inválido após sanitização.")
    return nome


def organizar(entrada: Path, classe: str, saida: Path, mover: bool) -> int:
    if not entrada.is_dir():
        raise SystemExit(f"Pasta de entrada não encontrada: {entrada}")

    arquivos = sorted(
        p for p in entrada.iterdir() if p.is_file() and p.suffix.lower() in IMAGE_EXTENSIONS
    )
    if not arquivos:
        raise SystemExit(f"Nenhuma imagem (.jpg/.jpeg/.png) encontrada em {entrada}")

    saida.mkdir(parents=True, exist_ok=True)

    existentes = list(saida.glob(f"{classe}_*"))
    inicio = len(existentes) + 1

    total = 0
    for i, origem in enumerate(arquivos, start=inicio):
        destino = saida / f"{classe}_{i:03d}{origem.suffix.lower()}"
        if mover:
            shutil.move(str(origem), destino)
        else:
            shutil.copy2(origem, destino)
        total += 1

    return total


def main():
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("--entrada", required=True, type=Path, help="Pasta com as imagens brutas de UMA classe")
    parser.add_argument("--classe", required=True, help="Nome da classe (ex: garrafa, controle)")
    parser.add_argument(
        "--saida",
        type=Path,
        default=None,
        help=f"Pasta de destino (padrão: {DEFAULT_SAIDA_BASE}/<classe>)",
    )
    parser.add_argument("--mover", action="store_true", help="Mover em vez de copiar as imagens")
    args = parser.parse_args()

    classe = sanitizar_nome_classe(args.classe)
    saida = args.saida or (DEFAULT_SAIDA_BASE / classe)

    total = organizar(args.entrada, classe, saida, args.mover)

    print(f"Classe: {classe}")
    print(f"Imagens organizadas: {total}")
    print(f"Destino: {saida}")
    if total < 100:
        print(f"Atenção: o Checkpoint 1 exige no mínimo 100 imagens válidas por classe (atual: {total}).")


if __name__ == "__main__":
    main()
