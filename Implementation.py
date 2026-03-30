import numpy as np
import os
os.add_dll_directory(r"C:\Users\USER\miniconda3")

# -----------------------------
# 1️⃣ modules (C++ + Python)
# -----------------------------
import calc_s_i
import calc_gamma
from r_ij import calc_r_ij


# -----------------------------
# 2️⃣ QUBO 생성 함수
# -----------------------------
def build_qubo(s_i, R, gamma, alpha=1.0, beta=1.0):
    N = len(s_i)
    Q = np.zeros((N, N))

    # diagonal
    for i in range(N):
        Q[i, i] += gamma - alpha * s_i[i]

    # off-diagonal
    for i in range(N):
        for j in range(i + 1, N):
            Q[i, j] += beta * R[i, j]
            Q[j, i] = Q[i, j]

    return Q


# -----------------------------
# 3️⃣ 전체 파이프라인
# -----------------------------
def run_pipeline(X):
    s_i = calc_s_i.calc_s_i(X)
    R = calc_r_ij(X)
    gamma = calc_gamma.calc_gamma(s_i)
    gamma = gamma * 0.8  # gamma 조정

    N = len(s_i)
    alpha = 1.0

    if np.sum(R) > 0:
        beta = np.mean(s_i) / np.mean(R[R > 0])
    else:
        beta = 1.0 / N

    Q = build_qubo(s_i, R, gamma, alpha, beta)

    return Q, s_i, R, gamma


# -----------------------------
# 4️⃣ QUBO energy
# -----------------------------
def qubo_energy(Q, x):
    return x @ Q @ x


# -----------------------------
# 5️⃣ Simulated Annealing solver
# -----------------------------
def simulated_annealing(Q, num_iter=5000, T0=10.0, alpha=0.995):
    N = Q.shape[0]

    x = np.random.randint(0, 2, size=N)
    best_x = x.copy()
    best_E = qubo_energy(Q, x)

    T = T0

    for _ in range(num_iter):
        i = np.random.randint(0, N)

        x_new = x.copy()
        x_new[i] ^= 1  # bit flip

        E_old = qubo_energy(Q, x)
        E_new = qubo_energy(Q, x_new)

        dE = E_new - E_old

        if dE < 0 or np.random.rand() < np.exp(-dE / T):
            x = x_new

            if E_new < best_E:
                best_x = x.copy()
                best_E = E_new

        T *= alpha

    return best_x, best_E


# -----------------------------
# 6️⃣ solve 함수
# -----------------------------
def solve_qubo(Q, num_runs=10):
    best_x = None
    best_E = float('inf')

    for _ in range(num_runs):
        x, E = simulated_annealing(Q)

        if E < best_E:
            best_x = x
            best_E = E

    return best_x, best_E


# -----------------------------
# 7️⃣ 실행 예제
# -----------------------------
if __name__ == "__main__":

    X = np.array([
        [1, 2],
        [2, 3],
        [3, 4],
        [10, 10],
        [11, 11],
        [50, 50]
    ], dtype=float)

    Q, s_i, R, gamma = run_pipeline(X)

    x, energy = solve_qubo(Q)

    print("===== 결과 =====")
    print("gamma:", gamma)
    print("selected count:", np.sum(x))
    print("selected index:", np.where(x == 1)[0])
    print("energy:", energy)
    print("s_i:", s_i)
    s_i = np.array(s_i)
    print("selected s_i:", s_i[[0,3,4,5]])
