int main() {
    string s;
    cin >> s;
    vi o = build_odd_palindromes(s),
       e = build_even_palindromes(s);
    pii p{};
    for (int i = 0; i < s.size(); ++i) {
        if (o[i] * 2 - 1 > p.y - p.x) p = {i - o[i] + 1, i + o[i]};
        if (e[i] * 2 > p.y - p.x) p = {i - o[i], i + o[i]};
    }
    cout << s.substr(p.x, p.y - p.x);
}
