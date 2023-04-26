#include <math.h>
#include <iostream>
#include "freeShape.h"
#include "graphics.h"


/********************* color2 Struct ********************/

color2::color2() : red(0.0), green(0.0), blue(0.0), alpha(1.0) {
}

color2::color2(double r, double g, double b, double a) : red(r), green(g), blue(b), alpha(a) {
}

color2::color2(double r, double g, double b) : red(r), green(g), blue(b), alpha(1.0) {
}

std::ostream& operator << (std::ostream& outs, const color2 &c) {
    outs << "(" << c.red << ", " << c.green << ", " << c.blue << ")";
    return outs;
}

/****************** Private helper methods *******************/
void FreeShape::reset_corners() {
    corners.clear();
    corners.resize(4);
    corners[0] = {center.x + (edge_length / 2.0), center.y + (edge_length / 2.0)};
    corners[1] = {center.x - (edge_length / 2.0), center.y + (edge_length / 2.0)};
    corners[2] = {center.x + (edge_length / 2.0), center.y - (edge_length / 2.0)};
    corners[3] = {center.x - (edge_length / 2.0), center.y - (edge_length / 2.0)};
}

void FreeShape::draw_point(const point &p) const {
    glVertex3f(p.x, p.y,0);
}

/********************** Public methods *************************/
FreeShape::FreeShape() {
    center = {0, 0};
    edge_length = 50;
    reset_corners();
}

FreeShape::FreeShape(point center, point p1, point p2, point p3, point p4) {
    this->center = center;
    corners[0] = p1;
    corners[1] = p2;
    corners[2] = p3;
    corners[3] = p4;
}

void FreeShape::setCenter(point c){
    center = c;
}

void FreeShape::setPoints(point p1, point p2, point p3, point p4){
    corners[0] = p1;
    corners[1] = p2;
    corners[2] = p3;
    corners[3] = p4;
}

std::vector<point> FreeShape::getPoints() {
    return corners;
}

void FreeShape::draw(color2 c) const {
    glBegin(GL_QUADS);

    glColor3f(c.red, c.green, c.blue);
    draw_point(corners[0]);
    draw_point(corners[1]);
    draw_point(corners[3]);
    draw_point(corners[2]);
    
    glEnd();
}

void FreeShape::rotate(double theta_z, bool clockwise) {
    double old_x = 0, old_y = 0, old_z = 0;
    double center_x = center.x, center_y = center.y;
    // Move to origin
    move(-center_x, -center_y);

    // Rotate
    for (point &p : corners) {
        old_x = p.x;
        old_y = p.y;
        if (clockwise) {
            p.x = old_x * cos(theta_z) - old_y * sin(theta_z);
            p.y = old_x * sin(theta_z) + old_y * cos(theta_z);
        } else {
            p.x = old_x * cos(theta_z) + old_y * sin(theta_z);
            p.y = - old_x * sin(theta_z) + old_y * cos(theta_z);
        }
    }
    // Move back to position
    move(center_x, center_y);
}

void FreeShape::move(double delta_x, double delta_y) {
    center.x += delta_x;
    for (point &p : corners) {
        p.x += delta_x;
    }
    center.y += delta_y;
    for (point &p : corners) {
        p.y += delta_y;
    }
}

void FreeShape::grow() {
    double center_x = center.x, center_y = center.y;
    // Move to origin
    move(-center_x, -center_y);
    for (point &p : corners) {
        p.x += p.x * .1;
        p.y += p.y * .1;
    }
    move(center_x, center_y);
}

void FreeShape::shrink() {
    double center_x = center.x, center_y = center.y;
    // Move to origin
    move(-center_x, -center_y);
    for (point &p : corners) {
        p.x = p.x / 1.1;
        p.y = p.y / 1.1;
    }
    move(center_x, center_y);
}