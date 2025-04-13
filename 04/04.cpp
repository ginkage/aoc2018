#include "../lib.hpp"

int days[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

long timestamp(smatch &linematch) {
    int month = stoi(linematch[1].str());
    int day = stoi(linematch[2].str());
    int hour = stoi(linematch[3].str());
    int minute = stoi(linematch[4].str());

    if (hour == 23) {
        minute = 0;
        day++;
        if (day > days[month - 1]) {
            month++;
            day = 1;
        }
    }

    return month * 10000 + day * 100 + minute;
}

int main() {
    unordered_map<int, int> day_guard;
    unordered_map<int, vector<int>> day_sleep;
    unordered_map<int, vector<int>> day_awake;
    const regex startrex("\\[1518-(\\d+)-(\\d+) (\\d+):(\\d+)\\] Guard #(\\d+) begins shift");
    const regex sleeprex("\\[1518-(\\d+)-(\\d+) (\\d+):(\\d+)\\] falls asleep");
    const regex wakeyrex("\\[1518-(\\d+)-(\\d+) (\\d+):(\\d+)\\] wakes up");
    while (true) {
        string s;
        getline(cin, s);
        if (!cin) break;

        smatch linematch;
        if (regex_match(s, linematch, startrex) && linematch.size() == 6) {
            long time = timestamp(linematch);
            day_guard[time / 100] = stoi(linematch[5].str());
        }
        else if (regex_match(s, linematch, sleeprex) && linematch.size() == 5) {
            long time = timestamp(linematch);
            day_sleep[time / 100].push_back(time % 100);
        }
        else if (regex_match(s, linematch, wakeyrex) && linematch.size() == 5) {
            long time = timestamp(linematch);
            day_awake[time / 100].push_back(time % 100);
        }
    }

    unordered_map<int, vector<pair<int, int>>> guard_sleep;
    unordered_map<int, long> total_sleep;
    for (auto &p : day_guard) {
        int day = p.first, id = p.second;
        vector<int> &sleep = day_sleep[day], &awake = day_awake[day];
        sort(sleep.begin(), sleep.end());
        sort(awake.begin(), awake.end());
        for (int i = 0; i < sleep.size(); i++) {
            guard_sleep[id].emplace_back(day * 100 + sleep[i], day * 100 + awake[i]);
            total_sleep[id] += awake[i] - sleep[i];
        }
    }

    int freq_min = 0, freq_id = 0;
    unordered_map<int, int> guard_minute;
    for (auto &g : guard_sleep) {
        int id = g.first;
        unordered_map<int, int> minutes;
        for (auto &p : g.second) {
            int m1 = p.first % 100, m2 = p.second % 100;
            for (int i = m1; i < m2; i++)
                minutes[i]++;
        }

        int max_min = 0, max_slm = 0;
        for (auto &p : minutes) {
            if (max_slm < p.second) {
                max_slm = p.second;
                max_min = p.first;
            }
        }
        guard_minute[id] = max_min;

        if (freq_min < max_slm) {
            freq_min = max_slm;
            freq_id = id;
        }
    }

    int max_sleep = 0, max_id = 0;
    for (auto &p : total_sleep) {
        if (max_sleep < p.second) {
            max_sleep = p.second;
            max_id = p.first;
        }
    }

    cout << "Guard " << max_id << ", minute " << guard_minute[max_id] << endl;
    cout << (max_id * guard_minute[max_id]) << endl;

    cout << "Guard " << freq_id << ", minute " << guard_minute[freq_id] << endl;
    cout << (freq_id * guard_minute[freq_id]) << endl;

    return 0;
}

