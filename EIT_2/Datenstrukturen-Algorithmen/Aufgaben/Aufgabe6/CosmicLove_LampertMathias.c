#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>

typedef struct {
    char name[31];
    double radius, mass, dist, density;
} Planet;

void compute_density(Planet *p) {
    double volume = (4.0 / 3.0) * M_PI * pow(p->radius, 3);
    p->density = p->mass / volume;
}

int compare_dist(const void *a, const void *b) {
    double d1 = ((Planet *)a)->dist;
    double d2 = ((Planet *)b)->dist;
    return (d1 > d2) - (d1 < d2);
}

double roche_limit(double r, double dA, double dP) {
    return r * cbrt(2 * dA / dP);
}

int main() {
    int N;
    scanf("%d", &N);
    Planet *planets = malloc(N * sizeof(Planet)), alice;
    bool found = false;

    for (int i = 0; i < N; i++) {
        char r_str[20], m_str[20], d_str[20];
        scanf("%s %s %s %s", planets[i].name, r_str, m_str, d_str);
        planets[i].radius = atof(r_str);
        planets[i].mass = atof(m_str);
        planets[i].dist = atof(d_str);
        compute_density(&planets[i]);

        if (!found && strcmp(planets[i].name, "Alice") == 0) {
            alice = planets[i];
            found = true;
        }
    }

    if (!found) {
        fprintf(stderr, "Alice not found\n");
        free(planets);
        return 1;
    }

    qsort(planets, N, sizeof(Planet), compare_dist);

    for (int i = 0; i < N; i++) {
        if (strcmp(planets[i].name, "Alice") == 0) continue;
        if (planets[i].dist > roche_limit(alice.radius, alice.density, planets[i].density)) {
            printf("%s\n", planets[i].name);
            free(planets);
            return 0;
        }
    }

    fprintf(stderr, "No viable planet found\n");
    free(planets);
    return 1;
}
