#include "../lib.hpp"

int main() {
    vector<vector<int>> grid(301, vector<int>(301, 0));
    int serial = 7139;

    for (int i = 1; i <= 300; i++)
        for (int j = 1; j <= 300; j++)
            grid[i][j] = ((((j + 10) * i + serial) * (j + 10)) % 1000) / 100 - 5;

    vector<vector<int>> sum(301, vector<int>(301, 0));
    for (int i = 1; i <= 300; i++) {
        int rs = 0;
        for (int j = 1; j <= 300; j++) {
            rs += grid[i][j];
            sum[i][j] = sum[i-1][j] + rs;
        }
    }

    int ms = 0, mi = 0, mj = 0, mz = 0;
    for (int sz = 0; sz < 300; sz++) {
        for (int i = 1; i <= 300 - sz; i++)
            for (int j = 1; j <= 300 - sz; j++) {
                int sq = sum[i-1][j-1] + sum[i+sz][j+sz] - sum[i-1][j+sz] - sum[i+sz][j-1];
                if (ms < sq) {
                    ms = sq;
                    mi = i;
                    mj = j;
                    mz = sz + 1;
                }
            }
    }
    cout << mj << ',' << mi << ',' << mz << endl;

    return 0;
}

