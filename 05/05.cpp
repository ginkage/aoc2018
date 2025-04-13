#include "../lib.hpp"

long shrink(string s) {
    bool react = true;
    while (react) {
        react = false;
        for (int i = 1; i < s.size() && !react; i++)
            if (tolower(s[i]) == tolower(s[i-1]) && s[i] != s[i-1]) {
                s = s.erase(i - 1, 2);
                react = true;
            }
    }
    return s.size();
}

int main() {
    string poly;
    getline(cin, poly);

    long mn = shrink(poly);
    for (int i = 0; i < 26; i++) {
        string s = poly;
        s.erase(remove(s.begin(), s.end(), 'a' + i), s.end());
        s.erase(remove(s.begin(), s.end(), 'A' + i), s.end());
        mn = min(mn, shrink(s));
    }
    cout << mn << endl;

    return 0;
}

