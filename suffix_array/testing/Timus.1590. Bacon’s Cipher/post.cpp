int main() {
    string s;
    cin >> s;
    vi suf = build_suffix_array<uint8_t>(s, 'z' + 1);
    vi lcp = build_lcp(s, suf);
    ll ans = s.size() * (s.size() + 1ll) / 2;
    for (int i : lcp) ans -= i;
    cout << ans << '\n';
}
