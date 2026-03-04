// 데이터의 중요도 값 s_i를 구하는 코드
// 데이터의 중요도를 데이터가 얼마나 먼 거리에 있는지로 여김
// 이유 : 데이터가 선형회귀 선에서 멀어질 수록 학습할 것이 많아진다고 고려할 수 있음
#include <vector>
#include <cmath>
#include <algorithm>

using namespace std;

double calc_dist(vector<double> A, vector<double> B) // 두 데이터 간의 거리 구하는 함수
{
    double s; // 각 feature 차들의 제곱 합
    for (int i = 0; i < A.size(); i++)
    {
        s += (A[i]-B[i]) * (A[i]-B[i]);
    }
    
    s = sqrt(s); // 두 데이터 간 eclidean 거리
    return s;
}

vector<double> calc_s_i(vector<vector<double>> X) // s_i를 구하는 함수 / 파라미터 : X:데이터 행렬
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
        s_i[i].push_back(dist[k-1]);
    }

    return s_i; // i(i=0,1,2,3,...)에 대하여 각 s_i값 행렬 반환
}
