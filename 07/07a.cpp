#include "../lib.hpp"

int main() {
    unordered_map<char, unordered_set<char>> deps, unlock;
    const regex linerex("Step (.*) must be finished before step (.*) can begin.");
    while (true) {
        string s;
        getline(cin, s);
        if (!cin) break;

        smatch linematch;
        if (regex_match(s, linematch, linerex) && linematch.size() == 3) {
            char id1 = linematch[1].str()[0];
            char id2 = linematch[2].str()[0];
            unlock[id1].insert(id2);
            deps[id2].insert(id1);
        }
    }

    set<char> ready;
    for (auto &p : unlock)
        if (deps[p.first].size() == 0)
            ready.insert(p.first);

    while (ready.size() > 0) {
        char c = *ready.begin();
        cout << c;
        ready.erase(c);
        for (auto &d : unlock[c]) {
            deps[d].erase(c);
            if (deps[d].size() == 0)
                ready.insert(d);
        }
    }
    cout << endl;

    return 0;
}

