#include <bits/stdc++.h>
using namespace std;

#define int long long

const int MOD = 998244353;
const int MAXN = 3e6 + 5;

int factorial[MAXN], inverse[MAXN], invFactorial[MAXN];

int modAdd(int a, int b) {
    return (a + b + 2 * MOD) % MOD;
}

int modMul(int a, int b) {
    return a * b % MOD;
}

int modPow(int base, int exponent) {
    int result = 1;
    while (exponent) {
        if (exponent & 1) result = modMul(result, base);
        base = modMul(base, base);
        exponent >>= 1;
    }
    return result;
}

void precomputeFactorials() {
    factorial[0] = factorial[1] = 1;
    inverse[0] = inverse[1] = 1;
    invFactorial[0] = invFactorial[1] = 1;
    for (int i = 2; i < MAXN; ++i) {
        factorial[i] = modMul(factorial[i - 1], i);
        inverse[i] = MOD - modMul(MOD / i, inverse[MOD % i]);
        invFactorial[i] = modMul(invFactorial[i - 1], inverse[i]);
    }
}

void NTT(vector<int> &poly) {
    int n = poly.size();
    for (int i = 1, j = 0; i < n; ++i) {
        int bit = n >> 1;
        for (; j & bit; bit >>= 1) j ^= bit;
        j ^= bit;
        if (i < j) swap(poly[i], poly[j]);
    }
    int root = modPow(3, (MOD - 1) / n);
    for (int len = 2; len <= n; len <<= 1) {
        int wlen = modPow(root, n / len);
        for (int i = 0; i < n; i += len) {
            int w = 1;
            for (int j = 0; j < len / 2; ++j) {
                int u = poly[i + j];
                int v = modMul(w, poly[i + j + len / 2]);
                poly[i + j] = modAdd(u, v);
                poly[i + j + len / 2] = modAdd(u, -v);
                w = modMul(w, wlen);
            }
        }
    }
}

vector<int> multiplyPolynomials(vector<int> &P, vector<int> &Q) {
    vector<int> A(P), B(Q);
    int size = 1;
    while (size < A.size() + B.size()) size <<= 1;
    A.resize(size);
    B.resize(size);

    NTT(A);
    NTT(B);
    for (int i = 0; i < size; ++i) A[i] = modMul(A[i], B[i]);
    NTT(A);
    reverse(A.begin() + 1, A.end());

    vector<int> result(size);
    int invN = modPow(size, MOD - 2);
    for (int i = 0; i < size; ++i) result[i] = modMul(A[i], invN);
    return result;
}

void solve() {
    int powerN, maxIndex;
    cin >> powerN >> maxIndex;

    vector<int> basePoly(maxIndex + 1), resultPoly(1, 1);
    for (int i = 0; i <= maxIndex; ++i)
        basePoly[i] = invFactorial[i];

    while (powerN) {
        if (powerN & 1) resultPoly = multiplyPolynomials(resultPoly, basePoly);
        basePoly = multiplyPolynomials(basePoly, basePoly);
        powerN >>= 1;
    }

    int queries;
    cin >> queries;
    while (queries--) {
        int index;
        cin >> index;
        cout << modMul(factorial[index], resultPoly[index]) << ' ';
    }
    cout << '\n';
}

int32_t main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    precomputeFactorials();

    int testCases = 1;

    while (testCases--) {
        solve();
        cout << '\n';
    }
    return 0;
}
