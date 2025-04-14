#include "../lib.hpp"

int main() {
    int np = 486, lm = 7083300;
    list<int> circle;
    circle.push_back(0);
    int cur = 0, cp = 0;
    vector<long> player(np);
    auto it = circle.begin();
    for (int i = 1; i <= lm; i++) {
        if (i % 23) {
            for (int i = 0; i < 2; i++) {
                if (it == circle.end())
                    it = circle.begin();
                it++;
            }
            it = circle.insert(it, i);
        }
        else {
            for (int i = 0; i < 7; i++) {
                if (it == circle.begin())
                    it = circle.end();
                it--;
            }
            player[cp] += *it + i;
            it = circle.erase(it);
        }
        cp = (cp + 1) % player.size();
    }

    long ms = 0;
    for (long score : player)
        if (ms < score)
            ms = score;

    cout << ms << endl;

    return 0;
}
