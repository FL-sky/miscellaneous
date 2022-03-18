#include <bits/stdc++.h>
using namespace std;
int gtrd(int md = 550000)
{
    unsigned int rt = rand() * rand() % md + 1;
    return rt;
}
void genDemand()
{
    int n = 35;
    freopen("demand.csv", "w", stdout);
    printf("mtime");
    for (int i = 0; i < n; i++)
        printf(",%d", i);
    puts("");
    int T = 8928;
    int mx = 0;
    for (int i = 0; i < T; i++)
    {
        printf("TIME");
        int ts = 0, tp;
        for (int j = 0; j < n; j++)
        {
            printf(",%d", tp = gtrd());
            ts += tp;
        }
        mx = max(mx, ts);
        puts("");
    }
    printf("mx=%d\n", mx);
}

void genQos()
{
    int n = 35;
    freopen("qos.csv", "w", stdout);
    printf("site_name");
    for (int i = 0; i < n; i++)
        printf(",%d", i);
    puts("");
    int m = 135;
    for (int i = 0; i < m; i++)
    {
        printf("%d", i);
        for (int j = 0; j < n; j++)
        {
            printf(",%d", 1);
        }
        puts("");
    }
}

void gen_site_bandwidth()
{
    int n = 35;
    freopen("site_bandwidth.csv", "w", stdout);
    printf("site_name,bandwidth\n");
    const int m = 135;
    const int sx = 13085450 / m + 1;
    for (int i = 0; i < m; i++)
    {
        int tp = gtrd(1000000);
        if (tp < sx)
            tp = sx + rand();
        printf("%d,%d\n", i, tp);
    }
}

int main()
{

    genQos();
    return 0;
}