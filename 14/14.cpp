#include "../lib.hpp"

int main()
{
    vector<int8_t> recipe { 3, 7 };
    int p1 = 0, p2 = 1;
    int target = 380621;
    vector<int8_t> tt { 3, 8, 0, 6, 2, 1 };

    for (int t = 0; t < target * 1000; t++) {
        int r1 = recipe[p1], r2 = recipe[p2], nr = r1 + r2;
        if (nr >= 10)
            recipe.push_back(1);
        recipe.push_back(nr % 10);
        p1 = (p1 + 1 + r1) % recipe.size();
        p2 = (p2 + 1 + r2) % recipe.size();
/*
        for (int8_t i : recipe)
            cout << ' ' << int(i);
        cout << endl;
*/
        if (recipe.size() > 7) {
            for (int j = recipe.size() - 7; j <= recipe.size() - 6; j++) {
                bool good = true;
                for (int i = 0; i < 6 && good; i++)
                    good &= tt[i] == recipe[j + i];
                if (good) {
                    cout << j << endl;
                    t = 1000000000;
                    break;
                }
            }
        }
    }

    for (int i = 0; i < 10; i++)
        cout << int(recipe[target + i]);
    cout << endl; 

    return 0;
}
