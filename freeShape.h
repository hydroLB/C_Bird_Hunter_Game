#ifndef INC_3D_GRAPHICS_FreeShape_H
#define INC_3D_GRAPHICS_FreeShape_H

#include <iostream>
#include <vector>

struct point {
    double x;
    double y;
};

struct color2 {
    double red;
    double green;
    double blue;
    double alpha; // Opacity (0 is transparent, 1 is solid)

    /* Constructors */
    color2();
    color2(double r, double g, double b);
    color2(double r, double g, double b, double a);

    /* Overloaded Operator */
    friend std::ostream& operator << (std::ostream& outs, const color2 &c);
};

class FreeShape {
private:
    point center;
    unsigned int edge_length;
    std::vector<point> corners;

    // Private helper methods
    void reset_corners();
    void draw_point(const point &p) const;

public:

    // Constructors
    FreeShape();
    FreeShape(point center, point p1, point p2, point p3, point p4);

    // Sets the center
    void setCenter(point c);

    // Sets the points
    void setPoints(point p1, point p2, point p3, point p4);

    std::vector<point> getPoints();


    // Draw the FreeShape
    void draw(color2 c) const;

    // Rotate the FreeShape
    void rotate(double theta_z, bool clockwise);

    // Move the FreeShape
    void move(double delta_x, double delta_y);

    // Grow the FreeShape
    void grow();

    // Shrink the FreeShape
    void shrink();
};


#endif //INC_3D_GRAPHICS_FreeShape_H
