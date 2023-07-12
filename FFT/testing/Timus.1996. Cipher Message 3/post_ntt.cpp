int most_significant_bit(const uint32_t n) {
    if (n == 0) return -1;
#ifdef __GNUC__
    return 8 * sizeof(uint32_t) - 1 - __builtin_clz(n);
#else
    for (int i = 8 * sizeof(uint32_t) - 1; i >= 0; --i)
        if (n & 1 << i) return i;
#endif
}

int main() {
    int n, m;
    cin >> n >> m;
    if (m > n) {
        cout << "No\n";
        return 0;
    }
    const int fft_size = most_significant_bit(n + m - 2) + 1;
    FFT::NTT fft(fft_size);
    vb sa(n); string ma(n, '\0');
    for (int i = 0; i < n; ++i) {
        string s;
        cin >> s;
        for (int j = 0; j < 7; ++j)
            ma[i] = ma[i] * 2 + (s[j] == '1');
        sa[i] = s[7] == '1';
    }
    vb sb(m); string mb(m, '\0');
    for (int i = 0; i < m; ++i) {
        string s;
        cin >> s;
        for (int j = 0; j < 7; ++j)
            mb[i] = mb[i] * 2 + (s[j] == '1');
        sb[i] = s[7] == '1';
    }

    vi a0(n), a1(n);
    for (int i = 0; i < n; ++i) {
        a0[i] = !sa[i];
        a1[i] = sa[i];
    }
    vi b0(m), b1(m);
    for (int i = 0; i < m; ++i) {
        b0[m - 1 - i] = !sb[i];
        b1[m - 1 - i] = sb[i];
    }
    vi c0 = fft.prod(a0, b0), c1 = fft.prod(a1, b1);
    c0.resize(n + m - 1); c1.resize(n + m - 1);
    vi C(n + m - 1);
    for (int i = 0; i < n + m - 1; ++i)
        C[i] += c0[i] + c1[i];
    mb += ma;
    vi z = z_function(mb.begin(), mb.end());
    pii ans{1e9, 0};
    for (int i = 0; i <= n - m; ++i)
        if (z[i + m] >= m)
            ans = min(ans, {m - C[m + i - 1], i + 1});
    if (ans.x <= m)
        cout << "Yes\n" << ans.x << ' ' << ans.y << '\n';
    else
        cout << "No\n";
}
