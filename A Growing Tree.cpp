#include <bits/stdc++.h>
using namespace std;

#define x first
#define y second

using ll = long long;
using P = pair<int, int>;
const int N = 1e6 + 10;

/*G存储图的邻接表,op存储每个节点方式*/
vector<vector<int>> g(N), op(N, vector<int>(3));
/*tr -> 存节点值的线段树,lz懒标记*/
vector<ll> tr(4 * N), lz(4 * N);
/*原图到线段树映射.rid -> 区间左,R -> 区间右*/
vector<int> R(N) , rid(N);

/*线段树初始化*/
void build(int u, int l, int r) {
    tr[u] = 0, lz[u] = 0;
    if (l != r) {
        int mid = l + r >> 1;
        build(u << 1, l, mid); 
        build(u << 1 | 1, mid + 1, r);
    }
}

/*更新父节点的值*/
void pushup(int u) {
    tr[u] = tr[u << 1] + tr[u << 1 | 1];
}

/*将父节点的懒标记传递给子节点*/
void pushdown(int u, int l, int r) {
    int mid = l + r >> 1;
    tr[u << 1] += lz[u] * (mid - l + 1);
    tr[u << 1 | 1] += lz[u] * (r - mid);
    lz[u << 1] += lz[u], lz[u << 1 | 1] += lz[u];
    lz[u] = 0;
}

/*线段树区间修改*/
void modify(int u, int l, int r, int ql, int qr, ll val) {
    if (l >= ql && r <= qr) {
        tr[u] += val * (r - l + 1);
        lz[u] += val;
        return;
    }

    int mid = l + r >> 1;
    pushdown(u, l, r);
    if (ql <= mid) modify(u << 1, l, mid, ql, qr, val);
    if (qr > mid) modify(u << 1 | 1, mid + 1, r, ql, qr, val);
    pushup(u);
}

/*区间查询节点和*/
ll query(int u, int l, int r, int ql, int qr) {
    if (ql <= l && qr >= r) return tr[u];

    int mid = l + r >> 1;
    pushdown(u, l, r);
    ll res = 0;
    if (ql <= mid) res += query(u << 1, l, mid, ql, qr);
    if (qr > mid) res += query(u << 1 | 1, mid + 1, r, ql, qr);
    return res;
}

/*深度优先搜索,遍历图节点,计算节点编号*/ 
void dfs(int u, int fa, vector<int>& rid, vector<int>& R,int &idx) {
    rid[u] = idx;
    for (int i : g[u]) {
        if (i == fa) continue;
        idx++;
        dfs(i,u,rid,R,idx);
    }
    R[rid[u]] = idx;
}
void solve() {
    int q;cin >> q;		//操作数 
    for (int i = 1;i <= q;i++)g[i].clear();

    int n = 1;
    for (int i = 1;i <= q;i++) {
        int t;cin >> t;
        if (t == 1) {	//操作1.添加新节点sz+1到节点v,sz表示树大小 
            n++;
            int v;cin >> v;
            g[v].push_back(n);			//放节点,名称为 n(sz+1) 
            op[i][0] = t, op[i][1] = n;//操作码 
        }
        else {			//操作2.给当前节点v及其子树节点都添加x 
            int v, x;cin >> v >> x;
            op[i][0] = t, op[i][1] = v, op[i][2] = x;
        }
    }

    build(1, 1, n);
    
    int idx = 1;
    dfs(1, -1,rid,R,idx);	//使用dfs将给定树映射到线段树

    for (int i = 1;i <= q;i++) {
        const vector<int>& I = op[i];
        if (I[0] == 1) {
            int node = rid[I[1]];
            modify(1, 1, n, node, node, -query(1, 1, n, node, node));
        }
        else {
            int nl = rid[I[1]], nr = R[nl];
            modify(1, 1, n, nl, nr, I[2]);
        }
    }

    for (int i = 1;i <= n;i++) {
        int node = rid[i];
        cout << query(1, 1, n, node, node) << " ";
    }
    cout << '\n';
}
signed main() {
    ios::sync_with_stdio(0);
    cin.tie(0);cout.tie(0);
    
    int t;cin >> t;while (t--)	//样例数 
        solve();

    return 0;
}
