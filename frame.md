# QUBO 기반 Dataset Subset Selection 설계 문서

## 목적함수
f(x) = γΣx_i − αΣs_i x_i + βΣr_ij x_i x_j  
f(x) = 선택비용 - 데이터 중요도 + 중복성

## 중요도 s_i
kNN 거리 기반 정의  
min-max 정규화

## 유사도 r_ij
kNN 이웃일 때만 cosine similarity  
rᵢⱼ = (1 + cosine(xᵢ, xⱼ)) / 2  
```python
from sklearn.metrics.pairwise import cosine_similarity

R = cosine_similarity(X)
```
​
## 선택비용 γ
γ를 일단 s_i의 median 값으로 설정하고,  
sweep을 통해 최적의 γ 찾음

## QUBO 변환
Diagonal:
Q_ii = γ − α s_i

Off-diagonal:
Q_ij = β r_ij

## 파이프라인
데이터 → s → R → Q → solver → subset
