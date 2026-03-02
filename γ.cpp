#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

double calc_gamma(vector<double> s_i) {
    int n = s_i.size();
    int mid = n / 2;

    nth_element(s_i.begin(), s_i.begin() + mid, s_i.end());
    double med = s_i[mid];

    if (n % 2 == 0)
    {
        nth_element(s_i.begin(), s_i.begin() + mid - 1, s_i.end());
        med = (med + s_i[mid - 1]) / 2.0;
    }

    return med;
}
