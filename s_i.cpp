// 데이터의 중요도 값 s_i를 구하는 코드
// 데이터의 중요도를 데이터가 얼마나 먼 거리에 있는지로 여김
// 이유 : 데이터가 선형회귀 선에서 멀어질 수록 학습할 것이 많아진다고 고려할 수 있음
// #define DEBUG_TEST
#include <vector>
#include <cmath>
#include <algorithm>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
using namespace std;

namespace
{
    double calc_dist(const vector<double>& A, const vector<double>& B) // 두 데이터 간의 거리 구하는 함수
    {
        double s = 0.0; // 각 feature 차들의 제곱 합
        for (int i = 0; i < A.size(); i++)
        {
            s += (A[i]-B[i]) * (A[i]-B[i]);
        }
        
        s = sqrt(s); // 두 데이터 간 eclidean 거리
        return s;
    }

    void max_normalize(vector<double>& s)
    {
        auto max_it = max_element(s.begin(), s.end());

        if (*max_it == 0.0) return;

        for (auto& val : s)
        {
            val = val / *max_it;
        }
    }
}

vector<double> calc_s_i(const vector<vector<double>>& X) // s_i를 구하는 함수 / 파라미터 : X:데이터 행렬
{   
    int N = X.size(); // X행렬 데이터 개수
    
    int k; // k는 데이터 갯수에 따라서 자동 지정
    if (N <= 50)
    {
        k = 3;
    }
    else if (N <= 200)
    {
        k = 5;
    }
    else if (N <= 1000)
    {
        k = 10;
    }
    else if (N <= 10000)
    {
        k = 15;
    }
    else if (N <= 100000)
    {
        k = 20;
    }
    else {
        k = 50;
    }

    k = min(k, N-1); // k는 데이터 개수보다 작아야 함
    
    vector<double> s_i; // 각 데이터에 대한 s_i값
    for (int i = 0; i < N; i++)
    {
        vector<double> dist; // 각 데이터 i를 기준으로 다른 데이터와의 거리를 저장하는 vector
        for (int j = 0; j < N; j++)
        {
            if (i == j) continue; // 자기 자신은 계산할 필요 없음
            dist.push_back(calc_dist(X[i], X[j]));
        }

        nth_element(dist.begin(), dist.begin()+k-1, dist.end()); // k번째 이웃 거리 구하기
        s_i.push_back(dist[k-1]); // 정규화 하지 않은 s_i값 생성.
    }

    max_normalize(s_i); // 0~1사이 값으로 정규화
    return s_i; // i(i=0,1,2,3,...)에 대하여 각 s_i값 행렬 반환
}

PYBIND11_MODULE(calc_s_i, m)
{
    m.def("calc_s_i", &calc_s_i); // calc_s_i 함수를 Python에서 사용할 수 있도록 pybind11로 래핑
}

#ifdef DEBUG_TEST
#include <iostream>

int main()
{
    vector<vector<double>> X = {
        {1,2},
        {2,3},
        {3,4}
    };

    auto result = calc_s_i(X);

    for (auto v : result)
        cout << v << endl;
}
#endif