#include "../lib.hpp"

enum type_t {
    type_rocky,
    type_wet,
    type_narrow
};

int main() {
    unordered_map<vec, long, VHash> erosion;

    long result = 0;
    int depth = 5616;
    vec target(785, 10), zero(0, 0);
    for (int i = 0; i <= target[0]; i++) {
        for (int j = 0; j <= target[1]; j++) {
            vec pos(i, j);
            int geoindex = -1;
            if (pos == target || pos == zero)
                geoindex = 0;
            else if (i == 0)
                geoindex = j * 16807l;
            else if (j == 0)
                geoindex = i * 48271l;
            else
                geoindex = erosion[vec(i-1, j)] * erosion[vec(i, j-1)];

            long eros = (geoindex + depth) % 20183;
            erosion[pos] = eros;
            result += eros % 3;
        }
    }
    cout << result << endl;

    return 0;
}
