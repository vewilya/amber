#include <stdio.h>
#include <dsp/inline_test.h>

#ifndef ERROR_H
#define ERROR_H

typedef enum {
    // Success (0)
    NO_ERR = 0,
    // Generic error (1)
    ERR,
    PLUGIN_CHAIN_ERR,
    PARAM_ERR,
    NULL_PTR_ERR,
    VALUE_ERR,

} Err_t;

static inline const char *get_err_msg(Err_t err) {

    switch (err) {
        case NO_ERR: return "Success";
        case ERR: return "Generic Error";
        case PLUGIN_CHAIN_ERR: return "Plugin chain error";
        case PARAM_ERR: return "Paramter error";
        case NULL_PTR_ERR: return "Null pointer error";
        case VALUE_ERR: return "Value error";

    }
    return "Unknown error";
}

TEST(err, msg_success) {
    CHECK_STR_EQ(get_err_msg(NO_ERR), "Success");
}

TEST(err, msg_value) {
    CHECK_STR_EQ(get_err_msg(VALUE_ERR), "Value error");
}

TEST(err, msg_unknown) {
    CHECK_STR_EQ(get_err_msg((Err_t)42), "Unknown error");
}

#endif
