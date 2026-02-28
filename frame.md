# QUBO 기반 Dataset Subset Selection 설계 문서

## 목적함수
f(x) = γΣx_i − αΣs_i x_i + βΣr_ij x_i x_j
f(x) = 선택비용 - 데이터 중요도 + 중복성

## 중요도 s_i
kNN 거리 기반 정의

## 유사도 r_ij
kNN 이웃일 때만 cosine similarity

## QUBO 변환
Diagonal:
Q_ii = γ − α s_i + λ(1−2k)

Off-diagonal:
Q_ij = β r_ij + 2λ

## 파이프라인
데이터 → s → R → Q → solver → subset
