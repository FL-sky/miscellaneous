#include <bits/stdc++.h>
using namespace std;
class micmxf
{
public:
    const static int MAXN = 170 + 10;
    const static int MAXM = 94500 + 10;
    int n, m, s, t, cnt = 1;
    int maxflow, mincost;
    int dis[MAXN], head[MAXN], incf[MAXN], pre[MAXN]; // dis表示最短路，incf表示当前增广路上最小流量，pre表示前驱
    bool vis[MAXN];
    struct EDGE
    {
        int next, to, dis, flow;
    } ee[MAXM << 1];
    void add(int from, int to, int flow, int dis)
    {
        ee[++cnt].next = head[from];
        ee[cnt].to = to;
        ee[cnt].dis = dis;
        ee[cnt].flow = flow;
        head[from] = cnt;
    }
    inline bool spfa()
    {
        queue<int> q;
        memset(dis, 0x3f, sizeof(dis));
        memset(vis, 0, sizeof(vis));
        q.push(s);
        dis[s] = 0;
        vis[s] = 1;
        incf[s] = 1 << 30;
        while (!q.empty())
        {
            int u = q.front();
            vis[u] = 0;
            q.pop();
            for (register int i = head[u]; i; i = ee[i].next)
            {
                if (!ee[i].flow)
                    continue; //没有剩余流量
                int v = ee[i].to;
                if (dis[v] > dis[u] + ee[i].dis)
                {
                    dis[v] = dis[u] + ee[i].dis;
                    incf[v] = min(incf[u], ee[i].flow); //更新incf
                    pre[v] = i;
                    if (!vis[v])
                        vis[v] = 1, q.push(v);
                }
            }
        }
        if (dis[t] == 0x3f3f3f3f)
            return 0;
        return 1;
    }
    void MCMF()
    {
        while (spfa())
        { //如果有增广路
            int x = t;
            maxflow += incf[t];
            mincost += dis[t] * incf[t];
            int i;
            while (x != s)
            { //遍历这条增广路，正向边减流反向边加流
                i = pre[x];
                ee[i].flow -= incf[t];
                ee[i ^ 1].flow += incf[t];
                x = ee[i ^ 1].to;
            }
        }
    }
    bool init(int a, int b, int c, int d)
    {
        cnt = 1;
        memset(head, 0, sizeof(head));
        mincost = maxflow = 0;
        n = a, m = b, s = c, t = d;
        return a > 0;
    }
} mcf;
int main()
{
    int n, m, s, t;
    while (scanf("%d%d%d%d", &n, &m, &s, &t), mcf.init(n, m, s, t))
    {

        int u, v, w, x;
        for (int i = 1; i <= m; ++i)
        {
            scanf("%d%d%d%d", &u, &v, &w, &x);
            mcf.add(u, v, w, x);
            mcf.add(v, u, 0, -x); //反向边费用为-f[i]
        }
        mcf.MCMF(); //最小费用最大流
        printf("%d %d\n", mcf.maxflow, mcf.mincost);
    }
    return 0;
}