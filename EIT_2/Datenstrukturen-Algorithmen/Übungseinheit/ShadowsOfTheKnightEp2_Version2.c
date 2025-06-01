#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    int w, h, n;
    scanf("%d %d", &w, &h);
    scanf("%d", &n);
    int x0, y0;
    scanf("%d %d", &x0, &y0);
    int x = x0, y = y0;
    char info[16];

    // Possible coordinates
    int *xs = malloc(w * sizeof(int));
    int *ys = malloc(h * sizeof(int));
    int xs_sz = w, ys_sz = h;
    for (int i = 0; i < w; ++i) xs[i] = i;
    for (int i = 0; i < h; ++i) ys[i] = i;

    while (n--) {
        if (scanf("%s", info) != 1) break;
        // Filter
        if (xs_sz > 1) {
            int new_sz = 0;
            for (int i = 0; i < xs_sz; ++i) {
                int xi = xs[i];
                int dx0 = x0 - xi;
                int dy0 = y0 - y;
                int d0 = dx0*dx0 + dy0*dy0;
                int dx1 = x - xi;
                int dy1 = y - y;
                int d1 = dx1*dx1 + dy1*dy1;
                if (strcmp(info, "UNKNOWN") == 0
                    || (strcmp(info, "SAME") == 0 && d1 == d0)
                    || (strcmp(info, "WARMER") == 0 && d1 < d0)
                    || (strcmp(info, "COLDER") == 0 && d1 > d0)) {
                    xs[new_sz++] = xi;
                }
            }
            xs_sz = new_sz;
        } else {
            int new_sz = 0;
            for (int i = 0; i < ys_sz; ++i) {
                int yi = ys[i];
                int dx0 = x0 - x;
                int dy0 = y0 - yi;
                int d0 = dx0*dx0 + dy0*dy0;
                int dx1 = x - x;
                int dy1 = y - yi;
                int d1 = dx1*dx1 + dy1*dy1;
                if (strcmp(info, "UNKNOWN") == 0
                    || (strcmp(info, "SAME") == 0 && d1 == d0)
                    || (strcmp(info, "WARMER") == 0 && d1 < d0)
                    || (strcmp(info, "COLDER") == 0 && d1 > d0)) {
                    ys[new_sz++] = yi;
                }
            }
            ys_sz = new_sz;
        }
        // Prepare next jump
        x0 = x; y0 = y;
        if (xs_sz > 1) {
            int lo = xs[0], hi = xs[xs_sz - 1];
            // aim to split
            int nextx = lo + hi - x0;
            if (nextx == x0) nextx++;
            if (nextx < lo) nextx = lo;
            if (nextx > hi) nextx = hi;
            x = nextx;
        } else {
            x = xs[0];
            int lo = ys[0], hi = ys[ys_sz - 1];
            int nexty = lo + hi - y0;
            if (nexty == y0) nexty++;
            if (nexty < lo) nexty = lo;
            if (nexty > hi) nexty = hi;
            y = nexty;
        }
        printf("%d %d\n", x, y);
        fflush(stdout);
    }

    free(xs);
    free(ys);
    return 0;
}