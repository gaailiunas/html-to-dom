#include <html2dom/dom.h>
#include <html2dom/lexer.h>
#include <stdio.h>
#include <stdlib.h>

const char *strtypes[] = {
    [TOKEN_OPEN_TAG] = "OPEN_TAG",
    [TOKEN_CLOSE_TAG] = "CLOSE_TAG",
    [TOKEN_TAG_NAME] = "TAG_NAME",
    [TOKEN_TEXT] = "TEXT",
    [TOKEN_ATTR] = "ATTR",
    [TOKEN_ATTR_NAME] = "ATTR_NAME",
    [TOKEN_ATTR_VALUE] = "ATTR_VALUE",
    [TOKEN_COMMENT_BEGIN] = "COMMENT_BEGIN",
    [TOKEN_COMMENT_END] = "COMMENT_END",
    [TOKEN_COMMENT_TEXT] = "COMMENT_TEXT",
};

DOM *h2d_dom_parse(const char *html, size_t len)
{
    size_t tokens_len = 0;
    token_array_t *tokens = h2d_lexer_tokenize(html, len);
    if (tokens) {

        for (size_t i = 0; i < tokens->len; i++) {
            token_t *t = h2d_lexer_array_get(tokens, i);
            printf("type:%s ", strtypes[t->type]);
            if (t->value != NULL) {
                printf("text:%s \n", t->value);
            }
            else {
                printf("\n");
            }

        }

        // construct a DOM tree

        h2d_lexer_tokens_free(tokens);
    }
    return NULL;
}

void h2d_dom_free(DOM *dom)
{

}
