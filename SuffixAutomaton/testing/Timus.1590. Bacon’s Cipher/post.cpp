int main() {
    string s;
    cin >> s;
    vi a(s.begin(), s.end());
    for (int& i : a) if (i < 0) i += 256;
    const int ALPHABET = 256;
    suffix::Automaton automaton(a, ALPHABET);
    vi ans(automaton.quant, 1);
    for (int i = automaton.quant - 1; i >= 0; --i)
        for (int j = 0; j < ALPHABET; ++j) {
            int to = automaton.next_step[j][i];
            if (to != -1)
                ans[i] += ans[to];
        }
    cout << ans[0] - 1 << '\n';
}
