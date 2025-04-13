#include "../lib.hpp"

int main() {
    vector<long> nums;
    long sum = 0;
    while (cin) {
        long n;
        cin >> n;
        if (!cin) break;
        nums.push_back(n);
    }

    unordered_set<long> met;
    long freq = 0;
    while (true) {
        for (long a : nums) {
            freq += a;
            if (met.find(freq) != met.end()) {
                cout << freq << endl;
                return 0;
            }
            met.insert(freq);
        }
    }

    return 0;
}

