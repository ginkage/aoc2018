#include "../lib.hpp"

int dist(vector<int> &p1, vector<int> &p2) {
    int result = 0;
    for (int i = 0; i < 4; i++)
        result += abs(p1[i] - p2[i]);
    return result;
}

int main() {
    vector<vector<int>> points;
    while (true) {
        string s;
        getline(cin, s);
        if (!cin) break;
        points.push_back(split_i(s, ","));
    }

    vector<int> parent(points.size());
    for (int i = 0; i < points.size(); i++)
        parent[i] = i;

    for (int i = 0; i < points.size(); i++) {
        for (int j = i + 1; j < points.size(); j++) {
            vector<int> &p1 = points[i];
            vector<int> &p2 = points[j];
            if (dist(p1, p2) <= 3) {
                int x = i;
                while (x != parent[x]) {
                    parent[x] = parent[parent[x]];
                    x = parent[x];
                }

                int y = j;
                while (y != parent[y]) {
                    parent[y] = parent[parent[y]];
                    y = parent[y];
                }

                if (x != y)
                    parent[y] = x;
            }
        }
    }

    int y = 0;
    while (y != parent[y]) {
        parent[y] = parent[parent[y]];
        y = parent[y];
    }

    int result = 0;
    for (int i = 0; i < parent.size(); i++)
        if (parent[i] == i)
            result++;
    cout << result << endl;

    return 0;
}

