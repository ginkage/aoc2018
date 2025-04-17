#include "../lib.hpp"

typedef Eigen::Vector3i v3;

struct area_t {
    v3 mn, mx;
};

inline int dist(v3 a, v3 b) {
    v3 d = a - b;
    return abs(d[0]) + abs(d[1]) + abs(d[2]);
}

inline bool clash(long dst1, long dst2, v3 &p1, v3 &p2, v3 &b1, v3 &b2, const int axis) {
    if (dst1 * dst2 >= 0) return false;
    if (dst1 == dst2) return false;
    for (int i = 0; i < 3; i++)
        if (i != axis) {
            long d = dst1 - dst2, hit = p2[i] * dst1 - p1[i] * dst2;
            if (hit < b1[i] * d || hit > b2[i] * d)
                return false;
        }
    return true;
}

bool line_box(v3 &b1, v3 &b2, v3 &l1, v3 &l2)
{
    for (int i = 0; i < 3; i++)
        if ((l2[i] < b1[i] && l1[i] < b1[i]) || (l2[i] > b2[i] && l1[i] > b2[i]))
            return false;

    bool good = true;
    for (int i = 0; i < 3; i++)
        if (l1[i] < b1[i] || l1[i] > b2[i])
            good = false;
    if (good) return true;

    for (int i = 0; i < 3; i++)
        if (clash(l1[i] - b1[i], l2[i] - b1[i], l1, l2, b1, b2, i) || clash(l1[i] - b2[i], l2[i] - b2[i], l1, l2, b1, b2, i))
            return true;
    return false;
}

bool check_corners(v3 *corner, int n, v3 &p, int r) {
    for (int i = 0; i < n; i++) 
        if (dist(corner[i], p) <= r)
            return true;
    return false;
}

bool intersect(area_t &a, v3 &p, int r) {
    v3 &mn = a.mn, &mx = a.mx, sz = mx - mn;

    if (sz[0] <= 5 && sz[1] <= 5 && sz[2] <= 5) {
        for (int x = mn[0]; x <= mx[0]; x++)
            for (int y = mn[1]; y <= mx[1]; y++)
                for (int z = mn[2]; z <= mx[2]; z++)
                    if (dist(v3(x, y, z), p) <= r)
                        return true;
        return false;
    }

    v3 corner[8] = {
        v3(mn[0], mn[1], mn[2]),
        v3(mn[0], mn[1], mx[2]),
        v3(mn[0], mx[1], mn[2]),
        v3(mn[0], mx[1], mx[2]),
        v3(mx[0], mn[1], mn[2]),
        v3(mx[0], mn[1], mx[2]),
        v3(mx[0], mx[1], mn[2]),
        v3(mx[0], mx[1], mx[2])
    };
    if (check_corners(corner, 8, p, r))
        return true;

    if (p[0] >= mn[0] && p[0] <= mx[0]) {
        v3 corner[4] = {
            v3(p[0], mn[1], mn[2]),
            v3(p[0], mn[1], mx[2]),
            v3(p[0], mx[1], mn[2]),
            v3(p[0], mx[1], mx[2])
        };
        if (check_corners(corner, 4, p, r))
            return true;
    }
    if (p[1] >= mn[1] && p[1] <= mx[1]) {
        v3 corner[4] = {
            v3(mn[0], p[1], mn[2]),
            v3(mn[0], p[1], mx[2]),
            v3(mx[0], p[1], mn[2]),
            v3(mx[0], p[1], mx[2])
        };
        if (check_corners(corner, 4, p, r))
            return true;
    }
    if (p[2] >= mn[2] && p[2] <= mx[2]) {
        v3 corner[4] = {
            v3(mn[0], mn[1], p[2]),
            v3(mn[0], mx[1], p[2]),
            v3(mx[0], mn[1], p[2]),
            v3(mx[0], mx[1], p[2])
        };
        if (check_corners(corner, 4, p, r))
            return true;
    }

    v3 node[6] = {
        v3(p[0] - r, p[1], p[2]),
        v3(p[0] + r, p[1], p[2]),
        v3(p[0], p[1] - r, p[2]),
        v3(p[0], p[1] + r, p[2]),
        v3(p[0], p[1], p[2] - r),
        v3(p[0], p[1], p[2] + r)
    };

    v3 lines[12][2] = {
        { node[0], node[2] },
        { node[0], node[3] },
        { node[0], node[4] },
        { node[0], node[5] },
        { node[1], node[2] },
        { node[1], node[3] },
        { node[1], node[4] },
        { node[1], node[5] },
        { node[2], node[4] },
        { node[2], node[5] },
        { node[3], node[4] },
        { node[4], node[5] }
    };

    for (int i = 0; i < 12; i++)
        if (line_box(mn, mx, lines[i][0], lines[i][1]))
            return true;

    return false;
}

pair<area_t, area_t> split(area_t &a) {
    v3 &mn = a.mn, &mx = a.mx;
    v3 sz = mx - mn;
    if (sz[0] >= sz[1] && sz[0] >= sz[2]) {
        int mid = mn[0] + sz[0] / 2;
        return make_pair(area_t { mn, v3(mid, mx[1], mx[2]) }, area_t { v3(mid + 1, mn[1], mn[2]), mx });
    }
    else if (sz[1] >= sz[0] && sz[1] >= sz[2]) {
        int mid = mn[1] + sz[1] / 2;
        return make_pair(area_t { mn, v3(mx[0], mid, mx[2]) }, area_t { v3(mn[0], mid + 1, mn[2]), mx });
    }
    else {
        int mid = mn[2] + sz[2] / 2;
        return make_pair(area_t { mn, v3(mx[0], mx[1], mid) }, area_t { v3(mn[0], mn[1], mid + 1), mx });
    }
}

inline int adist(area_t &a) {
    v3 md = (a.mn + a.mx) / 2;
    return dist(md, v3(0, 0, 0));
}

vector<v3> pos;
vector<int> r;

int count(area_t &a) {
    int k = 0;
    for (int i = 0; i < pos.size(); i++)
        if (intersect(a, pos[i], r[i]))
            k++;
    return k;
}

int main() {
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

    v3 mn = pos[0], mx = pos[0];
    for (int i = 0; i < pos.size(); i++) {
        v3 p = pos[i];
        for (int j = 0; j < 3; j++) {
            mn[j] = min(mn[j], p[j]);
            mx[j] = max(mx[j], p[j]);
        }
    }

    auto cmp = [](pair<int, area_t> a, pair<int, area_t> b) { return a.first < b.first || (a.first == b.first && adist(a.second) > adist(b.second)); };
    priority_queue<pair<int, area_t>, vector<pair<int, area_t>>, decltype(cmp)> q(cmp);

    area_t a { mn, mx };
    int k = count(a);
    q.push(make_pair(k, a));

    while (!q.empty()) {
        auto p = q.top();
        k = p.first;
        a = p.second;
        q.pop();

        cout << k << ": " << a.mn[0] << ' ' << a.mn[1] << ' ' << a.mn[2] << ' ' << a.mx[0] << ' ' << a.mx[1] << ' ' << a.mx[2] << ' ' << adist(a) << endl;
        if (a.mn == a.mx)
            break;

        auto sp = split(a);
        int k1 = count(sp.first), k2 = count(sp.second);
        q.push(make_pair(k1, sp.first));
        q.push(make_pair(k2, sp.second));
        cout << k1 << ' ' << k2 << endl;
    }

    return 0;
}

