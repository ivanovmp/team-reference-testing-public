namespace SuffixTree {
    struct Position {
        int vertex, shift; 	
    };

    const int ALPHABET = 26;
    
    class SuffixTree {
    public: 	 	
        SuffixTree(const vector <int> &s) {
            str = s;
            int n = (int) str.size();
            str_bounds.resize(2 * n, mp(0, 0)), suflink.resize(2 * n, -1), parent.resize(2 * n, mp(-1, -1));
            for (int i = 0; i < ALPHABET; i++) next_step[i].resize(2 * n, -1);
            
            quant++;
            int root = 0;
            str_bounds[root] = mp(0, -1);
            suflink[root] = root;
            Position cur = {root, 0};

            for (int i = 0; i < n; i++) {
                int symb = str[i];
                bool do_move = true;
                Position old = {-1, -1};
                while (!can_move(cur, symb)) {
                    if (cur.vertex == root) do_move = false;
                    cur = create_edge(cur, symb, i, n - 1);
                    if (old.vertex != -1) suflink[old.vertex] = cur.vertex;
                    old = cur;
                    cur = jump_suflink(cur);
                }
                if (old.vertex != -1) suflink[old.vertex] = cur.vertex;
                if (do_move) cur = move_next(cur, symb);
            }
        }

        bool can_move(Position cur, int symb) const {
            if (cur.shift == 0) return next_step[symb][cur.vertex] != -1;
            else {
                int next_symb = str[str_bounds[cur.vertex].x + cur.shift];
                return next_symb == symb;
            }
        }

        Position create_edge(Position cur, int symb, int lef, int rig) {
            if (cur.shift != 0) {
                quant++;
                int mid_vertex = quant - 1;
                
                parent[mid_vertex] = parent[cur.vertex];
                next_step[parent[mid_vertex].x][parent[mid_vertex].y] = mid_vertex;
                int next_symb = str[str_bounds[cur.vertex].x + cur.shift];
                parent[cur.vertex] = mp(next_symb, mid_vertex);
                next_step[next_symb][mid_vertex] = cur.vertex;
                str_bounds[mid_vertex] = mp(str_bounds[cur.vertex].x, str_bounds[cur.vertex].x + cur.shift - 1);
                str_bounds[cur.vertex] = mp(str_bounds[cur.vertex].x + cur.shift, str_bounds[cur.vertex].y);

                cur = {mid_vertex, 0};
            }
            
            quant++;
            int new_vertex = quant - 1;
            next_step[symb][cur.vertex] = new_vertex;
            parent[new_vertex] = mp(symb, cur.vertex);
            str_bounds[new_vertex] = mp(lef, rig);
            return cur;
        }

        Position jump_suflink(Position cur) const {
            int lef = 0, rig = 0;
            if (cur.shift == 0) {
                if (suflink[cur.vertex] != -1) return {suflink[cur.vertex], 0};
                lef = str_bounds[cur.vertex].x, rig = str_bounds[cur.vertex].y;
            } else {
                lef = str_bounds[cur.vertex].x, rig = str_bounds[cur.vertex].x + cur.shift - 1; 	
            }
            if (parent[cur.vertex].y == 0) lef++;
            Position new_cur = {suflink[parent[cur.vertex].y], 0};
            int tek = lef;
            while (tek <= rig) {
                int next_vert = next_step[str[tek]][new_cur.vertex];
                if (str_bounds[next_vert].y - str_bounds[next_vert].x + 1 <= rig - tek + 1) {
                    tek += str_bounds[next_vert].y - str_bounds[next_vert].x + 1;
                    new_cur = {next_vert, 0};
                } else {
                    new_cur = {next_vert, rig - tek + 1};
                    tek = rig + 1;
                }
            }
            return new_cur;
        }

        Position move_next(Position cur, int symb) const {
            if (cur.shift == 0) {
                int next_vert = next_step[symb][cur.vertex];
                if (str_bounds[next_vert].y == str_bounds[next_vert].x) return {next_vert, 0};
                else return {next_vert, 1};
            } else {
                if (str_bounds[cur.vertex].y - str_bounds[cur.vertex].x == cur.shift) return {cur.vertex, 0};
                else return {cur.vertex, cur.shift + 1};
            }
        }

    private:
        int quant = 0;
        vector <int> str;
        vector <pair <int, int> > str_bounds, parent;
        vector <int> next_step[ALPHABET + 3], suflink;                   
    };
}