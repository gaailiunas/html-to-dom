#include <html2dom/dom.h>
#include <assert.h>

int main(void)
{
    const char sample[] = "<html lang=\"en\"><head><meta charset=\"UTF-8\"><title>test 123<title></head><body><button disabled>Click me</button><input type=\"checkbox\" required checked>/body></html>";
    DOM *dom = h2d_dom_parse(sample, sizeof(sample));
    assert(dom != NULL);
    h2d_dom_free(dom);
    return 0;
}
