namespace DominatorsTree {
    const int H = 20;

    class DominatorsTree {
    public:
        void dfs(int v) {
            renum[v] = timer++;
            revnum.pb(v);
            for (int u : g[v])
                if (renum[u] == -1) {
                    dfs(u);
                    sons[v].pb(u);
                }
            return;
        }

        int find_parent(int v) {
            if (v == parent[v]) return v;
            int p = parent[v];
            parent[v] = find_parent(p);
            min_sdom[v] = min(min_sdom[v], min_sdom[p]);
            return parent[v];
        }

        int get_minimum(int v) {
            find_parent(v);
            return min_sdom[v];
        }

        void unions(int a, int b) {
            parent[b] = a;
            return;
        }

        void dfsik(int v, int pr) {
            dv[0][v] = mp(pr, v);
            for (int i = 1; i <= H; i++)
                if (dv[i - 1][v].x != -1) {
                    dv[i][v].x = dv[i - 1][dv[i - 1][v].x].x;
                    int a = dv[i - 1][v].y;
                    int b = dv[i - 1][dv[i - 1][v].x].y;
                    if (sdom[a] > sdom[b]) swap(a, b);
                    dv[i][v].y = a;
                }
                else break;

            if (pr != -1) {
                int cur = v, mn = v;
                for (int i = H; i >= 0; i--)
                    if (dv[i][cur].x != -1 && renum[dv[i][cur].x] >= sdom[v]) {
                        int a = dv[i][cur].y;
                        if (sdom[a] < sdom[mn]) mn = a;
                        cur = dv[i][cur].x;
                    }
                if (sdom[v] == sdom[mn]) dom[v] = sdom[v];
                else dom[v] = dom[mn];
            }

            for (int u : sons[v])
                dfsik(u, v);
            return;
        }
        
        DominatorsTree(int n, const vector <vector <int> > _g, int start) : g(_g) {
            rev.resize(n), sons.resize(n);
            renum.resize(n, -1), dom.resize(n, -1), sdom.resize(n, n);
            for (int v = 0; v < n; v++)
                for (int u : g[v])
                    rev[u].pb(v);

            dfs(start);

            parent.resize(n), min_sdom.resize(n);
            for (int i = 0; i < n; i++) parent[i] = i;

            for (int i = (int) revnum.size() - 1; i >= 0; i--) {
                int v = revnum[i];
                for (int u : rev[v]) {
                    if (renum[u] == -1 || u == v) continue;
                    if (renum[u] < i) sdom[v] = min(sdom[v], renum[u]);
                    else sdom[v] = min(sdom[v], get_minimum(u));
                }

                min_sdom[v] = sdom[v];
                for (int u : sons[v])
                    unions(v, u);
            }
            sdom[start] = 0;

            for (int i = 0; i <= H; i++) dv[i].resize(n);
            dfsik(start, -1);

            for (int i = 0; i < n; i++)
                if (dom[i] != -1) dom[i] = revnum[dom[i]];
        }

        vector <int> dominator_tree() const {
            return dom;
        }
    private:
        vector <vector <int> > g, rev, sons;
        vector <int> dom, sdom;
        vector <int> renum, revnum;
        int timer = 0;

        vector <int> parent, min_sdom;
        vector <pair <int, int> > dv[H + 2];
    };
}