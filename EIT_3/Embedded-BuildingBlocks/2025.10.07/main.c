#include <stdio.h>
#include <stdlib.h>

// Basisstruktur Shape
typedef struct Shape {
    int _x, _y;
    void (*fp_draw)(struct Shape *p_this);
} Shape;

// -------------------------------------------------------------------------------------------------
// Kreisstruktur
typedef struct Circle {
    Shape base;     // Zusammensetzung: Shape als erstes Element
    double _r;
} Circle;

void circle_draw(struct Shape *p_this) {
    Circle *p_circle = (Circle *)p_this;
    printf("Drawing a Circle r=%.2lf at (%d, %d).\n",
           p_circle->_r, p_circle->base._x, p_circle->base._y);
}

Circle *circle_create(int x, int y, double r) {
    Circle *p_this = malloc(sizeof(Circle));
    if (!p_this) return NULL;
    p_this->base._x = x;
    p_this->base._y = y;
    p_this->_r = r;
    p_this->base.fp_draw = circle_draw;
    return p_this;
}

void circle_free(Circle *p_this) {
    free(p_this);
}

// -------------------------------------------------------------------------------------------------
// Quadratstruktur
typedef struct Square {
    Shape base;
    double _a;
} Square;

void square_draw(struct Shape *p_this) {
    Square *p_square = (Square *)p_this;
    printf("Drawing a Square a=%.2lf at (%d, %d).\n",
           p_square->_a, p_square->base._x, p_square->base._y);
}

Square *square_create(int x, int y, double a) {
    Square *p_this = malloc(sizeof(Square));
    if (!p_this) return NULL;
    p_this->base._x = x;
    p_this->base._y = y;
    p_this->_a = a;
    p_this->base.fp_draw = square_draw;
    return p_this;
}

void square_free(Square *p_this) {
    free(p_this);
}

// -------------------------------------------------------------------------------------------------
// Fabrikfunktion
typedef enum {
    SHAPE_CIRCLE,
    SHAPE_SQUARE
} ShapeType;

Shape *shape_factory(ShapeType type, int x, int y, double size) {
    switch (type) {
        case SHAPE_CIRCLE:
            return (Shape *)circle_create(x, y, size);
        case SHAPE_SQUARE:
            return (Shape *)square_create(x, y, size);
        default:
            return NULL;
    }
}

// -------------------------------------------------------------------------------------------------
// Hauptprogramm
int main(void) {
    Shape *p_shape_1 = shape_factory(SHAPE_CIRCLE, 10, 20, 5.5);
    if (p_shape_1) {
        p_shape_1->fp_draw(p_shape_1);
        free(p_shape_1);
    }

    Shape *p_shape_2 = shape_factory(SHAPE_SQUARE, 30, 40, 8.0);
    if (p_shape_2) {
        p_shape_2->fp_draw(p_shape_2);
        free(p_shape_2);
    }

    printf("Press any key to exit ....."); 
    getchar();
    return 0;
}
