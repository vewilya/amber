# __init__.py
import argparse

import torch


def main() -> None:
    print("--- Welcome to Torchline! ---")
    parser = argparse.ArgumentParser(prog="torchline")
    sub = parser.add_subparsers(dest="command", required=True)
    sub.add_parser("train")
    sub.add_parser("export")
    args = parser.parse_args()

    if args.command == "train":
        from torchline.train import run

        run()

    elif args.command == "export":
        from torchline.export import export

        export()
