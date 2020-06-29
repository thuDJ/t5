#include <iostream>
#include <Dense>
#include "xtensor/xarray.hpp"
#include "xtensor/xio.hpp"
#include "xtensor/xview.hpp"

using Eigen::MatrixXd;
int main()
{
MatrixXd m(2,2);
m(0,0) = 3;
m(1,0) = 2.5;
m(0,1) = -1;
m(1,1) = m(1,0) + m(0,1);
std::cout << m << std::endl;

xt::xarray<double> arr1
      {{1.0, 2.0, 3.0},
       {2.0, 5.0, 7.0},
       {2.0, 5.0, 7.0}};
xt::xarray<double> arr2
      {5.0, 6.0, 7.0};
xt::xarray<double> res = xt::view(arr1, 1) + arr2;
std::cout << res << std::endl;
return 0;
}

