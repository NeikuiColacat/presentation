#include <bits/stdc++.h>
using namespace std;
#define ll long long 
#define pii pair<int,int>
#define FL cout <<">>>"
#define pss pair<string,string> 
#define x first
#define y second
const int INF = 1e9;
int n;
struct node {
    int l, r, P,N;
};
vector<node> tr;
vector<ll> p;
vector<ll> f;
//初始化
void init() {
    tr.clear(), p.clear(), f.clear();
    tr.resize(4 * n + 1);
    p.resize(n + 1);
    f.resize(n + 1);
}
//离散化二分
int find(ll x) {
    return lower_bound(f.begin() , f.end(), x) - f.begin();
}
void pushup(int u) {
    tr[u].P = max(tr[u << 1].P, tr[u << 1 | 1].P);
    tr[u].N = max(tr[u << 1].N, tr[u << 1 | 1].N);
}
//建树
void build(int u, int l, int r) {
    tr[u] = { l , r , -INF , -INF };
    if (l == r && f[l] == 0) tr[u].P = tr[u].N = 0;
    if (l != r) {
        int mid = l + r >> 1;
        build(u << 1, l, mid), build(u << 1 | 1, mid + 1, r);
        pushup(u);
    }
}

//区间查询
int query(int u, int l, int r, int M) {
    if (l > r)return -INF;
    if (tr[u].l >= l && tr[u].r <= r) return M ? tr[u].P : tr[u].N;
    int res = -INF;
    int mid = tr[u].l + tr[u].r >> 1;
    if (l <= mid) res = query(u << 1, l, r,M);
    if (r > mid) res = max(res, query(u << 1 | 1, l, r,M));
    return res;
}
//单点修改
void modify(int u, int pos,int P , int N ) {
    if (tr[u].l == tr[u].r && tr[u].l == pos) {
        tr[u].P = max(tr[u].P, P);
        tr[u].N = max(tr[u].N, N);
        return;
    }
    int mid = tr[u].l + tr[u].r >> 1;
    if (pos <= mid) modify(u << 1, pos,P,N);
    else modify(u << 1 | 1, pos, P,N);
    pushup(u);
}
void solve() {
    cin >> n;
    init();
    for (int i = 1;i <= n;++i) {
        cin >> p[i];
        p[i] += p[i - 1];
        f[i] = p[i];
    }

    //离散化
    sort(f.begin(), f.end());
    f.resize(unique(f.begin(), f.end()) - f.begin());
    int m = f.size() - 1;
    build(1, 0, m);
    
    vector<int> dp(n + 1, -1e9);
    map<ll, int> same;
    if (p[1] == 0) same[0] = 0;
    else if (p[1] > 0) same[p[1]] = 1;
    else same[p[1]] = -1;
    for (int i = 1;i <= n;i++) {
        //状态转移
        if(same.count(p[i]))dp[i] = same[p[i]];
        dp[i] = max(dp[i], query(1, 0, find(p[i])-1, 1)  + i );
        dp[i] = max(dp[i], query(1, find(p[i]) + 1, m, 0) - i);

        modify(1, find(p[i]), dp[i] - i, dp[i] + i);
        if (same.count(p[i])) same[p[i]] = max(same[p[i]], dp[i]);
        else same[p[i]] = dp[i];
    }

    cout << dp[n] << endl;
}

signed main() {
    ios::sync_with_stdio(0);
    cin.tie(0), cout.tie(0);
    int t;cin >> t;while (t--)
        solve();
    return 0;
}