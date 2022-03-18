#include <stdio.h>
#include <algorithm>
#include <string.h>
#include <queue>
using namespace std;
#define INF 99999999
const int N = 202;
const int oo = 0x7fffffff;
struct list
{
    int x, h;
    bool friend operator<(const list &a, const list &b)
    {
        return a.h < b.h;
    }
} q;
struct push_relablel //最高标号预流推进算法，时间复杂度O(n*n*sqrt（m）)
{
    struct Edge
    {
        int v, res, nxt;
    } e[N << 1];
    int nume, g[N];
    int h[N];     //高度
    int ef[N];    //余流
    int n;        //点数
    int max_flow; //最大流
    // queue<int>que;//活动定点队列
    priority_queue<list> que;
    int u, v, p, i;
    int vis[N];
    void init(int x)
    {
        n = x;
        nume = -1;
        memset(g + 1, -1, sizeof(int) * n);
        memset(h + 1, 0, sizeof(int) * n);
        memset(ef + 1, 0, sizeof(int) * n);
    }
    void addedge(int u, int v, int w)
    {
        e[++nume].v = v;
        e[nume].res = w;
        e[nume].nxt = g[u];
        g[u] = nume; //--
        e[++nume].v = u;
        e[nume].res = 0;
        e[nume].nxt = g[v];
        g[v] = nume;
    }
    int ans(int s, int t)
    {
        max_flow = 0;
        h[s] = n;
        ef[s] = oo;
        ef[t] = -oo;
        while (!que.empty())
            que.pop();
        q.x = s;
        q.h = h[s];
        que.push(q);
        while (!que.empty())
        {
            q = que.top();
            que.pop();
            u = q.x;
            for (int i = g[u]; ~i; i = e[i].nxt)
            {
                v = e[i].v;
                if (e[i].res < ef[u])
                    p = e[i].res;
                else
                    p = ef[u];
                if (p > 0 && (u == s || h[u] == h[v] + 1))
                {
                    e[i].res -= p;
                    e[i ^ 1].res += p;
                    if (v == t)
                        max_flow += p;
                    ef[u] -= p;
                    ef[v] += p;
                    if (v != s && v != t)
                    {
                        q.x = v;
                        q.h = h[v];
                        que.push(q);
                    }
                }
            }
            if (u != s && u != t && ef[u] > 0)
            {
                h[u]++;
                q.x = u;
                q.h = h[u];
                que.push(q);
            }
        }
        return max_flow;
    }
} G;
int main()
{
    int m, n, u, v, w;
    while (~scanf("%d%d", &m, &n))
    {
        G.init(n);
        while (m--)
        {
            scanf("%d%d%d", &u, &v, &w);
            G.addedge(u, v, w);
        }
        printf("%d\n", G.ans(1, n));
    }
    return 0;
}
