#ifndef GARTLIN_H
#define GARTLIN_H

#include <stddef.h>

typedef struct {
    size_t size;
    float *data;
} Vector;

typedef struct {
    size_t rows;
    size_t cols;
    float *data;
} Matrix;

// Vector *vec_create(size_t size);
Vector *vec_create(float *data, size_t size);
Matrix *mat_create(size_t rows, size_t cols);
Matrix *mat_create_from(const float *data, size_t rows, size_t cols);

float scalar_product(Vector *a, Vector *b);
float norm(Vector *vec);

bool matmul(const Matrix *a, const Matrix *b, Matrix *out);

void vec_free(Vector *vec);
void mat_free(Matrix *mat);

#endif // GARTLIN_H
