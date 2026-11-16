import json
from pathlib import Path

import torch
from safetensors.torch import save_file
from torch import nn

from torchline.export_header import export_header

MODELS_DIR = Path(__file__).resolve().parents[2] / "models"  # torchline/python/models


def export():
    print("--- Exporting ---")


def save_model(model, model_name):
    MODELS_DIR.mkdir(exist_ok=True)

    torch.save(model.state_dict(), "models/asym_sat_model.pt")
    save_file(model.state_dict(), MODELS_DIR / "asym_sat_model.safetensors")
    export_header(model, "../src/model_weights.h")

    # JSON export
    layers = []

    for module in model:
        if isinstance(module, nn.Linear):
            layers.append(
                {
                    "type": "linear",
                    "in": module.in_features,
                    "out": module.out_features,
                    "weight": module.weight.detach().flatten().tolist(),
                    "bias": module.bias.detach().flatten().tolist(),
                }
            )
        elif isinstance(module, nn.ReLU):
            layers.append({"type": "activation", "function": "relu"})
        elif isinstance(module, nn.Tanh):
            layers.append({"type": "activation", "function": "tanh"})
        else:
            raise ValueError(f"Unsupported layer: {module}")

    with open(MODELS_DIR / model_name, "w") as file:
        json.dump({"layers": layers}, file, indent=2)


def c_values(values):
    if isinstance(values, list):
        return "{" + ", ".join(c_values(value) for value in values) + "}"
    return f"{values!r}f"
