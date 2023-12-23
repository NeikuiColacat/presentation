---
marp: true
size: 4:3
math: mathjax
---

# Optimal Partition

![picture 0](/images/51c0552d94a3c2c98eb971169d4e1cbe86a6ea6a8002e7a03771284eaada3a7c.png)  

---

## 注意到 ， 假设最优解从左到右划分若干次，之前的若干次划分不会影响到当前划分的最优决策,故考虑DP

---

### 定义 $pre_i为1\sim i的前缀和$

### 设 $dp_i表示仅考虑前i个整数时，最优的答案，有$

$$
dp_i=max\left\{
\begin{aligned}
dp_j+i-j, \forall j<i且pre_j<pre_i\\
dp_j-(i-j),\forall j<i且pre_j>pre_i\\
dp_j,\forall j<i且pre_j=pre_i
\end{aligned}
\right.
$$

### 变换后有

$$
dp_i=max\left\{
\begin{aligned}
(dp_j-j)+i, \forall j<i且pre_j<pre_i\\
(dp_j+j)-i,\forall j<i且pre_j>pre_i\\
dp_j,\forall j<i且pre_j=pre_i
\end{aligned}
\right.
$$

固状态转移时，可将 $i$ 看作常数，在三种情况对应集合中求 $max$ ，即可完成状态转移

---

## 所以我们需要一个数据结构，支持如下操作

- 在给定值域集合中求max
- 支持在线操作，即每次完成状态转移时，将 $dp_i$ 插入到集合中，以支持之后的状态转移更新

故可以将 $n$ 个前缀和值进行离散化后 , 在离散化后的值域上建立一棵权值线段树

$O(logn)$ 每次的 $query$ 得到

$$
\left\{
\begin{aligned}
max(dp_j-j), \forall j<i且pre_j<pre_i\\
max(dp_j+j),\forall j<i且pre_j>pre_i\\
\end{aligned}
\right.
$$

再使用一个 $map$ 维护 $max(dp_j),\forall j<i且pre_j=pre_i$ 

---

## 对线段树结点

$l,r表示当前结点表示的区间做右端点$
$P表示max(dp_j-j),\forall l<=pre_j<=r$
$N表示max(dp_j+j),\forall l<=pre_j<=r$

```cpp
//线段树结点定义
struct node {
    int l, r, P,N;
};  
vector<node> tr; // 线段树数组
vector<ll> p; // 前缀和数组
vector<ll> f; // 离散化数组
```

---

---

## 对离散化数组查找函数，以数组下标取代实际值

```cpp
int find(ll x) {
    return lower_bound(f.begin() , f.end(), x) - f.begin();
}
```

---

## 线段树若干操作函数

```cpp
//由子节点计算父节点max
void pushup(int u) {
    tr[u].P = max(tr[u << 1].P, tr[u << 1 | 1].P);
    tr[u].N = max(tr[u << 1].N, tr[u << 1 | 1].N);
}
```

```cpp
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
```

---

## 区间查询

```cpp
int query(int u, int l, int r, int M) {
    if (l > r)return -INF;
    if (tr[u].l >= l && tr[u].r <= r) return M ? tr[u].P : tr[u].N;
    int res = -INF;
    int mid = tr[u].l + tr[u].r >> 1;
    if (l <= mid) res = query(u << 1, l, r,M);
    if (r > mid) res = max(res, query(u << 1 | 1, l, r,M));
    return res;
}

```

---

## 单点修改

```cpp
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
```

---

```cpp
void solve() {
    cin >> n;
    init();
    for (int i = 1;i <= n;++i) {
        cin >> p[i];
        p[i] += p[i - 1];
        f[i] = p[i];
    }

    //离散化数组
    sort(f.begin() , f.end());
    f.resize(unique(f.begin(), f.end()) - f.begin());
    int m = f.size() - 1;
    build(1, 0, m);
    
    vector<int> dp(n + 1, -1e9);
    map<ll, int> same;
    if (p[1] == 0) same[0] = 0;
    else if (p[1] > 0) same[p[1]] = 1;
    else same[p[1]] = -1;
    for (int i = 1;i <= n;i++) {
        
        //状态计算
        if(same.count(p[i]))dp[i] = same[p[i]];
        dp[i] = max(dp[i], query(1, 0, find(p[i])-1, 1)  + i );
        dp[i] = max(dp[i], query(1, find(p[i]) + 1, m, 0) - i);

        //完成计算，向树中插入新数值
        modify(1, find(p[i]), dp[i] - i, dp[i] + i);
        if (same.count(p[i])) same[p[i]] = max(same[p[i]], dp[i]);
        else same[p[i]] = dp[i];
    }

    cout << dp[n] << endl;
}
```
