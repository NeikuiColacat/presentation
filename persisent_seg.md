---
marp: true
size: 4:3
math: mathjax
---
# 第k小数

![picture 0](/images/2341c1822305d3441e9f9aedaf17523f0387386351316f7766a4f1a6be5cceac.png)  

---

## 在给定数组所能表示的值域上建立线段树 ， 有

- 由于数据范围 $[1,10^9]$ , 可先将值域离散化
- 每个结点存该结点所表示值域包含数组元素个数,记为 $cnt$

### 查询第k大数 ，由根节点起线段树上二分搜索

- 若 $cnt_{左子节点} >= k$ , 搜索左子树
- 若 $cnt_{右子节点} <= k$ , 搜索右子树 ， 令 $k=k-cnt$
- 不断搜索直到到达叶节点，得到 $ans$

---

### 当查询区间变化时，可使用主席树建立$n$棵线段树

第 $i$ 棵线段树由第 $i-1$ 棵线段树上插入元素 $a_i$ 得到

令 $seg_i 表示第i棵线段树 , 查询区间为[l,r]$ , 利用前缀和有

- $区间[l,r]的cnt值=cnt_{seg_r}-cnt_{seg_{l-1}}$

### 故我们可以求得任意利用权值线段树与主席树做到

- 查询任意区间上，某一个值域存在多少元素
- 结合二分思想，便可以 $O(logn)$ 得到区间第k大值

---

```cpp
struct node {
    int l, r;
    int cnt;
};
vector<int> a, root, f;
vector<node> tr;
int idx;
```

- $node$ 结点表示线段树结点
  - $l表示左子节点地址,r表示右子节点地址$
  - $cnt 与以上定义一致，即值域包含的元素数量$
- $a为题目给定序列$
- $root为每棵线段树根节点地址$
- $f为离散化数组，用于表示值域的$
- $idx表示尚未用到的空间地址$

---

### 初始化 ， 读入数据并分配空间 ， 同时对 $f$ 离散化数组初始化  

```cpp
void init(int n) {
    root.resize(n+1), a.resize(n) ,tr.resize(n << 5);
    for (int i = 0;i < n;i++) cin >> a[i];

    f = a;
    sort(f.begin(), f.end());//排序
    f.resize(unique(f.begin(), f.end()) - f.begin());//去重
}
```

$以数值在f的下标代替实际值，实现离散化值域上建立线段树$

---

### 二分搜索给定数值在离散化数组中的下标

```cpp
int find(int val) {
    int l = 0, r = f.size() - 1;
    while (l < r) {
        int mid = l + r >> 1;
        if (f[mid] >= val) r = mid;
        else l = mid + 1;
    }
    return l;
}
```

---

### 建树

```cpp
int build(int l, int r) {
    int u = ++idx; // 分配空间
    tr[u].cnt = tr[u].l = tr[u].r = 0;
    if (l == r) return u; //递归建树直到叶结点

    int mid = l + r >> 1;
    tr[u].l = build(l, mid);
    tr[u].r = build(mid + 1, r);
    return u;
}
```

---

### 根据之前的信息建立一棵新的线段树 ， 插入 $a_i$

```cpp
// pos即在值域上插入的位置 , u为上一棵线段树表示区间[l,r]结点地址
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
```

---

### 查询答案，返回第k大值在离散化数组中的下标

```cpp
int query(int u, int v, int l, int r, int k) {
    if (l == r) return r;

    int mid = l + r >> 1;
    int cnt = tr[tr[v].l].cnt - tr[tr[u].l].cnt;
    if (k <= cnt) return query(tr[u].l, tr[v].l, l, mid, k);
    else return query(tr[u].r, tr[v].r, mid + 1, r, k - cnt);
}
```

---

### 调用以上函数 ，建立主席树 ， 回答m个询问

```cpp
void solve() {
    int n, m;cin >> n >> m;
    init(n);
    root[0] = build(0, f.size() - 1);
    for (int i = 1;i <= n;i++) 
    root[i] = insert(0, f.size() - 1, root[i - 1], find(a[i - 1]));

    while (m--) {
        int li, ri, k;cin >> li >> ri >> k;
        int ans = query(root[li - 1], root[ri], 0, f.size() - 1, k);
        cout << f[ans] << endl;
    }
}
```
