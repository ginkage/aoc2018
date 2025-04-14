#include "../lib.hpp"

struct job_t {
    char id;
    int from, to;
};

int main() {
    unordered_map<char, unordered_set<char>> deps, unlock;
    const regex linerex("Step (.*) must be finished before step (.*) can begin.");
    while (true) {
        string s;
        getline(cin, s);
        if (!cin) break;

        smatch linematch;
        if (regex_match(s, linematch, linerex) && linematch.size() == 3) {
            char id1 = linematch[1].str()[0];
            char id2 = linematch[2].str()[0];
            unlock[id1].insert(id2);
            deps[id2].insert(id1);
        }
    }

    set<char> ready;
    for (auto &p : unlock)
        if (deps[p.first].size() == 0)
            ready.insert(p.first);

    vector<job_t> jobs;
    for (int t = 0; ready.size() > 0 || jobs.size() > 0; t++) {
        // Scan jobs for completed ones
        for (int i = 0; i < jobs.size(); i++) {
            job_t &job = jobs[i];
            if (job.to == t) {
                for (auto &d : unlock[job.id]) {
                    deps[d].erase(job.id);
                    if (deps[d].size() == 0)
                        ready.insert(d);
                }

                swap(job, jobs.back());
                jobs.pop_back();
                i--;
            }
        }

        // Assign ready jobs to free workers
        while (ready.size() > 0 && jobs.size() < 5) {
            char c = *ready.begin();
            cout << c;
            ready.erase(c);
            jobs.push_back(job_t { c, t, t + (c - 'A' + 61) });
        }

        if (ready.size() == 0 && jobs.size() == 0)
            cout << endl << t << endl;
    }

    return 0;
}

