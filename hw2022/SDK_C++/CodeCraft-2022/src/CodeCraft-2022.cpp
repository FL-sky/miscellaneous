#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
int qos_constraint = 1e9;
vector<vector<int>> demand;
// vector<vector<int>> qos;
vector<vector<bool>> can;
vector<int> site_bandwidth;
vector<string> client, server;
void readData()
{
    // 读文件
    const string prefix = "/data/";
    // const string prefix = "./littleData/";
    string fileName;
    string inFileName;

    //读取配置 qos_constraint
    {
        fileName = "config.ini";
        inFileName = prefix + fileName;

        ifstream inFile(inFileName, ios::in);
        string lineStr;
        // vector<vector<string>> strArray;
        while (getline(inFile, lineStr))
        {
            if (lineStr.substr(0, 14) == "qos_constraint")
            {
                qos_constraint = atoi(lineStr.substr(15).c_str());
                break;
            }
        }
        inFile.close();
    }
    //读取配置 demand
    {
        fileName = "demand.csv";
        inFileName = prefix + fileName;

        ifstream inFile(inFileName, ios::in);
        string lineStr;
        // vector<vector<string>> strArray;
        getline(inFile, lineStr);
        {
            stringstream ss(lineStr);
            string str;
            vector<string> stringArr;
            // 按照逗号分隔
            getline(ss, str, ',');
            while (getline(ss, str, ','))
            {
                client.push_back(str);
            }
        }
        while (getline(inFile, lineStr))
        {
            // 存成二维表结构
            stringstream ss(lineStr);
            string str;
            vector<int> intArr;
            // 按照逗号分隔
            getline(ss, str, ',');
            while (getline(ss, str, ','))
            {
                intArr.push_back(atoi(str.c_str()));
            }
            demand.push_back(intArr);
        }
        inFile.close();
    }
    // 读取 qos.csv
    {
        fileName = "qos.csv";
        inFileName = prefix + fileName;

        ifstream inFile(inFileName, ios::in);
        string lineStr;
        // vector<vector<string>> strArray;
        getline(inFile, lineStr);
        while (getline(inFile, lineStr))
        {
            // 存成二维表结构
            stringstream ss(lineStr);
            string str;
            vector<bool> boolArr;
            // 按照逗号分隔
            getline(ss, str, ',');
            while (getline(ss, str, ','))
            {
                boolArr.push_back(atoi(str.c_str()) < qos_constraint);
            }
            can.push_back(boolArr);
        }
        inFile.close();
    }
    // 读取 site_bandwidth.csv
    {
        fileName = "site_bandwidth.csv";
        inFileName = prefix + fileName;

        ifstream inFile(inFileName, ios::in);
        string lineStr;
        getline(inFile, lineStr);
        while (getline(inFile, lineStr))
        {
            // 存成二维表结构
            stringstream ss(lineStr);
            string str;
            // 按照逗号分隔
            getline(ss, str, ',');
            server.push_back(str);
            while (getline(ss, str, ','))
            {
                site_bandwidth.push_back(atoi(str.c_str()));
            }
        }
        inFile.close();
    }
}
class micmxf
{
public:
    const static int MAXN = 170 + 10;
    const static int MAXM = 94500 + 10;
    int s, t, cnt = 1;
    int maxflow, mincost;
    int dis[MAXN], head[MAXN], incf[MAXN], pre[MAXN]; // dis表示最短路，incf表示当前增广路上最小流量，pre表示前驱
    bool vis[MAXN];
    struct EDGE
    {
        int next, to, dis, flow;
        int raw;
    } ee[MAXM << 1];
    void add(int from, int to, int flow, int dis)
    {
        ee[++cnt].next = head[from];
        ee[cnt].to = to;
        ee[cnt].dis = dis;
        ee[cnt].flow = flow;
        ee[cnt].raw = flow;
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
    void init()
    {
        cnt = 1;
        memset(head, 0, sizeof(head));
        mincost = maxflow = 0;
    }
} mcf;
void solv()
{
    // 写文件
    ofstream outFile;
    const string ofileName = "/output/solution.txt";
    outFile.open(ofileName, ios::out); // 打开模式可省略
    // outFile << "name" << ',' << "age" << ',' << "hobby" << endl;
    for (int it = 0; it < demand.size(); it++)
    {
        mcf.s = 171;
        mcf.t = mcf.s + 1;
        mcf.init();
        for (int i = 0; i < server.size(); i++)
        {
            for (int j = 0; j < client.size(); j++)
            {
                if (can[i][j])
                {
                    int mx = min(site_bandwidth[i], demand[it][j]);
                    int tp = 1;
                    while (mx > 0)
                    {
                        mcf.add(j, i + client.size(), tp, tp);
                        mcf.add(i + client.size(), j, 0, -tp);
                        mx -= tp;
                        tp = tp << 1;
                    }
                }
            }
        }
        for (int j = 0; j < client.size(); j++)
        {
            mcf.add(mcf.s, j, demand[it][j], 0);
            mcf.add(j, mcf.s, 0, 0);
        }
        for (int i = 0; i < server.size(); i++)
        {
            mcf.add(i + client.size(), mcf.t, site_bandwidth[i], 0);
            mcf.add(mcf.t, i + client.size(), 0, 0);
        }
        mcf.MCMF();
        // printf("%d %d\t\t", mcf.maxflow, mcf.mincost);
        for (int c = 0; c < client.size(); c++)
        {
            outFile << client[c] << ":";
            int cnt[135 + 10];
            memset(cnt, 0, sizeof(cnt));
            for (int i = mcf.head[c]; i; i = mcf.ee[i].next)
            {
                if (mcf.ee[i].flow >= 0 && mcf.ee[i].to >= client.size() && mcf.ee[i].to < mcf.s)
                {
                    // cout << "<" << server[mcf.ee[i].to - client.size()] << "," << mcf.ee[i].flow << ">";
                    cnt[mcf.ee[i].to - client.size()] += mcf.ee[i].raw - mcf.ee[i].flow;
                }
            }
            bool prt = false;
            for (int i = 0; i < 135 + 10; i++)
            {
                if (cnt[i] > 0)
                {
                    if (prt)
                        outFile << ",";
                    outFile << "<" << server[i] << "," << cnt[i] << ">";
                    prt = true;
                }
            }
            outFile << endl;
        }
    }
    outFile.close();
}
int main()
{
    readData();
    solv();
    return 0;
}