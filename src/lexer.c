#include <html2dom/lexer.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>

static token_array_t *h2d_lexer__array_new()
{
    token_array_t *arr = (token_array_t *)malloc(sizeof(*arr));
    if (arr) {
        arr->arr = (token_t *)malloc(sizeof(*arr->arr) * H2D_TOKEN_ARRAY_INITIAL_LEN);
        if (arr->arr == NULL) {
            free(arr);
            return NULL;
        }
        arr->maxlen = H2D_TOKEN_ARRAY_INITIAL_LEN;
    }
    return arr;
}

static int h2d_lexer__array_grow(token_array_t *arr)
{
    size_t tmplen = arr->maxlen * 2;
    token_t *tmp = (token_t *)realloc(arr->arr, sizeof(*tmp) * tmplen);
    if (tmp == NULL)
        return 1;
    arr->arr = tmp;
    arr->maxlen = tmplen;
    return 0;
}

// valuelen must be >= 31
static int h2d_lexer__array_add(token_array_t *arr, enum token_type type, char *value, size_t valuelen)
{
    if (arr->len == arr->maxlen) {
        if (h2d_lexer__array_grow(arr) != 0)
            return 1;
    }
    token_t *t = &arr->arr[arr->len];
    t->type = type;
    if (value) {
        memcpy(t->value, value, valuelen);
        t->value[valuelen] = 0;
    }
    arr->len++;
    return 0;
}

token_array_t *h2d_lexer_tokenize(const char *html, size_t len)
{
    token_array_t *arr = h2d_lexer__array_new();
    if (arr == NULL)
        return NULL;

    // states
    bool tag_open = false;
    char text[1024];
    size_t text_len = 0;

    for (size_t i = 0; i < len; i++) {
        if (html[i] == '<') {
            tag_open = true;
            printf("opening tag\n");

            if (h2d_lexer__array_add(arr, TOKEN_OPEN_TAG, NULL, 0) != 0)
                goto error;

            continue;
        }
        else if (tag_open && html[i] == '>') {
            tag_open = false;
            printf("closing tag\n");
            
            if (text_len > 0 && h2d_lexer__array_add(arr, TOKEN_TEXT, text, text_len) != 0)
                goto error;

            text_len = 0;

            if (h2d_lexer__array_add(arr, TOKEN_CLOSE_TAG, NULL, 0) != 0)
                goto error;

            continue;
        }

        // read text until whitespace (now just space)
        if (html[i] == ' ') {
            if (text_len > 0 && h2d_lexer__array_add(arr, TOKEN_TEXT, text, text_len) != 0)
                goto error;

            text_len = 0;
        }
        else {
            text[text_len++] = html[i];
        }
    }

    return arr;

error:
    h2d_lexer_tokens_free(arr);
    return NULL;
}

void h2d_lexer_tokens_free(token_array_t *arr)
{
    if (arr->arr)
        free(arr->arr);
    free(arr);
}
