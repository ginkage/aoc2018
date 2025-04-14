#include "../lib.hpp"

int main() {
    unordered_set<int> plants;

    string s, state;
    getline(cin, s);
    const regex linerex("initial state: (.*)");
    smatch linematch;
    if (regex_match(s, linematch, linerex) && linematch.size() == 2)
        state = linematch[1].str();
    getline(cin, s);

    unordered_map<string, bool> rules;
    const regex rulerex("(.*) => (.*)");
    while (true) {
        getline(cin, s);
        if (!cin) break;

        if (regex_match(s, linematch, rulerex) && linematch.size() == 3)
            rules[linematch[1].str()] = (linematch[2].str() == "#");
    }

    for (int i = 0; i < state.size(); i++)
        if (state[i] == '#')
            plants.insert(i);

    for (int t = 0; t < 500; t++) {
        unordered_set<int> next;

        int mn = 1000000000, mx = 0;
        for (int p : plants) {
            mn = min(p, mn);
            mx = max(p, mx);
        }

        string st = ".....";
        for (int x = mn - 5; x <= mx + 5; x++) {
            if (rules[st])
                next.insert(x - 3);

            for (int i = 0; i < 4; i++)
                st[i] = st[i+1];
            st[4] = plants.find(x) != plants.end() ? '#' : '.';
            cout << st[4];
        }
        cout << endl;

        long sum = 0;
        for (int x : plants)
            sum += x;
        cout << sum << endl;

        swap(plants, next);
    }

    return 0;
}

