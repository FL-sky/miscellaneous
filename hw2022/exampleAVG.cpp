#include <bits/stdc++.h>
using namespace std;
// string trim
// trim from start (in place)
static inline void ltrim(std::string &s)
{
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch)
                                    { return !std::isspace(ch); }));
}

// trim from end (in place)
static inline void rtrim(std::string &s)
{
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch)
                         { return !std::isspace(ch); })
                .base(),
            s.end());
}

// trim from both ends (in place)
static inline void trim(std::string &s)
{
    ltrim(s);
    rtrim(s);
}

//
int qos_constraint = -1;
vector<vector<int>> demand;
// vector<vector<int>> qos;
vector<vector<bool>> can;
vector<int> site_bandwidth;
vector<string> client, server;
map<string, int> clientID, serverID;
int icx = 0, isx = 0;

void getinvalid(int li)
{
    // printf("line=%d\n", li);
    // puts("");
    // char *str = "ABCDEFG";
    // for (int i = 0; i < 10000; i++)
    // {
    //     str[i] = str[i + 1];
    // }
}
void readData()
{
    // 读文件
    const string prefix = "/data/";
    // const string prefix = "./littleData/";
    // const string prefix = "./myData/";
    // const string prefix = "./test_data/";
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
            string tstr = "qos_constraint=";
            auto ps = lineStr.find(tstr);
            if (ps != string::npos)
            {
                qos_constraint = atoi(lineStr.substr(ps + tstr.length()).c_str());
                break;
            }
        }
        inFile.close();
        if (qos_constraint < 0)
        {
            getinvalid(__LINE__);
        }
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
            icx = 0;
            stringstream ss(lineStr);
            string str;
            vector<string> stringArr;
            // 按照逗号分隔
            getline(ss, str, ',');
            while (getline(ss, str, ','))
            {
                trim(str);
                client.push_back(str);
                clientID[str] = icx++;
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
                int xv = atoi(str.c_str());
                if (!(xv >= 0 && xv <= 550000))
                {
                    getinvalid(__LINE__);
                }
                intArr.push_back(xv);
            }
            if (intArr.size() > 0)
                demand.push_back(intArr);
        }
        inFile.close();
    }
    // 读取 site_bandwidth.csv
    {
        isx = 0;
        fileName = "site_bandwidth.csv";
        inFileName = prefix + fileName;

        ifstream inFile(inFileName, ios::in);
        string lineStr;
        getline(inFile, lineStr);
        struct po
        {
            string name;
            int val;
            po(string n = "", int v = 0)
            {
                name = n;
                val = v;
            }
            bool operator<(const po &b) const
            {
                return val < b.val;
            }
        };
        vector<po> vp;
        while (getline(inFile, lineStr))
        {
            // 存成二维表结构
            stringstream ss(lineStr);
            string str;
            // 按照逗号分隔
            getline(ss, str, ',');
            trim(str);
            // server.push_back(str);
            // serverID[str] = isx++;
            string svName = str;
            int xv;
            while (getline(ss, str, ','))
            {
                if (str.length())
                {
                    xv = atoi(str.c_str());
                    if (xv > 1000000 || xv < 0)
                    {
                        getinvalid(__LINE__);
                    }
                    // site_bandwidth.push_back(xv);
                }
            }
            if (svName.length())
                vp.push_back(po(svName, xv));
        }
        sort(vp.begin(), vp.end());
        for (int i = 0; i < vp.size(); i++)
        {
            server.push_back(vp[i].name);
            serverID[vp[i].name] = i;
            site_bandwidth.push_back(vp[i].val);
        }
        inFile.close();
    }
    // 读取 qos.csv
    {
        fileName = "qos.csv";
        inFileName = prefix + fileName;

        ifstream inFile(inFileName, ios::in);
        string lineStr;
        vector<vector<string>> strArray;
        // getline(inFile, lineStr);
        while (getline(inFile, lineStr))
        {
            // 存成二维表结构
            stringstream ss(lineStr);
            string str;
            vector<string> strArr;
            // 按照逗号分隔
            // getline(ss, str, ',');
            while (getline(ss, str, ','))
            {
                strArr.push_back(str);
            }
            if (strArr.size())
                strArray.push_back(strArr);
        }
        can.resize(server.size());
        for (int i = 0; i < server.size(); i++)
            can[i].resize(client.size());
        for (int i = 1; i < strArray.size(); i++)
        {
            trim(strArray[i][0]);
            int svid = serverID[strArray[i][0]];
            for (int j = 1; j < strArray[i].size(); j++)
            {
                trim(strArray[0][j]);
                int clid = clientID[strArray[0][j]];
                int xv = atoi(strArray[i][j].c_str());
                if (!(xv > 0 && xv <= 1000))
                {
                    getinvalid(__LINE__);
                }
                can[svid][clid] = (xv < qos_constraint);
            }
        }
        inFile.close();
    }
}
int iter = 0;
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

    void init(int cli, int srv)
    {
        // n = cli + srv + 2;
        src = cli + srv;
        sink = src + 1;
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

    int dfs(int u, int delta, int lim)
    {
        if (u == sink)
            return delta;
        int ret = 0;
        if (u == src)
        {
            vector<int> tv;
            for (int i = g[u]; i; i = e[i].nxt)
                tv.push_back(i);

            // random_shuffle(tv.begin(), tv.end());
            for (int ik = 0; ik < tv.size(); ik++)
            {
                int i = tv[ik];
                if (e[i].f && dist[e[i].v] == dist[u] + 1 && e[i ^ 1].f < lim)
                {
                    int dd = dfs(e[i].v, min(lim, min(e[i].f, delta)), lim);
                    e[i].f -= dd;
                    e[i ^ 1].f += dd;
                    delta -= dd;
                    ret += dd;
                }
            }
        }
        else
        {
            for (int i = g[u]; i; i = e[i].nxt)
            {
                if (e[i].f && dist[e[i].v] == dist[u] + 1)
                {
                    int dd = dfs(e[i].v, min(e[i].f, delta), lim);
                    e[i].f -= dd;
                    e[i ^ 1].f += dd;
                    delta -= dd;
                    ret += dd;
                }
            }
        }

        return ret;
    }

    int maxflow(int sum)
    {
        int ret = 0;
        // int mv = sum / server.size() + 1;
        int mxspt = *max_element(site_bandwidth.begin(), site_bandwidth.end());
        // int delt = (mxspt - mv) / 1000 + 1;
        // int lim = mv;
        int delt = mxspt / 1000 + 1;
        int lim = delt;

        while (sum != ret)
        {
            for (int i = 0; i < server.size(); i++)
            {
                memset(vis, 0, sizeof(vis));
                bfs();
                if (!vis[sink])
                    return ret;
                int r = dfs(src, inf, lim);
                if (r == 0)
                    break;
                ret += r;
            }
            lim += delt;
        }
        return ret;
    }
} mcf;
vector<int> cb[135];
void solv()
{
    int cc = demand.size() * 0.05;
    // 写文件
    ofstream outFile;
    // const string ofileName = "/output/solution.txt";
    const string ofileName = "./output/solution.txt";
    // const string ofileName = "./output/solution.txt";
    if (client.size() > 35 || server.size() > 135)
    {
        getinvalid(__LINE__);
    }
    outFile.open(ofileName, ios::out); // 打开模式可省略
    int start = server.size() - 1;
    //
    ofstream outsv;
    // const string ofileName = "/output/solution.txt";
    const string svfile = "/home/fx/softdoc/gitfiles/miscellaneous/hw2022/sv.txt";
    outsv.open(svfile, ios::out); // 打开模式可省略
    //
    for (int it = 0; it < demand.size(); it++)
    {
        mcf.init(client.size(), server.size());
        for (int i = 0; i < server.size(); i++)
        {
            for (int j = 0; j < client.size(); j++)
            {
                if (can[i][j])
                {
                    int mx = min(site_bandwidth[i], demand[it][j]);
                    ///服务器到客户端
                    mcf.addedge(i, j + server.size(), mx);
                    // mcf.addedge(i + client.size(), j, 0);
                }
            }
        }
        for (int i = 0; i < server.size(); i++)
        {
            mcf.addedge(mcf.src, i, site_bandwidth[i]);
            // mcf.addedge(j, mcf.src, 0);
        }
        for (int j = 0; j < client.size(); j++)
        {
            mcf.addedge(j + server.size(), mcf.sink, demand[it][j]);
            // mcf.addedge(mcf.sink, i + client.size(), 0);
        }
        int sum = accumulate(demand[it].begin(), demand[it].end(), 0);

        iter = it;
        int mf = 0;

        mf = mcf.maxflow(sum);

        assert(mf == sum);
        bool ck = true;
        int ussv[135];

        int cnt[135];
        memset(ussv, 0, sizeof(ussv));
        for (int c = 0; c < client.size(); c++)
        {
            outFile << client[c] << ":";
            memset(cnt, 0, sizeof(cnt));
            for (int i = mcf.g[c + server.size()]; i; i = mcf.e[i].nxt)
            {
                if (mcf.e[i].raw == 0)
                {
                    int sv_id = mcf.e[i].v;
                    cnt[sv_id] += mcf.e[i].f;
                    ussv[sv_id] += mcf.e[i].f;
                    if (ussv[sv_id] > site_bandwidth[sv_id])
                    {
                        getinvalid(__LINE__);
                    }
                }
            }

            if (accumulate(cnt, cnt + 135, 0) != demand[it][c])
            {
                int x = accumulate(cnt, cnt + 135, 0);
                int y = demand[it][c];
                getinvalid(__LINE__);
            }
            bool prt = false;
            for (int i = 0; i < server.size(); i++)
            {
                if (cnt[i] > 0)
                {
                    if (prt)
                    {
                        outFile << ",";
                        // putchar(',');
                    }
                    outFile << "<" << server[i] << "," << cnt[i] << ">";

                    prt = true;
                }
            }
            outFile << endl;

            for (int i = 0; i < server.size(); i++)
            {
                cb[i].push_back(ussv[i]);
                outsv << ussv[i] << "\t";
            }
            outsv << endl;
        }
    }
    outFile.close();
}

int getchengben()
{
    const double eps = 1e-8;
    int s = 0;
    int t = ceil(cb[0].size() * 0.95 + eps) + eps;
    for (int i = 0; i < server.size(); i++)
    {
        sort(cb[i].begin(), cb[i].end());
        s += cb[i][t - 1];
    }
    return s;
}
int main()
{
    readData();
    solv();
    printf("chengben=%d\n", getchengben());
    return 0;
}