import math
from collections import OrderedDict
from pathlib import Path

import matplotlib.pyplot as plt
import torch
from torch import nn

from torchline.export import save_model
from torchline.lookup_table import generate

MODELS_DIR = Path(__file__).resolve().parents[2] / "models"  # torchline/python/models


# Asymmetric saturation: tanh for positive input, sine for negative
def soft_saturation(x: torch.Tensor, drive: float = 3.0) -> torch.Tensor:
    negative = torch.sin(torch.clamp(drive * x, min=-math.pi / 2)) / drive
    return torch.where(x >= 0, torch.tanh(drive * x) / drive, negative)


def train(
    model: nn.Module, inputs: torch.Tensor, targets: torch.Tensor, steps: int = 1000
) -> nn.Module:
    optimizer = torch.optim.Adam(model.parameters(), lr=0.01)
    loss_function = nn.MSELoss()

    for step in range(steps):
        loss = loss_function(model(inputs), targets)
        optimizer.zero_grad()
        loss.backward()
        optimizer.step()

        if step % 100 == 0:
            print(f"step {step:4d}  loss {loss.item():.5f}")

    print(f"final loss: {loss.item():.5f}")
    return model


def plot(model: nn.Module) -> None:
    with torch.no_grad():
        x = torch.linspace(-3, 3, 200).unsqueeze(1).repeat(1, 2)
        y = model(x)

    plt.plot(x[:, 0], soft_saturation(x[:, 0]), label="soft_saturation")
    plt.plot(x[:, 0], y[:, 0], label="model", linestyle="--")
    plt.legend()
    plt.show()


def run() -> None:
    inputs = torch.rand(200, 2) * 6 - 3
    targets = soft_saturation(inputs)

    model = nn.Sequential(
        OrderedDict(
            [
                ("hidden", nn.Linear(2, 16)),
                ("activation", nn.Tanh()),
                ("output", nn.Linear(16, 2)),
            ]
        )
    )

    train(model, inputs, targets)
    plot(model)
    save_model(model, "asym_sat_model.json")

    with torch.no_grad():
        predictions = model(torch.tensor([[0.5, 0.3]]))

    print(predictions)

    generate(model)
