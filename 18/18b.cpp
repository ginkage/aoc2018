#include "../lib.hpp"

vector<string> grid;
int m, n;

int main() {
    while (true) {
        string s;
        getline(cin, s);
        if (!cin) break;
        grid.push_back(s);
    }

    m = grid.size();
    n = grid[0].size();

    vector<long> history;
    for (int t = 0; t < 1000; t++) {
        vector<string> next = grid;

        for (int i = 0; i < m; i++)
            for (int j = 0; j < n; j++) {
                int trees = 0, lumber = 0;
                for (int di = -1; di <= 1; di++)
                    for (int dj = -1; dj <= 1; dj++)
                        if (di != 0 || dj != 0) {
                            int ni = i + di, nj = j + dj;
                            if (ni >= 0 && nj >= 0 && ni < m && nj < n) {
                                if (grid[ni][nj] == '|')
                                    trees++;
                                else if (grid[ni][nj] == '#')
                                    lumber++;
                            }
                        }

                if (grid[i][j] == '.') {
                    if (trees >= 3)
                        next[i][j] = '|';
                }
                else if (grid[i][j] == '|') {
                    if (lumber >= 3)
                        next[i][j] = '#';
                }
                else {
                    if (trees == 0 || lumber == 0)
                        next[i][j] = '.';
                }
            }

        swap(grid, next);

        long trees = 0, lumber = 0;
        for (int i = 0; i < m; i++)
            for (int j = 0; j < n; j++)
                if (grid[i][j] == '|')
                    trees++;
                else if (grid[i][j] == '#')
                    lumber++;
        history.push_back(trees * lumber);

        if (history.size() > 100) {
            int n = history.size() - 1;
            for (int w = 3; w <= 50; w++) {
                int pn = n - w;
                bool good = true;
                for (int i = 0; i < w && good; i++)
                    if (history[n - i] != history[pn - i])
                        good = false;
                if (good) {
                    cout << history[pn + (1000000000 - pn) % w - 1] << endl;
                    t = 1000;
                    break;
                }
            }
        }
    }

    return 0;
}

