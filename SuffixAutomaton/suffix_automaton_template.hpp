namespace suffix {
    struct VectorVertex {
        vector<int> v;
        int d;
        VectorVertex(const int default_value) : d(default_value) {}
        int get_val(const int index) const {
            return index < v.size() ? v[index] : d;
        }
        void set_val(const int index, const int value) {
            if (v.size() <= index) v.resize(index + 1, d);
            v[index] = value;
        }
    };
    template<template<class, class> class Map>
    struct MapVertex {
        Map<int, int> v;
        int d;
        MapVertex(const int default_value) : d(default_value) {}
        int get_val(const int index) const {
            auto it = v.find(index);
            return it != v.end() ? it->y : d;
        }
        void set_val(const int index, const int value) {
            v[index] = value;
        }
    };

    template<class Vertex>
    struct TemplateAutomaton {
        const int ALPHABET;
        TemplateAutomaton(const vector <int> &s, const int ALPHABET) : ALPHABET(ALPHABET) {
            int n = (int) s.size();
            int max_states = max(2 * n - 1, 5);
            len.resize(max_states, 0), suflink.resize(max_states, -1);
            next_step.resize(max_states, Vertex(-1));

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
            while (p != -1 && next_step[p].get_val(c) == -1) {
                next_step[p].set_val(c, cur);
                p = suflink[p];
            }

            if (p == -1) {
                suflink[cur] = 0;
                return cur;
            }

            int q = next_step[p].get_val(c);
            if (len[q] == len[p] + 1) {
                suflink[cur] = q;
                return cur;
            }

            quant++;
            int clone = quant - 1;
            len[clone] = len[p] + 1;
            while (p != -1 && next_step[p].get_val(c) == q) {
                next_step[p].set_val(c, clone);
                p = suflink[p];
            }
            for (int i = 0; i < ALPHABET; i++) {
                int val = next_step[q].get_val(i);
                if (val != -1)
                    next_step[clone].set_val(i, val);
            }
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
        vector<Vertex> next_step;
    };
}
