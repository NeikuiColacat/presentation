#include <bits/stdc++.h>
using namespace std;
struct node {
    int l, r;
    int cnt;
};
vector<int> a, root, f;
vector<node> tr;
int idx;

void init(int n) {
    root.resize(n+1), a.resize(n) ,tr.resize(n << 5);
    for (int i = 0;i < n;i++) cin >> a[i];

    f = a;
    sort(f.begin(), f.end());
    f.resize(unique(f.begin(), f.end()) - f.begin());
}

int find(int val) {
    int l = 0, r = f.size() - 1;
    while (l < r) {
        int mid = l + r >> 1;
        if (f[mid] >= val) r = mid;
        else l = mid + 1;
    }
    return l;
}

int build(int l, int r) {
    int u = ++idx;
    tr[u].cnt = tr[u].l = tr[u].r = 0;
    if (l == r) return u;

    int mid = l + r >> 1;
    tr[u].l = build(l, mid);
    tr[u].r = build(mid + 1, r);
    return u;
}

int insert(int l, int r, int u, int pos) {
    int v = ++idx;
    tr[v] = tr[u];

    if (l == r) {
        tr[v].cnt++;
        return v;
    }
    int mid = l + r >> 1;
    if (pos <= mid) tr[v].l = insert(l, mid, tr[u].l, pos);
    else tr[v].r = insert(mid + 1, r, tr[u].r, pos);
    tr[v].cnt = tr[tr[v].l].cnt + tr[tr[v].r].cnt;

    return v;
}

int query(int u, int v, int l, int r, int k) {
    if (l == r) return r;

    int mid = l + r >> 1;
    int cnt = tr[tr[v].l].cnt - tr[tr[u].l].cnt;
    if (k <= cnt) return query(tr[u].l, tr[v].l, l, mid, k);
    else return query(tr[u].r, tr[v].r, mid + 1, r, k - cnt);
}

void solve() {
    int n, m;cin >> n >> m;
    init(n);
    root[0] = build(0, f.size() - 1);
    for (int i = 1;i <= n;i++) root[i] = insert(0, f.size() - 1, root[i - 1], find(a[i - 1]));

    while (m--) {
        int li, ri, k;cin >> li >> ri >> k;
        int ans = query(root[li - 1], root[ri], 0, f.size() - 1, k);
        cout << f[ans] << endl;
    }
}
int main() {
    solve();
    return 0;
}