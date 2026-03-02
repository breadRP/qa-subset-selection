import numpy as np
from sklearn.preprocessing import StandardScaler
from sklearn.metrics.pairwise import cosine_similarity

def calc_r_ij(X: np.ndarray) -> np.ndarray:
    X_scaled = StandardScaler().fit_transform(X)
    return (1 + cosine_similarity(X_scaled)) / 2