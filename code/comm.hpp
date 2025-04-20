#pragma once
#include <cmath>
#include <tuple>

const double eps = 1e-8;

int fcmp(const double &A, const double &B) {
    if (fabs(A - B) < eps) return 0;
    return A - B > 0 ? 1 : -1;
}

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
    friend bool operator < (const Point& A, const Point& B) {
        return fcmp(A.x_, B.x_) != 0 ? A.x_ < B.x_ :
               fcmp(A.y_, B.y_) != 0 ? A.y_ < B.y_ : A.z_ < B.z_;
    }
    friend bool operator == (const Point& A, const Point& B) {
        return fcmp(A.x_, B.x_) == 0 && 
               fcmp(A.y_, B.y_) == 0 && 
               fcmp(A.z_, B.z_) == 0;
    } 
} ;

Point NULL_POINT(1e18, 1e18, 1e18);

double DistCal(const Point& A, const Point& B) { return (A - B).len(); }