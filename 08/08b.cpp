#include "../lib.hpp"

long recur(vector<int>::iterator &it) {
    long result = 0;
    int nodes = *it++;
    int entries = *it++;

    vector<int> data;
    while (nodes--)
        data.push_back(recur(it));

    while (entries--) {
        int index = *it++;
        if (data.size() == 0)
            result += index;
        else if (index > 0 && index <= data.size())
            result += data[index-1];
    }

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

