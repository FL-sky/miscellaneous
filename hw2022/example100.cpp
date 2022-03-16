#include <bits/stdc++.h>
using namespace std;

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
    const int inf = 1000000000;
    const static int N = 35 + 135 + 10, M = 35 * 135 + 35 + 135 + 10;
    struct Edge
    {
        int v, f, nxt;
        int raw;
    };
    int src, sink;
    int g[N + 10], nume;
    Edge e[M << 1];

    void addedge(int u, int v, int c)
    {
        e[++nume].v = v;
        e[nume].f = c;
        e[nume].raw = c;
        e[nume].nxt = g[u];
        g[u] = nume;

        e[++nume].v = u;
        e[nume].f = 0;
        e[nume].raw = 0;
        e[nume].nxt = g[v];
        g[v] = nume;
    }

    void init()
    {
        memset(g, 0, sizeof(g));
        nume = 1;
        //
    }

    queue<int> que;
    bool vis[N + 10];
    int dist[N + 10];

    void bfs()
    {
        memset(dist, 0, sizeof(dist));
        while (!que.empty())
            que.pop();
        vis[src] = true;
        que.push(src);
        while (!que.empty())
        {
            int u = que.front();
            que.pop();
            for (int i = g[u]; i; i = e[i].nxt)
            {
                if (e[i].f && !vis[e[i].v])
                {
                    que.push(e[i].v);
                    dist[e[i].v] = dist[u] + 1;
                    vis[e[i].v] = true;
                }
            }
        }
    }

    int dfs(int u, int delta)
    {
        if (u == sink)
            return delta;
        int ret = 0;
        for (int i = g[u]; i; i = e[i].nxt)
        {
            if (e[i].f && dist[e[i].v] == dist[u] + 1)
            {
                int dd = dfs(e[i].v, min(e[i].f, delta));
                e[i].f -= dd;
                e[i ^ 1].f += dd;
                delta -= dd;
                ret += dd;
            }
        }
        return ret;
    }

    int maxflow(int sum)
    {
        int ret = 0;
        while (sum != ret)
        {
            memset(vis, 0, sizeof(vis));
            bfs();
            if (!vis[sink])
                return ret;
            ret += dfs(src, inf);
        }
        return ret;
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
        mcf.src = 171;
        mcf.sink = mcf.src + 1;
        mcf.init();
        for (int i = 0; i < server.size(); i++)
        {
            for (int j = 0; j < client.size(); j++)
            {
                if (can[i][j])
                {
                    int mx = min(site_bandwidth[i], demand[it][j]);
                    int tp = 1;

                    mcf.addedge(j, i + client.size(), mx);
                    // mcf.addedge(i + client.size(), j, 0);
                }
            }
        }
        for (int j = 0; j < client.size(); j++)
        {
            mcf.addedge(mcf.src, j, demand[it][j]);
            // mcf.addedge(j, mcf.src, 0);
        }
        for (int i = 0; i < server.size(); i++)
        {
            mcf.addedge(i + client.size(), mcf.sink, site_bandwidth[i]);
            // mcf.addedge(mcf.sink, i + client.size(), 0);
        }
        int sum = accumulate(demand[it].begin(), demand[it].end(), 0);
        int mf = mcf.maxflow(sum);
        // printf("it=%d\tmf=%d\n", it, mf);
        for (int c = 0; c < client.size(); c++)
        {
            outFile << client[c] << ":";
            int cnt[135 + 10];
            memset(cnt, 0, sizeof(cnt));
            for (int i = mcf.g[c]; i; i = mcf.e[i].nxt)
            {
                if (mcf.e[i].raw > 0 && mcf.e[i].f >= 0 && mcf.e[i].v >= client.size() && mcf.e[i].v < mcf.src)
                {
                    cnt[mcf.e[i].v - client.size()] += mcf.e[i].raw - mcf.e[i].f;
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