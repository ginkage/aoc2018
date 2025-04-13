#include <iostream>

using namespace std;

int main() {
    long sum = 0;
    while (cin) {
        long n;
        cin >> n;
        if (!cin) break;
        sum += n;
    }
    cout << sum << endl;
    return 0;
}

