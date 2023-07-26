#include <bits/stdc++.h>
#define pb push_back
#define mp make_pair
#define x first
#define y second

using namespace std;

typedef long long ll;
typedef long double ld;

namespace GeneralMatching {
    class GeneralMatching {
    public:
        GeneralMatching(int _n, const vector <vector <int> > &_g) : n(_n), g(_g) {
            match.resize(n, -1), used.resize(n), base.resize(n), parent.resize(n), blossom.resize(n);
            color.resize(n);
            for (int v = 0; v < n; v++)
                for (int u : g[v])
                    if (match[v] == -1 && match[u] == -1)
                        match[v] = u, match[u] = v;
            
            for (int v = 0; v < n; v++)
                if (match[v] == -1)
                    find_path(v);
        }

        void mark_path(int v, int b, int pr) {
            while (base[v] != b) {
                blossom[base[v]] = blossom[base[match[v]]] = true;
                parent[v] = pr, pr = match[v], v = parent[pr];
            }
        }

        int lca(int a, int b) {
            color_count++;
            for (a = base[a]; match[a] != -1; a = base[parent[match[a]]]) color[a] = color_count;
            for (b = base[b]; a != b && color[b] != color_count; b = base[parent[match[b]]]);
            return b;
        }

        void find_path(int root) {
            for (int i = 0; i < n; i++)
                base[i] = i, used[i] = 0, parent[i] = -1;

            queue <int> qu;
            qu.push(root);
            used[root] = true;
            while (!qu.empty()) {
                int v = qu.front();
                qu.pop();

                for (int u : g[v]) {
                    if (base[u] == base[v] || match[v] == u) continue;
                    if (u != root && match[u] == -1) {
                        while (u != -1) {
                            int old = match[v];
                            match[u] = v, match[v] = u;
                            u = old, v = parent[old];
                        }
                        return;
                    }
                    if (u == root || parent[match[u]] != -1) {
                        int curbase = lca(v, u);
                        for (int i = 0; i < n; i++) blossom[i] = false;
                        mark_path(v, curbase, u);
                        mark_path(u, curbase, v);
                        for (int i = 0; i < n; i++)
                            if (blossom[base[i]]) {
                                base[i] = curbase;
                                if (!used[i]) qu.push(i), used[i] = true;
                            }
                    } else if (parent[u] == -1) {
                        parent[u] = v, u = match[u];
                        qu.push(u), used[u] = true;
                    }
                }
            }
        }

        vector <int> matching() const {
            return match;
        }
    
    private:
        int n;
        vector <vector <int> > g;
        vector <int> match, base, parent;
        vector <bool> used, blossom;
        int color_count = 0;
        vector <int> color;
   };
}