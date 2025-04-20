#include "comm.hpp"
#include "ThreadPool/ThreadPool.h"

#include <locale>
#include <iostream>
#include <vector>
#include <algorithm>
#include <sstream>

#ifdef _WIN32
    #include <windows.h>
#endif

void setup_console() {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
#elif defined(__unix__) || defined(__APPLE__)
    std::locale::global(std::locale("en_US.UTF-8"));
#else
    #error "Unsupported platform"
#endif
}

void InputData(int &n, double &rc, double &x_lim, double &y_lim, 
    double &z_lim, std::vector<Point>& parr, bool flag = 0) {
    if (flag) std::cout << "输入体系中原子个数 n:" << std::endl;
    std::cin >> n;
    if (flag) std::cout << "输入体系原子作用力截断距离 R_c" << std::endl;
    std::cin >> rc;
    if (flag) std::cout << "输入体系 x, y, z 坐标最大值:" << std::endl;
    // 0 <= x <= x_lim, 0 <= y <= y_lim, 0 <= z <= z_lim
    std::cin >> x_lim >> y_lim >> z_lim;
    if (flag) std::cout << "输入体系 n 个原子的坐标 x, y, z(输入n行, 坐标用空格隔开):" << std::endl;
    parr.resize(n);
    for (int i = 0; i < n; ++i)
        std::cin >> parr[i].x_ >> parr[i].y_ >> parr[i].z_;
}
void OutputData(std::vector<Point>& parr) {
    std::cout << "映像原子列表共包含" << parr.size() << "个原子, 坐标如下:" << std::endl;
    for (const auto& pi : parr)
        std::cout << pi.x_ << ' ' << pi.y_ << ' ' << pi.z_ << std::endl;
}

Point bf_check(const Point &p, const std::vector<Point>& parr, const double &rc) {
    for (const auto& pi : parr) {
        if (fcmp(rc, DistCal(p, pi)) >= 0) {
            return p;
        }
    }
    return NULL_POINT;
}

int main() {
    setup_console();
    int n;
    double rc, x_lim, y_lim, z_lim;
    std::vector<Point> p_real;
    InputData(n, rc, x_lim, y_lim, z_lim, p_real, 1);
    std::vector<Point> p_mirror;
    p_mirror.reserve(n / 8);

    ThreadPool pool(8);
    std::vector< std::future<Point> > results;
    for (int i = 0; i < n; ++i) {
        for (int mask = 1; mask < 8; ++mask) {
           results.emplace_back(
                pool.enqueue(
                    [&, i, mask](){
                        Point tp = p_real[i];
                        
                        // std::stringstream ss;
                        // ss << i << " : " << tp.x_ << ' ' << tp.y_ << ' ' << tp.z_ << std::endl;
                        // std::cerr << ss.str() ;
 
                        if (mask & 1) {
                            tp.x_ = (tp.x_ * 2 < x_lim) ? tp.x_ + x_lim : tp.x_ - x_lim;
                        }
                        if (mask >> 1 & 1) {
                            tp.y_ = (tp.y_ * 2 < y_lim) ? tp.y_ + y_lim : tp.y_ - y_lim;
                        }
                        if (mask >> 2 & 1) {
                            tp.z_ = (tp.z_ * 2 < z_lim) ? tp.z_ + z_lim : tp.z_ - z_lim;
                        }
                        return bf_check(tp, p_real, rc);
                    }
                )
            );
        }
    }

    for (auto &&result : results) {
        Point p = result.get();
        if (p == NULL_POINT) continue;
        p_mirror.emplace_back(p);
    }
    
    // unique
    std::sort(p_mirror.begin(), p_mirror.end());
    p_mirror.erase(std::unique(p_mirror.begin(), p_mirror.end()), p_mirror.end());

    OutputData(p_mirror);
    system("pause");
    return 0;
}