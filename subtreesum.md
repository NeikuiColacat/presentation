---
marp: true
size: 4:3
math: mathjax
---

# A Growing Tree

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
