const int M = 1769931413;
int sum(int a, int b) {
    if ((a += b) >= M) a -= M;
    return a;
}
int dif(int a, int b) {
    if ((a -= b) < 0) a += M;
    return a;
}
int prod(int a, int b) {
    return ll(a) * b % M;
}
int invm(int a, int m) {
    if (a == 1) return 1;
    return (1 - ll(invm(m % a, a)) * m) / a + m;
}
int inv(int a) {
    return invm(a, M);
}
bool invertible(int a) {
    return a;
}
int norm(int n) {
    return n != 0;
}

int main() {
    int m, n;
    cin >> m >> n;
    vector<pair<pii, vi>> v(m);
    for (int i = 0; i < m; ++i) {
        v[i].first.second = i;
        v[i].second.resize(n);
        for (int j = 0; j < n; ++j) {
            cin >> v[i].second[j];
            if (v[i].second[j] < 0)
                v[i].second[j] += M;
        }
    }
    for (int i = 0; i < m; ++i)
        cin >> v[i].first.first;
    sort(v.begin(), v.end());
    vvi u(n, vi(m + 1));
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < m; ++j)
            u[i][j] = v[j].second[i];
    vi ans, where, perm;
    gauss(u, ans, where, perm, sum, dif, prod, inv, norm, invertible);
    vi cls;
    int cost = 0, cnt = 0;
    for (int i = 0; i < m; ++i)
        if (where[i] != -1) {
            ++cnt;
            cost += v[i].first.first;
            cls.push_back(v[i].first.second);
        }
    if (cnt < n) {
        cout << "0\n";
        return 0;
    }
    cout << cost << '\n';
    sort(cls.begin(), cls.end());
    for (int i : cls)
        cout << i + 1 << '\n';
}
