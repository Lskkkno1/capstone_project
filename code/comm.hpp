#pragma once
#include <cmath>

struct Point {
    double x_, y_, z_;
    Point(double x = 0, double y = 0, double z = 0)
        : x_(x), y_(y), z_(z) {}
    Point friend operator + (const Point &A, const Point &B) {
        return Point(A.x_ + B.x_, A.y_ + B.y_, A.z_ + B.z_);
    }
    Point friend operator - (const Point &A, const Point &B) {
        return Point(A.x_ - B.x_, A.y_ - B.y_, A.z_ - B.z_);
    }
    double len2() { return x_ * x_ + y_ * y_ + z_ * z_; }
    double len() { return std::sqrt(len2()); }
} ;

double DistCal(const Point& A, const Point& B) { return (A - B).len(); }