#include <html2dom/dom.h>
#include <html2dom/lexer.h>
#include <stdio.h>
#include <stdlib.h>

DOM *h2d_dom_parse(const char *html, size_t len)
{
    size_t tokens_len = 0;
    token_array_t *tokens = h2d_lexer_tokenize(html, len);
    if (tokens) {

        for (size_t i = 0; i < tokens->len; i++) {
            token_t *t = h2d_lexer_array_get(tokens, i);
            printf("type:%u name:%s\n", t->type, t->value);
        }

        // construct a DOM tree

        h2d_lexer_tokens_free(tokens);
    }
    return NULL;
}

void h2d_dom_free(DOM *dom)
{

}
