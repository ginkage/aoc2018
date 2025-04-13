#include "../lib.hpp"

int main() {
    vector<pair<int, int>> coord;
    const regex linerex("(\\d+), (\\d+)");
    int i1 = -1, i2 = -1, j1 = -1, j2 = -1;
    while (true) {
        string s;
        getline(cin, s);
        if (!cin) break;

        smatch linematch;
        if (regex_match(s, linematch, linerex) && linematch.size() == 3) {
            int x = stoi(linematch[1].str());
            int y = stoi(linematch[2].str());
            coord.emplace_back(y, x);
            i1 = i1 < 0 ? y : min(i1, y);
            j1 = j1 < 0 ? x : min(j1, x);
            i2 = i2 < 0 ? y : max(i2, y);
            j2 = j2 < 0 ? x : max(j2, x);
        }
    }

    unordered_map<int, int> count;
    unordered_set<int> blacklist;
    for (int i = i1; i <= i2; i++) {
        for (int j = j1; j <= j2; j++) {
            int md = -1, cc = -1, id = 0, last_id = -1;
            for (auto &p : coord) {
                int dist = abs(p.first - i) + abs(p.second - j);
                if (md < 0 || md > dist) {
                    cc = 0;
                    md = dist;
                    last_id = id;
                }
                else if (md == dist)
                    cc++;
                id++;
            }

            if (cc == 0) {
                count[last_id]++;
                if (i == i1 || i == i2 || j == j1 || j == j2)
                    blacklist.insert(last_id);
            }
        }
    }

    int max_count = -1;
    for (auto &p : count)
        if (blacklist.find(p.first) == blacklist.end())
            max_count = max(max_count, p.second);
    cout << max_count << endl;

    int in_10k = 0;
    for (int i = i1; i <= i2; i++) {
        for (int j = j1; j <= j2; j++) {
            long total = 0;
            for (auto &p : coord)
                total += abs(p.first - i) + abs(p.second - j);
            if (total < 10000)
                in_10k++;
        }
    }
    cout << in_10k << endl;

    return 0;
}

