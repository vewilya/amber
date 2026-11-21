#include <gratlin/gratlin.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void init() { printf("Init"); }

Vector *vec_create(float *data, size_t size) {
    Vector *vec = malloc(sizeof(Vector));
    vec->data = data;
    vec->size = size;

    return vec;
}

Matrix *mat_create(size_t rows, size_t cols) {
    Matrix *mat = malloc(sizeof(Matrix));
    if (mat == NULL) {
        return NULL;
    }

    mat->data = calloc(rows * cols, sizeof(float)); // zeroed
    if (mat->data == NULL) {
        free(mat);
        return NULL;
    }

    mat->rows = rows;
    mat->cols = cols;
    return mat;
}

Matrix *mat_create_from(const float *data, size_t rows, size_t cols) {
    Matrix *mat = mat_create(rows, cols);
    if (mat != NULL) {
        memcpy(mat->data, data, rows * cols * sizeof(float)); // <string.h>
    }
    return mat;
}

void mat_free(Matrix *mat) {
    if (mat != NULL) {
        free(mat->data);
        free(mat);
    }
}

float scalar_product(Vector *a, Vector *b) {
    if (a->size != b->size) {
        return 0;
    }

    float result = 0;

    for (size_t i = 0; i < a->size; i++) {
        result += a->data[i] * b->data[i];
    }

    return result;
}

float norm(Vector *vec) {
    float norm = 0.0f;

    for (size_t i = 0; i < vec->size; i++) {
        norm += vec->data[i] * vec->data[i];
    }

    return sqrtf(norm);
}

// out = a * b. out must already be a->rows x b->cols. Does not allocate.
bool matmul(const Matrix *a, const Matrix *b, Matrix *out) {
    if (a->cols != b->rows || out->rows != a->rows || out->cols != b->cols) {
        return false;
    }

    // Init data vector
    for (size_t i = 0; i < out->rows * out->cols; i++) {
        out->data[i] = 0.0f;
    }

    // row -> shared dimension -> column: the innermost loop walks b and out row by row
    for (size_t r = 0; r < a->rows; r++) {
        for (size_t k = 0; k < a->cols; k++) {
            const float a_rk = a->data[(r * a->cols) + k];
#pragma clang loop vectorize(enable)
            for (size_t c = 0; c < b->cols; c++) {
                out->data[(r * out->cols) + c] += a_rk * b->data[(k * b->cols) + c];
            }
        }
    }
    return true;
}
