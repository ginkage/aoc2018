#include "../lib.hpp"

enum type_t {
    type_rocky,
    type_wet,
    type_narrow
};

enum gear_t {
    gear_none,
    gear_torch,
    gear_climb
};

struct state_t {
    vec pos;
    gear_t gear;
};

int depth = 5616;
vec zero(0, 0), target(785, 10);
unordered_map<vec, long, VHash> erosion;

long erode(vec pos) {
    auto it = erosion.find(pos);
    if (it != erosion.end())
        return it->second;

    int geoindex = -1, i = pos[0], j = pos[1];
    if (pos == target || pos == zero)
        geoindex = 0;
    else if (i == 0)
        geoindex = j * 16807l;
    else if (j == 0)
        geoindex = i * 48271l;
    else
        geoindex = erode(vec(i - 1, j)) * erode(vec(i, j - 1));

    long eros = (geoindex + depth) % 20183;
    erosion[pos] = eros;
    return eros;
}

type_t grid(vec pos) {
    return (type_t)(erode(pos) % 3);
}

unordered_map<vec, unordered_map<gear_t, long>, VHash> visit;

long dist(state_t state) {
    auto it = visit.find(state.pos);
    if (it == visit.end())
        return -1;

    auto jt = it->second.find(state.gear);
    if (jt == it->second.end())
        return -1;

    return jt->second;
}

vec delta[] = {
    vec(-1, 0),
    vec(0, -1),
    vec(0, 1),
    vec(1, 0)
};

bool good_state(state_t s) {
    return s.pos[0] >= 0 && s.pos[1] >= 0 && (int)grid(s.pos) != (int)s.gear && dist(s) < 0;
}

int main() {
    auto cmp = [](pair<int, state_t> a, pair<int, state_t> b) { return a.first > b.first; };
    priority_queue<pair<int, state_t>, vector<pair<int, state_t>>, decltype(cmp)> q(cmp);

    state_t state { zero, gear_torch };
    q.push(make_pair(0, state));

    while (!q.empty()) {
        auto p = q.top();
        q.pop();
        int d = p.first;
        state_t st = p.second;

        if (st.pos == target && st.gear == gear_torch) {
            cout << d << endl;
            break;
        }

        if (dist(st) < 0) {
            visit[st.pos][st.gear] = d;

            for (int i = 0; i < 4; i++) {
                state_t alt { st.pos + delta[i], st.gear }; // Moving with the same gear
                if (good_state(alt))
                    q.push(make_pair(d + 1, alt));
            }

            for (int i = 0; i < 3; i++)
                if (i != (int)st.gear) {
                    state_t alt { st.pos, (gear_t) i }; // Switching gear
                    if (good_state(alt))
                        q.push(make_pair(d + 7, alt));
                }
        }
    }

    return 0;
}
