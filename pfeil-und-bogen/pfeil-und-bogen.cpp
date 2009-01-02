// g++ -O2 -Wall -o pfeil-und-bogen -I/opt/boost/include/boost-1_37 pfeil-und-bogen.cpp

#include <cstdlib>
#include <vector>
#include <utility>
#include <iostream>
#include <iomanip>
#include <math.h>

#include <boost/math/distributions.hpp>
#include <boost/math/special_functions.hpp>


int main(int argc, char* argv[])
{
    double p_disc = 0.9973;
    if (argc > 1)
        p_disc = atof(argv[1]);
    if (p_disc <= 0.0 || p_disc >= 1.0)
        return EXIT_FAILURE;
    std::cout << "chance to hit the disc: " << p_disc * 100 << '%' << std::endl;
    boost::math::normal dist;
    std::vector<std::pair<double, double> > q(16+1);
    for (int i = 0; i <= 16; ++i) {
        double p =  (double) i / 32.0 * p_disc + 0.5;
        q[i] = std::make_pair(p, sqrt(boost::math::quantile(dist, p)));
    }
    double sigma = q[16].second;
    std::cout << "Ring    p                       Quantil         r"
              << std::endl;
    std::cout << "-------------------------------------------------------"
              << std::endl;
    for (int i = 0; i <= 16; ++i) {
        std::cout << std::right << std::setw(4)
                  << i << '\t' 
                  << std::setprecision(10) << std::setw(19) << std::left
                  << q[i].first << '\t'
                  << std::setprecision(5) << std::setw(9) << std::left
                  << q[i].second << '\t'
                  << std::setprecision(4) << std::setw(9) << std::left
                  << q[i].second / sigma
                  << std::endl;
    }
    std::cout << std::endl;
    return EXIT_SUCCESS;
}
