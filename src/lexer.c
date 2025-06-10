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
        arr->len = 0;
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

static int h2d_lexer__array_add(token_array_t *arr, enum token_type type, char *value, size_t valuelen)
{
    if (arr->len == arr->maxlen) {
        if (h2d_lexer__array_grow(arr) != 0)
            return 1;
    }
    token_t *t = &arr->arr[arr->len];
    t->type = type;

    if (value != NULL) {
        char *v = (char *)malloc(valuelen + 1);
        memcpy(v, value, valuelen);
        v[valuelen] = 0;
        t->value = v;
    }
    else {
        t->value = NULL;
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
    bool tag_name = false;
    size_t text_len = 0;
    bool is_kv_attr = false; // keyvalue

    for (size_t i = 0; i < len; i++) {
        if (html[i] == '<') {
            tag_open = true;
            printf("opening tag\n");

            if (text_len > 0 && h2d_lexer__array_add(arr, TOKEN_TEXT, (char *)html - text_len + i, text_len) != 0)
                goto error;
            text_len = 0;

            if (h2d_lexer__array_add(arr, TOKEN_OPEN_TAG, NULL, 0) != 0)
                goto error;

            continue;
        }
        else if (tag_open && html[i] == '>') {
            tag_open = false;

            printf("closing tag\n");
            
            if (!tag_name && text_len > 0 && h2d_lexer__array_add(arr, TOKEN_TAG_NAME, (char *)html - text_len + i, text_len) != 0) {
                goto error;
            }
            else if (is_kv_attr) {
                if (text_len > 0 && h2d_lexer__array_add(arr, TOKEN_ATTR_VALUE, (char *)html - text_len + i, text_len) != 0)
                    goto error;
                is_kv_attr = false;
            }

            tag_name = false;
            text_len = 0;

            if (h2d_lexer__array_add(arr, TOKEN_CLOSE_TAG, NULL, 0) != 0)
                goto error;

            continue;
        }

        // read text until whitespace (now just space)
        if (html[i] == ' ') {
            // check if its the tag name
            if (tag_open && !tag_name) {
                if (text_len > 0 && h2d_lexer__array_add(arr, TOKEN_TAG_NAME, (char *)html - text_len + i, text_len) != 0)
                    goto error;
                tag_name = true;
            }
            else if (tag_open && is_kv_attr) {
                if (text_len > 0 && h2d_lexer__array_add(arr, TOKEN_ATTR_VALUE, (char *)html - text_len + i, text_len) != 0)
                    goto error;
                is_kv_attr = false;
            }
            else {
                if (text_len > 0 && h2d_lexer__array_add(arr, TOKEN_TEXT, (char *)html - text_len + i, text_len) != 0)
                    goto error;
            }

            text_len = 0;
        }
        // attr key before =
        else if (html[i] == '=') {
            if (text_len > 0 && h2d_lexer__array_add(arr, TOKEN_ATTR_NAME, (char *)html - text_len + i, text_len) != 0)
                goto error;

            text_len = 0;
            is_kv_attr = true;
        }
        else {
            text_len++;
        }
    }

    return arr;

error:
    printf("error\n");
    h2d_lexer_tokens_free(arr);
    return NULL;
}

void h2d_lexer_tokens_free(token_array_t *arr)
{
    for (size_t i = 0; i < arr->len; i++) {
        if (arr->arr[i].value != NULL) {
            free(arr->arr[i].value);
        }
    }
    free(arr->arr);
    free(arr);
}
