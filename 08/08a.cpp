#include "../lib.hpp"

long recur(vector<int>::iterator &it) {
    long result = 0;
    int nodes = *it++;
    int entries = *it++;
    while (nodes--)
        result += recur(it);
    while (entries--)
        result += *it++;
    return result;
}

int main() {
    string s;
    getline(cin, s);
    vector<int> input = split_i(s, " ");
    auto it = input.begin();
    cout << recur(it) << endl;
    return 0;
}

