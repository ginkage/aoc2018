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

    for (int t = 0; t < 10; t++) {
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

        //for (int i = 0; i < m; i++)
        //    cout << next[i] << endl;
        //cout << endl;

        swap(grid, next);
    }

    long trees = 0, lumber = 0;
    for (int i = 0; i < m; i++)
        for (int j = 0; j < n; j++)
            if (grid[i][j] == '|')
                trees++;
            else if (grid[i][j] == '#')
                lumber++;
    cout << (trees * lumber) << endl;

    return 0;
}

