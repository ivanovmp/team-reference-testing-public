struct FlowEdge {
	int u, v, capacity, flow;
	FlowEdge(int u, int v, int capacity, int flow = 0) : u(u), v(v), capacity(capacity), flow(flow) {
	}
	bool first(int x) const {
		return x == u;
	}
	int y(int x) const {
		return u + v - x;
	}
	int rc(int x) const {
		if (first(x))
			return capacity - flow;
		else
			return flow;
	}
	void push(int x, int val) {
		if (first(x))
			flow += val;
		else
			flow -= val;
	}
};
struct Flow {
	vector<FlowEdge> f;
	vvi a;
	Flow() {
	}
	FlowEdge& getFlowEdge(int edge) {
		return f[edge];
	}
	int addEdge(int u, int v, int capacity) {
		int edge_number = f.size();
		f.emplace_back(u, v, capacity, 0);
		if (a.size() <= max(u, v))
			a.resize(max(u, v) + 1);
		a[u].push_back(edge_number);
		a[v].push_back(edge_number);
		return edge_number;
	}
	int n() const {
		return a.size();
	}
	int maxFlow(int u, int v) {
		int ans = 0;
		while (true) {
			queue<int> q;
			q.push(u);
			vi bk(n(), -1);
			bk[u] = -2;
			while (!q.empty()) {
				int top = q.front();
				if (top == v)
					break;
				q.pop();
				for (int e : a[top]) {
					FlowEdge& ed = f[e];
					int y = ed.y(top);
					if (ed.rc(top) > 0 && bk[y] == -1) {
						bk[y] = e;
						q.push(y);
					}
				}
			}
			if (bk[v] == -1)
				break;
			vector<pair<int, int>> edges;
			int x = v;
			int F = 1e9;
			while (x != u) {
				edges.emplace_back(bk[x], f[bk[x]].y(x));
				x = edges.back().second;
				F = min(F, f[edges.back().first].rc(edges.back().second));
			}
			ans += F;
			for (const pair<int, int>& edge : edges)
				f[edge.first].push(edge.second, F);
		}
		return ans;
	}
};
