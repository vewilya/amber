#include "gratlin/gratlin.h"


int main() {

    Vector *v1 = vec_create((float[]) { 2.0f, 1.4f, 1.1f }, 3);
    Vector *v2 = vec_create((float[]) { 1.2f, 0.24f, 3.1f }, 3);

    float scalar = scalar_product(v1, v2);

    return 0;
}
