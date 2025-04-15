#include "../lib.hpp"

enum op_t {
    op_addr,
    op_addi,
    op_mulr,
    op_muli,
    op_banr,
    op_bani,
    op_borr,
    op_bori,
    op_setr,
    op_seti,
    op_gtir,
    op_gtri,
    op_gtrr,
    op_eqir,
    op_eqri,
    op_eqrr
};

long regs[4] {};

unordered_map<op_t, function<void(int, int, int)>> ops = {
    { op_addr, [](int in1, int in2, int out) { regs[out] = regs[in1] + regs[in2]; } },
    { op_addi, [](int in1, int in2, int out) { regs[out] = regs[in1] + in2; } },
    { op_mulr, [](int in1, int in2, int out) { regs[out] = regs[in1] * regs[in2]; } },
    { op_muli, [](int in1, int in2, int out) { regs[out] = regs[in1] * in2; } },
    { op_banr, [](int in1, int in2, int out) { regs[out] = regs[in1] & regs[in2]; } },
    { op_bani, [](int in1, int in2, int out) { regs[out] = regs[in1] & in2; } },
    { op_borr, [](int in1, int in2, int out) { regs[out] = regs[in1] | regs[in2]; } },
    { op_bori, [](int in1, int in2, int out) { regs[out] = regs[in1] | in2; } },
    { op_setr, [](int in1, int in2, int out) { regs[out] = regs[in1]; } },
    { op_seti, [](int in1, int in2, int out) { regs[out] = in1; } },
    { op_gtir, [](int in1, int in2, int out) { regs[out] = in1 > regs[in2]; } },
    { op_gtri, [](int in1, int in2, int out) { regs[out] = regs[in1] > in2; } },
    { op_gtrr, [](int in1, int in2, int out) { regs[out] = regs[in1] > regs[in2]; } },
    { op_eqir, [](int in1, int in2, int out) { regs[out] = in1 == regs[in2]; } },
    { op_eqri, [](int in1, int in2, int out) { regs[out] = regs[in1] == in2; } },
    { op_eqrr, [](int in1, int in2, int out) { regs[out] = regs[in1] == regs[in2]; } }
};

unordered_map<op_t, string> op_name = {
    { op_addr, "addr" },
    { op_addi, "addi" },
    { op_mulr, "mulr" },
    { op_muli, "muli" },
    { op_banr, "banr" },
    { op_bani, "bani" },
    { op_borr, "borr" },
    { op_bori, "bori" },
    { op_setr, "setr" },
    { op_seti, "seti" },
    { op_gtir, "gtir" },
    { op_gtri, "gtri" },
    { op_gtrr, "gtrr" },
    { op_eqir, "eqir" },
    { op_eqri, "eqri" },
    { op_eqrr, "eqrr" }
};

struct sample_t {
    long before[4], after[4], in[4];
};

int main() {
    int result = 0;
    string s;
    vector<sample_t> samples;
    const regex linerex("(Before|After):\\s+\\[(\\d+), (\\d+), (\\d+), (\\d+)\\]");
    const regex fourrex("(\\d+) (\\d+) (\\d+) (\\d+)");
    while (true) {
        getline(cin, s);
        if (!cin) break;

        sample_t ss;
        smatch linematch;
        if (regex_match(s, linematch, linerex) && linematch.size() == 6) {
            ss.before[0] = stoi(linematch[2].str());
            ss.before[1] = stoi(linematch[3].str());
            ss.before[2] = stoi(linematch[4].str());
            ss.before[3] = stoi(linematch[5].str());
        }
        else break;

        getline(cin, s);
        if (regex_match(s, linematch, fourrex) && linematch.size() == 5) {
            ss.in[0] = stoi(linematch[1].str());
            ss.in[1] = stoi(linematch[2].str());
            ss.in[2] = stoi(linematch[3].str());
            ss.in[3] = stoi(linematch[4].str());
        }

        getline(cin, s);
        if (regex_match(s, linematch, linerex) && linematch.size() == 6) {
            ss.after[0] = stoi(linematch[2].str());
            ss.after[1] = stoi(linematch[3].str());
            ss.after[2] = stoi(linematch[4].str());
            ss.after[3] = stoi(linematch[5].str());
        }

        samples.push_back(ss);
        getline(cin, s);

        int match = 0;
        for (int i = 0; i < 16; i++) {
            memcpy(regs, ss.before, sizeof(regs));
            ops[(op_t)i](ss.in[1], ss.in[2], ss.in[3]);
            if (!memcmp(regs, ss.after, sizeof(regs)))
                match++;
        }

        if (match >= 3)
            result++;
    }

    cout << result << endl;

    int fixup[16], left = 16;
    op_t reop[16];
    memset(fixup, -1, sizeof(fixup));
    while (left > 0) {
        for (sample_t &ss : samples) {
            int match = 0;
            op_t m1;
            for (int i = 0; i < 16; i++) {
                if (fixup[i] >= 0) continue;
                memcpy(regs, ss.before, sizeof(regs));
                ops[(op_t)i](ss.in[1], ss.in[2], ss.in[3]);
                if (!memcmp(regs, ss.after, sizeof(regs))) {
                    match++;
                    m1 = (op_t)i;
                }
            }

            if (match == 1) {
                cout << ss.in[0] << ' ' << op_name[m1] << endl;
                fixup[m1] = ss.in[0];
                reop[ss.in[0]] = m1;
                left--;
            }
        }
    }

    memset(regs, 0, sizeof(regs));

    getline(cin, s);
    while (true) {
        getline(cin, s);
        if (!cin) break;

        smatch linematch;
        if (regex_match(s, linematch, fourrex) && linematch.size() == 5) {
            int r0 = stoi(linematch[1].str());
            int r1 = stoi(linematch[2].str());
            int r2 = stoi(linematch[3].str());
            int r3 = stoi(linematch[4].str());
            ops[reop[r0]](r1, r2, r3);
        }
    }

    cout << regs[0] << endl;

    return 0;
}

