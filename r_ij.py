import numpy as np
from sklearn.preprocessing import StandardScaler
from sklearn.metrics.pairwise import cosine_similarity
from sklearn.neighbors import NearestNeighbors


def auto_select_k(N: int) -> int:
    """
    데이터 개수 N에 따라 k 자동 선택
    """

    if N <= 50:
        return 3
    elif N <= 200:
        return 5
    elif N <= 1000:
        return 10
    elif N <= 10000:
        return 15
    elif N <= 100000:
        return 20
    else:
        return 50


def calc_r_ij(X: np.ndarray) -> np.ndarray:
    """
    X : (N,d) 데이터 행렬
    return:
        R : (N,N) symmetric masked cosine similarity matrix
    """

    N = X.shape[0]

    # -----------------------------
    # 1️⃣ k 자동 선택
    # -----------------------------
    k = auto_select_k(N)

    # -----------------------------
    # 2️⃣ feature 정규화
    # -----------------------------
    scaler = StandardScaler()
    X_scaled = scaler.fit_transform(X)

    # -----------------------------
    # 3️⃣ cosine similarity 계산
    # -----------------------------
    S = cosine_similarity(X_scaled)

    # -----------------------------
    # 4️⃣ kNN adjacency 생성
    # -----------------------------
    nbrs = NearestNeighbors(n_neighbors=k).fit(X_scaled)
    _, indices = nbrs.kneighbors(X_scaled)

    A = np.zeros((N, N), dtype=np.float32)

    for i in range(N):
        for j in indices[i]:
            if i != j:
                A[i, j] = 1

    # -----------------------------
    # 5️⃣ 대칭화 (OR 방식)
    # -----------------------------
    A = np.maximum(A, A.T)

    # -----------------------------
    # 6️⃣ mask 적용
    # -----------------------------
    R = A * S

    # 자기 자신 제거
    np.fill_diagonal(R, 0.0)

    return R