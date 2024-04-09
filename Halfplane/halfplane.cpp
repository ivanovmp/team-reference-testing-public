namespace Halfplane
{
	// bounding box ~ MAXC^3
	const ll INF = (ll) 1e18;
	const ld EPS = 1e-9;

   	ll vecs(pair <int, int> A, pair <int, int> B) {
   	 	return (ll) A.x * B.y - (ll) A.y * B.x;
   	}

   	ll scal(pair <int, int> A, pair <int, int> B) {
   	 	return (ll) A.x * B.x + (ll) A.y * B.y;
   	}

	struct Halfplane
	{
		// Ax + By + C >= 0
		int A, B;
		ll C;

		bool outside(pair <ld, ld> point) const {
		 	return A * point.x + B * point.y + C < -EPS;
		}

		bool operator < (const Halfplane &other) const {
		 	bool fl1 = (B > 0) || (B == 0 && A > 0);
		 	bool fl2 = (other.B > 0) || (other.B == 0 && other.A > 0);
		 	if (fl1 != fl2) return fl1;
		 	ll v = vecs(mp(A, B), mp(other.A, other.B));
		 	if (v != 0) return v > 0;
		 	return 1.0 * C / sqrtl((ll) A * A + (ll) B * B) < 1.0 * other.C / sqrtl((ll) other.A * other.A + (ll) other.B * other.B);
		}
	};

	pair <ld, ld> intersect(Halfplane L1, Halfplane L2) {
		ll X = L2.B * L1.C - L1.B * L2.C;
		ll Y = L1.A * L2.C - L2.A * L1.C;
		ll Z = (ll) L2.A * L1.B - (ll) L1.A * L2.B;
		return mp(1.0 * X / Z, 1.0 * Y / Z); 	
	}

	vector <pair <ld, ld> > intersection(const vector <Halfplane> &halfplanes) {
		/*vector <Halfplane> bounding_box = {
			{0, 1, INF},
			{-1, 0, INF},
			{0, -1, INF},
			{1, 0, INF},	
		};*/
		vector <Halfplane> hp = halfplanes;
		//hp.insert(hp.end(), bounding_box.begin(), bounding_box.end());

		sort(hp.begin(), hp.end());
		deque <Halfplane> dq;
		for (const Halfplane &h : hp) {
		 	while ((int) dq.size() >= 2 && h.outside(intersect(dq.rbegin()[0], dq.rbegin()[1]))) dq.pop_back();
		 	while ((int) dq.size() >= 2 && h.outside(intersect(dq[0], dq[1]))) dq.pop_front();
		 	if (!dq.empty() && vecs(mp(h.A, h.B), mp(dq.back().A, dq.back().B)) == 0)
		 	{
		 	 	if (scal(mp(h.A, h.B), mp(dq.back().A, dq.back().B)) < 0) return {};
		 	 	continue;
		 	}
		 	dq.pb(h);
		}

		while ((int) dq.size() > 2 && dq[0].outside(intersect(dq.rbegin()[0], dq.rbegin()[1]))) dq.pop_back();
		while ((int) dq.size() > 2 && dq.back().outside(intersect(dq[0], dq[1]))) dq.pop_front();

		if ((int) dq.size() < 3) return {};

		vector <pair <ld, ld> > result((int) dq.size());
		for (int i = 0; i < (int) dq.size(); i++)
		{
			int nxt = (i + 1) % (int) dq.size();
			if (vecs(mp(dq[i].A, dq[i].B), mp(dq[nxt].A, dq[nxt].B)) <= 0) return {};
			result[i] = intersect(dq[i], dq[nxt]);
		}
		return result;
	}
}