#include "../lib.hpp"

int main() {
    string s;
    int a = 0, b = 0;
    vector<string> ids;
    while (cin) {
        cin >> s;
        if (!cin) break;
        int cnt[128] {};
        for (char c : s)
            cnt[c]++;
        bool two = false, three = false;
        for (int i = 0; i < 128; i++)
            if (cnt[i] == 2)
                two = true;
            else if (cnt[i] == 3)
                three = true;
        if (two) a++;
        if (three) b++;
        ids.push_back(s);
    }
    cout << a*b << endl;

    for (int i = 0; i < ids.size(); i++) {
        string &s1 = ids[i];
        for (int j = i + 1; j < ids.size(); j++) {
            string &s2 = ids[j];
            int discrep = 0;
            for (int k = 0; k < s1.size(); k++) {
                if (s1[k] != s2[k])
                    discrep++;
            }
            if (discrep == 1)
                cout << s1 << endl << s2 << endl;
        }
    }

    return 0;
}

