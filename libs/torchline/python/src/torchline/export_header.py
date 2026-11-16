from torch import nn

ACTIVATIONS = {
    nn.Tanh: "ACTIVATION_TANH",
    nn.ReLU: "ACTIVATION_RELU",
}


def c_array(values):
    return "{" + ", ".join(f"{value!r}f" for value in values) + "}"


def collect_layers(model):
    layers = []
    for name, module in model.named_children():
        if isinstance(module, nn.Linear):
            layers.append(
                {
                    "name": name,
                    "input_size": module.in_features,
                    "output_size": module.out_features,
                    "weights": module.weight.detach().flatten().tolist(),
                    "biases": module.bias.detach().flatten().tolist(),
                    "activation": "ACTIVATION_NONE",
                }
            )
        elif type(module) in ACTIVATIONS:
            layers[-1]["activation"] = ACTIVATIONS[type(module)]
        else:
            raise ValueError(f"Unsupported layer '{name}': {type(module).__name__}")
    return layers


def export_header(model, path):
    layers = collect_layers(model)
    max_layer_size = max(
        max(layer["input_size"], layer["output_size"]) for layer in layers
    )

    with open(path, "w") as file:
        file.write("#ifndef MODEL_WEIGHTS_H\n")
        file.write("#define MODEL_WEIGHTS_H\n")
        file.write("\n")
        file.write('#include "engine.h"\n\n')
        file.write(f"#define MODEL_INPUT_SIZE {layers[0]['input_size']}\n")
        file.write(f"#define MODEL_OUTPUT_SIZE {layers[-1]['output_size']}\n")
        file.write(f"#define MAX_LAYER_SIZE {max_layer_size}\n")
        file.write(f"#define LAYER_COUNT {len(layers)}\n\n")

        for layer in layers:
            file.write(
                f"static const float {layer['name']}_weights[] = {c_array(layer['weights'])};\n"
            )
            file.write(
                f"static const float {layer['name']}_biases[] = {c_array(layer['biases'])};\n"
            )

        file.write("\nstatic const Layer layers[LAYER_COUNT] = {\n")
        for layer in layers:
            file.write(
                f"    {{{layer['input_size']}, {layer['output_size']}, "
                f"{layer['name']}_weights, {layer['name']}_biases, {layer['activation']}}},\n"
            )
        file.write("};\n")
        file.write("\n")
        file.write("#endif // MODEL_WEIGHTS_H\n")
