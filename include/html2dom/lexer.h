#ifndef _HTML2DOM_LEXER_H
#define _HTML2DOM_LEXER_H

#include <stddef.h>

#define H2D_TOKEN_ARRAY_INITIAL_LEN 12

enum token_type {
    TOKEN_OPEN_TAG,
    TOKEN_CLOSE_TAG,
    TOKEN_TAG_NAME,
    TOKEN_TEXT,
    TOKEN_ATTR,
    TOKEN_ATTR_NAME,
    TOKEN_ATTR_VALUE,
    TOKEN_COMMENT_BEGIN,
    TOKEN_COMMENT_END,
    TOKEN_COMMENT_TEXT,
};

typedef struct token_s  {
    enum token_type type;
    char *value;
} token_t;

typedef struct token_array_s {
    token_t *arr;
    size_t len;
    size_t maxlen;
} token_array_t;

static inline token_t *h2d_lexer_array_get(token_array_t *arr, size_t index)
{
    return &arr->arr[index];
}

token_array_t *h2d_lexer_tokenize(const char *html, size_t len);
void h2d_lexer_tokens_free(token_array_t *);

#endif // _HTML2DOM_LEXER_H
