#include "../lib.hpp"

typedef Eigen::Vector3i v3;

int main() {
    vector<v3> pos;
    vector<int> r;
    const regex linerex("pos=<(.*),(.*),(.*)>, r=(.*)");
    while (true) {
        string s;
        getline(cin, s);
        if (!cin) break;

        smatch linematch;
        if (regex_match(s, linematch, linerex) && linematch.size() == 5) {
            pos.emplace_back(stoi(linematch[1].str()), stoi(linematch[2].str()), stoi(linematch[3].str()));
            r.push_back(stoi(linematch[4].str()));
        }
    }

    int n = pos.size(), ms = 0, mi = 0;
    for (int i = 0; i < n; i++)
        if (ms < r[i]) {
            ms = r[i];
            mi = i;
        }

    int result = 0;
    for (int i = 0; i < n; i++) {
        v3 dif = pos[i] - pos[mi];
        int dst = abs(dif[0]) + abs(dif[1]) + abs(dif[2]);
        if (dst <= ms)
            result++;
    }
    cout << result << endl;

    return 0;
}

