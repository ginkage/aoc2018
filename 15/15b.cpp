#include "../lib.hpp"

static int di[] = { -1, 0, 0, 1 };
static int dj[] = { 0, -1, 1, 0 };

struct unit_t {
    bool elf;
    vec pos;
    int hp;
};

int main() {
    vector<string> original;

    while (true) {
        string s;
        getline(cin, s);
        if (!cin) break;
        original.push_back(s);
    }

    int m = original.size(), n = original[0].size();
    bool fail = true;
    for (int power = 3; power <= 400 && fail; power++) {
        vector<string> grid = original;
        vector<unit_t> units;
        unordered_map<vec, unit_t*, VHash> pos_unit;

        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                char c = grid[i][j];
                if (c == 'G' || c == 'E') {
                    units.push_back(unit_t { (c == 'E'), vec(i, j), 200 });
                    grid[i][j] = '.';
                }
            }
        }

        for (unit_t &unit : units)
            pos_unit[unit.pos] = &unit;

        fail = false;
        bool over = false;
        for (int t = 0; t < 5000 && !over; t++) {
            map<pair<int, int>, unit_t*> sorted;
            for (unit_t &unit : units)
                if (unit.hp > 0)
                    sorted[make_pair(unit.pos[0], unit.pos[1])] = &unit;

            for (auto &p : sorted) {
                unit_t *unit = p.second;
                if (unit->hp <= 0) continue;

                over = true;
                for (unit_t &other : units)
                    if (other.elf != unit->elf && other.hp > 0)
                        over = false;

                if (over) {
                    int sum = 0;
                    for (unit_t &other : units)
                        if (other.hp > 0)
                            sum += other.hp;
                    cout << "No targets left at " << t << ", sum = " << sum << ", outcome = " << t * sum << endl;
                    break;
                }

                vec pos = unit->pos;
                int i = pos[0], j = pos[1];
                vector<vector<int>> visit(m, vector<int>(n, -1));
                vector<vector<vec>> from(m, vector<vec>(n, vec(-1, -1)));

                unit_t *enemy = nullptr;
                visit[i][j] = 0;
                queue<vec> q;
                q.push(pos);
                while (!q.empty()) {
                    vec pos = q.front();
                    int i = pos[0], j = pos[1];
                    q.pop();

                    int dist = visit[i][j];
                    auto it = pos_unit.find(pos);
                    if (it != pos_unit.end() && dist != 0) {
                        if (it->second->elf != unit->elf) {
                            // Closest enemy in reading order!
                            enemy = it->second;
                            break;
                        }
                        else // Somebody's in the way...
                            continue;
                    }

                    for (int k = 0; k < 4; k++) {
                        int ni = i + di[k], nj = j + dj[k];
                        if (visit[ni][nj] < 0 && grid[ni][nj] == '.') {
                            visit[ni][nj] = dist + 1;
                            from[ni][nj] = (dist > 1) ? from[i][j] : pos;
                            q.push(vec(ni, nj));
                        }
                    }
                }

                if (enemy) {
                    if (visit[enemy->pos[0]][enemy->pos[1]] > 1) {
                        // Closest enemy is far away, move towards it.
                        vec to = from[enemy->pos[0]][enemy->pos[1]];
                        pos_unit.erase(unit->pos);
                        pos_unit[to] = unit;
                        unit->pos = to;
                    }

                    // We have a candidate enemy, but are we near, and is it the best one?
                    enemy = nullptr;
                    int i = unit->pos[0], j = unit->pos[1];
                    for (int k = 0; k < 4; k++) {
                        int ni = i + di[k], nj = j + dj[k];
                        auto it = pos_unit.find(vec(ni, nj));
                        if (it != pos_unit.end() && it->second->elf != unit->elf)
                            if (!enemy || enemy->hp > it->second->hp)
                                enemy = it->second;
                    }

                    if (enemy) {
                        // Attack!
                        enemy->hp -= (unit->elf ? power : 3);
                        if (enemy->hp <= 0) {
                            pos_unit.erase(enemy->pos);
                            if (enemy->elf) {
                                fail = true;
                                over = true;
                            }
                        }
                    }
                }
            }
/*
            for (int i = 0; i < m; i++) {
                for (int j = 0; j < n; j++) {
                    auto it = pos_unit.find(vec(i, j));
                    if (it != pos_unit.end())
                        cout << (it->second->elf ? 'E' : 'G');
                    else
                        cout << grid[i][j];
                }
                cout << endl;
            }
            cout << endl;
*/
        }
    }

    return 0;
}

