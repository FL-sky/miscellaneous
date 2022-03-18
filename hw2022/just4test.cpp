#include <bits/stdc++.h>
using namespace std;

int main()
{
    typedef long long ll;
    auto st = clock();
    int n = 100000000;
    ll s = 0;
    for (ll i = 0; i < n; i++)
        s += (i ^ (i + 10));
    auto ed = clock();
    double endtime = (double)(ed - st) / CLOCKS_PER_SEC;
    cout << s << endl;
    // cout << "Total time:" << endtime << endl; // s为单位
    cout << "Total time:" << endtime * 1000 << "ms" << endl;
    return 0;
}