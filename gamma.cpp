#include <iostream>
#include <vector>
#include <algorithm>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
using namespace std;

double calc_gamma(vector<double> s_i)
{
    int n = s_i.size();
    int mid = n / 2;

    // 중앙값을 찾기 위해 nth_element를 사용하여 s_i를 정렬한다.
    nth_element(s_i.begin(), s_i.begin() + mid, s_i.end());
    double med = s_i[mid];

    // 짝수면 중앙값이 두 개이므로, 두 번째 중앙값을 찾아서 평균을 구한다.
    if (n % 2 == 0)
    {
        nth_element(s_i.begin(), s_i.begin() + mid - 1, s_i.end());
        med = (med + s_i[mid - 1]) / 2.0;
    }

    return med;
}

PYBIND11_MODULE(calc_gamma, m)
{
    m.def("calc_gamma", &calc_gamma); // calc_gamma 함수를 Python에서 사용할 수 있도록 pybind11로 래핑
}