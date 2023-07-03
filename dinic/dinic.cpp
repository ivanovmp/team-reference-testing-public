const int MAXN = ...;
const int INF = 1e9 + 9;

struct edge {
    int a, b, cap, flow;
};

int n, s, t, d[MAXN], ptr[MAXN], q[MAXN];
vector<edge> e;
vector<int> g[MAXN];

void add_edge(int a, int b, int cap) {
    edge e1 = { a, b, cap, 0 };
    edge e2 = { b, a, cap, cap };
    g[a].push_back((int) e.size());
    e.push_back (e1);
    g[b].push_back((int) e.size());
    e.push_back (e2);
}

bool bfs() {
    int qh=0, qt=0;
    q[qt++] = s;
    fill(d, d + n, -1);
    d[s] = 0;
    while (qh < qt && d[t] == -1) {
        int v = q[qh++];
        for (size_t i=0; i<g[v].size(); ++i) {
            int id = g[v][i],
                to = e[id].b;
            if (d[to] == -1 && e[id].flow < e[id].cap) {
                q[qt++] = to;
                d[to] = d[v] + 1;
            }
        }
    }
    return d[t] != -1;
}

int dfs(int v, int flow, int can) {
    if (!flow) return 0;
    if (v == t) return flow;
    for (; ptr[v]<(int)g[v].size(); ++ptr[v]) {
        int id = g[v][ptr[v]],
            to = e[id].b;
        if (d[to] != d[v] + 1 || e[id].cap - e[id].flow < can) continue;
        int pushed = dfs(to, min (flow, e[id].cap - e[id].flow), can);
        if (pushed) {
            e[id].flow += pushed;
            e[id^1].flow -= pushed;
            return pushed;
        }
    }
    return 0;
}
int dinic() {
    int flow = 0;
    for (;;) {
        if (!bfs()) break;
        for (int k = 30; k >= 0; k--) { // just k = 0, if no scaling
			fill(ptr, ptr + n, 0);
            while (int pushed = dfs(s, INF, (1 << k)))
                flow += pushed;
        }
    }
    return flow;
}
