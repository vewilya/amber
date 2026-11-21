#include "gratlin/gratlin.h"
#include <criterion/criterion.h>

Test(gratlin, vector) {
    float *data = (float[]) { 3.0f, 2.0f, 10.3f };
    Vector *vec = vec_create(data, 3);

    cr_assert_eq(vec->size, 3);
    cr_assert_float_eq(vec->data[0], 3.0, 1e-6);
    cr_assert_float_eq(vec->data[1], 2.0, 1e-6);
    cr_assert_float_eq(vec->data[2], 10.3, 1e-6);
}

Test(gratlin, matrix) {
    float *data = (float[]) { 1.f, 2.f, 3.f, 4.f, 5.f, 6.f, 7.f, 8.f, 9.f };

    Matrix *mat = mat_create_from(data, 3, 3);

    cr_assert_eq(mat->rows, 3);
    cr_assert_eq(mat->cols, 3);
    cr_assert_float_eq(mat->data[0], 1.0, 1e-6);
    cr_assert_float_eq(mat->data[1], 2.0, 1e-6);
    cr_assert_float_eq(mat->data[2], 3.0, 1e-6);
    cr_assert_float_eq(mat->data[3], 4.0, 1e-6);
    cr_assert_float_eq(mat->data[4], 5.0, 1e-6);
    cr_assert_float_eq(mat->data[5], 6.0, 1e-6);
    cr_assert_float_eq(mat->data[6], 7.0, 1e-6);
    cr_assert_float_eq(mat->data[7], 8.0, 1e-6);
    cr_assert_float_eq(mat->data[8], 9.0, 1e-6);
}

Test(gratlin, matmul) {
    float data_a[] = {1, 2, 3,
                      4, 5, 6};          // 2x3
    float data_b[] = {7, 8,
                      9, 10,
                      11, 12};           // 3x2
    Matrix *a = mat_create_from(data_a, 2, 3);
    Matrix *b = mat_create_from(data_b, 3, 2);
    Matrix *c = mat_create(2, 2);

    cr_assert(matmul(a, b, c));
    cr_assert_float_eq(c->data[0], 58.0f, 1e-6);    // 1·7 + 2·9 + 3·11
    cr_assert_float_eq(c->data[1], 64.0f, 1e-6);    // 1·8 + 2·10 + 3·12
    cr_assert_float_eq(c->data[2], 139.0f, 1e-6);   // 4·7 + 5·9 + 6·11
    cr_assert_float_eq(c->data[3], 154.0f, 1e-6);   // 4·8 + 5·10 + 6·12

    mat_free(a);
    mat_free(b);
    mat_free(c);
}

Test(gratlin, matmul_rejects_mismatched_sizes) {
    Matrix *a = mat_create(2, 3);
    Matrix *b = mat_create(2, 3);    // a->cols (3) != b->rows (2)
    Matrix *c = mat_create(2, 3);

    cr_assert_not(matmul(a, b, c));

    mat_free(a);
    mat_free(b);
    mat_free(c);
}


Test(gratlin, scalar_prod) {
    Vector *a = vec_create((float[]) { 1.0f, 2.0f, 3.0f }, 3);
    Vector *b = vec_create((float[]) { 4.0f, 5.0f, 6.0f }, 3);
    float result = scalar_product(a, b);

    cr_assert_float_eq(result, 32.0, 1e-6);
}

Test(gratlin, norm) {
    Vector *vec = vec_create((float[]) { 3.0f, 4.0f, 5.0f }, 3);
    float result = norm(vec);

    cr_assert_float_eq(result, 7.071068, 1e-6);
}
