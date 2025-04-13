#include "../lib.hpp"

struct claim_t {
    int id, x, y, w, h;
};

int main() {
    unordered_map<pair<int, int>, int, Hash> cloth;
    vector<claim_t> claims;
    const regex linerex("#(\\d+) @ (\\d+),(\\d+): (\\d+)x(\\d+)");
    while (true) {
        string s;
        getline(cin, s);
        if (!cin) break;

        smatch linematch;
        if (regex_match(s, linematch, linerex) && linematch.size() == 6) {
            claim_t claim {
                stoi(linematch[1].str()),
                stoi(linematch[2].str()),
                stoi(linematch[3].str()),
                stoi(linematch[4].str()),
                stoi(linematch[5].str())
            };
            claims.push_back(claim);

            for (int i = 0; i < claim.w; i++)
                for (int j = 0; j < claim.h; j++)
                    cloth[make_pair(claim.x + i, claim.y + j)]++;
        }
    }

    int result = 0;
    for (auto &p : cloth)
        if (p.second > 1)
            result++;
    cout << result << endl;

    for (claim_t &claim : claims) {
        bool good = true;
        for (int i = 0; i < claim.w; i++)
            for (int j = 0; j < claim.h; j++)
                if (cloth[make_pair(claim.x + i, claim.y + j)] > 1)
                    good = false;
        if (good)
            cout << claim.id << endl;
    }

    return 0;
}

