---
marp: true
size: 4:3
math: mathjax
---

# Segment tree

## 用于维护幺半群区间信息的数据结构

## 功能

### $O(logN)$复杂度下实现

- 区间查询 (query)
- 区间修改 (modify)
- 支持任何符合幺半群性质的代数系统，如求和，乘法，异或，gcd，max等运算

---

## 线段树的建树 ， 我们给出一个求和代数系统下的例子

线段树将每个长度不为 1 的区间划分成左右两个区间递归求解，把整个线段划分为一个树形结构，通过合并左右两区间信息来求得该区间的信息。这种数据结构可以方便的进行大部分的区间操作。

---

![h:400 w:1000](/images/870675c4379d6654720449258cd30b1a75f0b63d6be6c0c8ef0a0a2087149505.png)  

**红色字体**表明的是该结点管辖的区间，黑色字体表示该区间维护的信息

左子结点表示: $[l,(l+r)/2]$  右子结点表示: $[(l+r)/2+1,r]$

特别的 , 当$l==r$则该结点为叶节点

---

## 性质

由线段树为CBT,即完全二叉树，存树除了可持久化线段树,一般使用堆式存储

- 树高 $\lceil logN \rceil$
- 总结点数 $2^{\lceil logn \rceil+1}$

可得总空间开销约为$4N$,空间复杂度$O(N)$

---

## 区间查询 $O(logN)$

假设查询区间为 $[l,r]$ ,从线段树根节点开始查询

### 有  

- 假设查询区间覆盖左结点，查询左子树

- 假设查询区间覆盖右结点，查询右子树

- 每次区间查询可覆盖未查询子区间 $\lfloor n/2 \rfloor$

---

## lazy tag 加速区间修改 $O(logN)$

lazy tag 的核心思想便是延迟修改，若查询过程某节点被完全覆盖，则将其修改并打上tag
**并停止向下搜索**

下面我们给出一个例子，代数系统依然为求和

---

![h:400 w:700](/images/fc1c4d8d60af4505ec2d8c751ec9632115ae5d6572130a0cc55b8ddf4c2fe81b.png)  

### 假设我们对区间 $[3,5]$ 加上5

- 从根节点向下搜索过程有 $[3,3]$ , $[4,5]$ 被查询区间**完全覆盖**
- 我们给这两个结点打上tag，修改区间后停止向下搜索

---

![h:400 w:700](/images/9d602ef3f3dc1091586296d80fdd00e4865439e9237f9635229fbaf5d985cc10.png)  

### 修改完成后假设我们需要查询区间 $[4,4]上的信息$

---

#### 我们沿着根节点路径向下搜索过程中，不断将lazy tag 下放，并将搜索路径上的tag清空，以此不断维护需要的信息

![picture 3](/images/e51340cdc04ce360a274563125806659e738ce7c8b354099198b748fc6a08be8.png)  

---

## 可持久化线段树

主席树是线段树可持久化数据结构。

即每次做 $modify$ 操作时，保留历史版本

---

## 实现方式

- 可以观察到每次做区间修改更改的结点数量约为树高

- 每次进行修改，额外增加 $logN$ 个修改过后的新结点

- 由于主席树是动态开点的思想，所以存树使用链式存储。

---

![picture 4](/images/093d693bfdbb9ddb0bd14010b0d7641d9e1f10b48e27c4eaf9c3c265a2e36408.png)  

图中红色结点为区间修改时需要更改的结点

---

## 例题

- 权值线段树加速动态规划状态转移

- 线段树结合dfs序维护子树和，实现子树修改

- 可持久化线段树实现查询区间第k大值 
  
---

## Optimal Partition

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

---

## A Growing Tree

![picture 0](/images/943b60618cb39a08de70346bd828a5a7abfa09c88717902a7ea97d0860ee45e8.png)  

---

## 树的dfs序存在一个性质 ， 即以某个结点为根子树可由dfs序上的区间来表示

![h:400 w:600](/images/26e1a38627ecdd1a52f7c17e7c75dbb011d90fb678016ebe541588bd0316d885.png)  

dfs 序有 1 2 3 4 5 6 7 8 9
其中4号结点子树可由区间 $[6,9]$表示

---

### 故我们可以将题目给定操作离线下来，将所有操作完成后的树存下，之后得到该树dfs序,注意到

- 第一种操作，即插入新结点，等价于将新结点为根的子树赋值为0
- 第二种操作，即给某个结点为根的子树加上数值，等价于给dfs序某一个区间加上一个数

以上操作可以由带lazy tag的线段树支持

---

### 求dfs序

其中 $rid$ 表示题目给定结点编号对应dfs序编号
$R$ 表示某一个结点表示的子树区间右端点编号

```cpp
void dfs(int u, int fa, vector<int>& rid, vector<int>& R,int &idx) {
    rid[u] = idx;
    for (int i : g[u]) {
        if (i == fa) continue;
        idx++;
        dfs(i,u,rid,R,idx);
    }
    R[rid[u]] = idx;
}
```

---

### 线段树的两个操作函数

```cpp
//线段树递归回溯时由子节点计算父节点
void pushup(int u) {
    tr[u] = tr[u << 1] + tr[u << 1 | 1];
}
//lazy tag 下传
void pushdown(int u, int l, int r) {
    int mid = l + r >> 1;
    tr[u << 1] += lz[u] * (mid - l + 1);
    tr[u << 1 | 1] += lz[u] * (r - mid);
    lz[u << 1] += lz[u], lz[u << 1 | 1] += lz[u];
    lz[u] = 0;
}
```

---

### 区间修改

```cpp
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
```

---

### 查询操作

```cpp
ll query(int u, int l, int r, int ql, int qr) {
    if (ql <= l && qr >= r) return tr[u];
 
    int mid = l + r >> 1;
    pushdown(u, l, r);
    ll res = 0;
    if (ql <= mid) res += query(u << 1, l, mid, ql, qr);
    if (qr > mid) res += query(u << 1 | 1, mid + 1, r, ql, qr);
    return res;
}
```

---

```cpp
void solve() {
    .......
    //按照题目给定操作执行
    for (int i = 1;i <= q;i++) {
        const vector<int>& I = op[i];
        //置零
        if (I[0] == 1) {
            int node = rid[I[1]];
            modify(1, 1, n, node, node, -query(1, 1, n, node, node));
        }
        //区间加法
        else {
            int nl = rid[I[1]], nr = R[nl];
            modify(1, 1, n, nl, nr, I[2]);
        }
    }
    //单点查询
    for (int i = 1;i <= n;i++) {
        int node = rid[i];
        cout << query(1, 1, n, node, node) << " ";
    }
    cout << '\n';
}
```

---

## 第k小数

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
- 结合二分思想，便可以 $O(logn)$ 得到区间第k小值

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

### 查询答案，返回第k小值在离散化数组中的下标

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
