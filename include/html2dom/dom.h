#ifndef _HTML2DOM_DOM_H
#define _HTML2DOM_DOM_H

#include <stddef.h>

// https://en.wikipedia.org/wiki/Left-child_right-sibling_binary_tree
/* DOM:
 *
 *                1
                 /|\
                / | \
               /  |  \
              2   3   4
             / \      |
            5   6     7
                     / \
                    8   9
 *
 */

typedef struct node_attr_s {
    char key[32];
    char *value; // dynamically-allocated
    struct node_attr_s *next;
} node_attr_t;

typedef struct node_s {
    char tag[32];
    node_attr_t *attrs;
    struct node_s *first_child;
    struct node_s *next_sibling;
} node_t;

typedef struct {
    node_t *root;
} DOM;

DOM *h2d_dom_parse(const char *html, size_t len);
void h2d_dom_free(DOM *);

#endif // _HTML2DOM_DOM_H
