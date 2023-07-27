namespace suffix {
    struct Automaton {
        const int ALPHABET;
        Automaton(const vector <int> &s, const int ALPHABET) : ALPHABET(ALPHABET), next_step(ALPHABET + 3) {
            int n = (int) s.size();
            int max_states = max(2 * n - 1, 5);
            len.resize(max_states, 0), suflink.resize(max_states, -1);
            for (int i = 0; i < ALPHABET; i++) next_step[i].resize(max_states, -1);

            int root = 0;
            quant++;

            int cur = root;
            for (int x : s) cur = add_symbol(cur, x);
            last_vert = cur;
        }

        int add_symbol(int last, int c) {
            quant++;
            int cur = quant - 1;
            len[cur] = len[last] + 1;

            int p = last;
            while (p != -1 && next_step[c][p] == -1) {
                next_step[c][p] = cur;
                p = suflink[p];
            }

            if (p == -1) {
                suflink[cur] = 0;
                return cur;
            }

            int q = next_step[c][p];
            if (len[q] == len[p] + 1) {
                suflink[cur] = q;
                return cur;
            }

            quant++;
            int clone = quant - 1;
            len[clone] = len[p] + 1;
            while (p != -1 && next_step[c][p] == q) {
                next_step[c][p] = clone;
                p = suflink[p];
            }
            for (int i = 0; i < ALPHABET; i++) next_step[i][clone] = next_step[i][q];
            suflink[clone] = suflink[q];
            suflink[q] = clone;
            suflink[cur] = clone;
            return cur;
        }
    vector<bool> get_terminal_states() const { // только если надо!
        vector<bool> term(len.size());
        for (int i = last_vert; i; i = suflink[i])
            term[i] = true;
        term[0] = true;
        return term;
    }

    // private:
        int quant = 0, last_vert = 0;
        vector<int> len, suflink;
        vector<vector<int>> next_step;
    };
}