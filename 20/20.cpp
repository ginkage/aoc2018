#include "../lib.hpp"

unordered_map<char, vec> dir = {
    { 'N', vec(-1, 0) },
    { 'W', vec(0, -1) },
    { 'E', vec(0, 1) },
    { 'S', vec(1, 0) }
};

unordered_map<vec, unordered_set<vec, VHash>, VHash> edges;

unordered_set<vec, VHash> parse(string &s, int &i, unordered_set<vec, VHash> starts) {
    unordered_set<vec, VHash> ends;

    if (i >= s.size() || s[i] == '$') return starts;
    if (s[i] == '^') i++;

    unordered_set<vec, VHash> pos = starts;
    while (i < s.size()) {
        char c = s[i++];
        if (c == ')' || c == '$') {
            ends.merge(pos);
            break;
        }
        else if (c == '|') {
            ends.merge(pos);
            pos = starts;
        }
        else if (c == '(') {
            pos = parse(s, i, pos);
        }
        else {
            unordered_set<vec, VHash> step;
            for (vec p : pos) {
                vec next = p + dir[c];
                edges[p].insert(next); // Door!
                edges[next].insert(p); // Door!
                step.insert(next);
            }
            swap(pos, step);
        }
    }

    return ends;
}

int main()
{
    string s;
    getline(cin, s);
    int i = 0;
    unordered_set<vec, VHash> start { vec(0, 0) };
    parse(s, i, start);

    int mx = 0;
    unordered_map<vec, int, VHash> visit;
    queue<vec> q;
    q.emplace(0, 0);
    visit[vec(0, 0)] = 0;
    while (!q.empty()) {
        vec pos = q.front();
        q.pop();

        int dist = visit[pos];
        for (const vec &edge: edges[pos]) {
            if (visit.find(edge) == visit.end()) {
                visit[edge] = dist + 1;
                q.push(edge);
            }
        }
        mx = max(mx, dist);
    }

    cout << mx << endl;

    int far = 0;
    for (auto &p : visit)
        if (p.second >= 1000)
            far++;
    cout << far << endl;

    return 0;
}
