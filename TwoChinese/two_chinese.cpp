namespace TwoChinese {
    struct Edge {
        int from, to, weight;
    };
    class TwoChinese {
    public:
        TwoChinese(int _n, const vector <Edge> &_edges, int _root) : n(_n), edges(_edges), root(_root) {
            komp.resize(n), metka.resize(n), in_edges.resize(n);
            delta.resize(n), parent.resize(n), used.resize(n), finish.resize(n);
            for (int i = 0; i < n; i++) metka[i] = i, komp[i].pb(i);
            for (int i = 0; i < (int) edges.size(); i++)
                in_edges[edges[i].to].insert(mp(edges[i].weight, i));
            finish[root] = true;
            for (int vert = 0; vert < n; vert++)
                if (!finish[vert]) {
                    path.clear();
                    build_path(metka[vert], -1);
                }
            for (int i = 0; i < n; i++)
                if (i != root) total += edges[parent[i]].weight;
        }
        void build_path(int v, int last) {
            path.pb(mp(v, last));
            used[v] = true;
            while (metka[edges[in_edges[v].begin()->y].from] == v)
                in_edges[v].erase(in_edges[v].begin());
            delta[v] = in_edges[v].begin()->x;
            int num = in_edges[v].begin()->y;
            int u = metka[edges[num].from];
            if (finish[u]) {
                for (const auto &[v0, nm] : path)
                    for (int u0 : komp[v0])
                        finish[u0] = true;
                path.pb(mp(u, num));
                for (int i = 0; i < (int) path.size() - 1; i++)
                    parent[path[i].x] = path[i + 1].y;
                return;	
            }
            if (!used[u]) {
                build_path(u, num);
                return;
            }
            vector <pair <int, int> > cycle;
            int new_vert = u;
            while (true) {
                pair <int, int> A = path.back();
                cycle.pb(A);
                path.pop_back();
                if ((int) komp[A.x].size() > (int) komp[new_vert].size()) new_vert = A.x;
                if (A.x == u) break;
            }
            int old = cycle.back().y;
            cycle.back().y = num;
            vector <pair <int, int> > otkat;
            for (const auto &[v0, nm] : cycle) {
                if (v0 == new_vert) continue;
                for (int u0 : komp[v0]) {
                    otkat.pb(mp(u0, metka[u0]));
                    metka[u0] = new_vert, komp[new_vert].pb(u0);
                }
                for (const auto &[w, nums] : in_edges[v0])
                    in_edges[new_vert].insert(mp(w - delta[v0] + delta[new_vert], nums));
            }
            build_path(new_vert, old);
            int need = parent[new_vert];
            for (int i = 0; i < (int) cycle.size() - 1; i++)
                parent[cycle[i + 1].x] = cycle[i].y;
            parent[cycle[0].x] = cycle.back().y;
            while (!otkat.empty()) {
                pair <int, int> A = otkat.back();
                otkat.pop_back();
                metka[A.x] = A.y;
            }
            parent[metka[edges[need].to]] = need;
        }
    private:
        int n;
        vector <Edge> edges;
        int root;
        ll total = 0;
        vector <int> parent, metka;
        vector <vector <int> > komp;
        vector <ll> delta;
        vector <set <pair <ll, int> > > in_edges;
        vector <bool> finish, used;
        vector <pair <int, int> > path;
    };
}