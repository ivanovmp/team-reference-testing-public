struct Circulation {
	Flow f;
	vi balance;
	vi lowers;
	int addEdge(int u, int v, int lower, int upper) {
		lowers.push_back(lower);
		int edge = f.addEdge(u, v, upper - lower);
		if (balance.size() <= max(u, v))
			balance.resize(max(u, v) + 1);
		balance[u] += lower;
		balance[v] -= lower;
		return edge;
	}
	int n() const {
		return balance.size();
	}
	bool check() {
		vi g(n());
		int full = 0;
		for (int i = 0; i < n(); ++i) {
			if (balance[i] >= 0)
				g[i] = f.addEdge(i, n() + 1, balance[i]);
			else
				g[i] = f.addEdge(n(), i, -balance[i]);
			full += abs(balance[i]);
		}
		return f.maxFlow(n(), n() + 1) == full / 2;
	}
	int getFinal(int edge) {
		return lowers[edge] + f.getFlowEdge(edge).flow;
	}
};
struct dsu {
	vi p, r;
	dsu(int n) : p(n), r(n) {
		for (int i = 1; i < n; ++i)
			p[i] = i;
	}
	int find(int x) {
		if (x == p[x])
			return x;
		else
			return p[x] = find(p[x]);
	}
	bool unite(int x, int y) {
		x = find(x);
		y = find(y);
		if (x == y)
			return false;
		if (r[x] < r[y])
			swap(x, y);
		if (r[x] == r[y])
			++r[x];
		p[y] = x;
		return true;
	}
};
int main() {
	int n, m;
	cin >> n >> m;
	Circulation c;
	dsu d(n);
	int s = n;
	vi y(n);
	vi lost(n, -1);
	for (int i = 1; i < n; ++i)
		y[i] = i;
	for (int i = 0; i < m; ++i) {
		int t;
		cin >> t;
		if (t == 1) {
			int u, v;
			cin >> u >> v;
			--u; --v;
			u = d.find(u);
			v = d.find(v);
			d.unite(u, v);
			c.addEdge(y[u], s, 0, n);
			c.addEdge(y[v], s, 0, n);
			y[d.find(u)] = s;
			++s;
		} else if (t == 2) {
			int u;
			cin >> u;
			--u;
			lost[u] = c.addEdge(y[d.find(u)], u, 0, 1);
		} else {
			int u, l, h;
			cin >> u >> l >> h;
			--u;
			u = d.find(u);
			c.addEdge(y[u], s, l, h);
			y[u] = s;
			++s;
		}
	}
	for (int u = 0; u < n; ++u)
		if (lost[u] == -1)
			lost[u] = c.addEdge(y[d.find(u)], u, 0, 1);
	if (!c.check())
		cout << "NO\n";
	else {
		cout << "YES\n";
		for (int i = 0; i < n; ++i)
			if (c.getFinal(lost[i]) == 1)
				cout << 'R';
			else
				cout << 'B';
		cout << '\n';
	}
}
