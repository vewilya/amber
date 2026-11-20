#!/bin/bash
set -euo pipefail

cd "$(dirname "$0")"
cmake --preset dev
cmake --build --preset dev
./build/dev/amber
