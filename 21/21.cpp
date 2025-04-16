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

long regs[6] {};

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

unordered_map<string, op_t> name_op = {
    { "addr", op_addr },
    { "addi", op_addi },
    { "mulr", op_mulr },
    { "muli", op_muli },
    { "banr", op_banr },
    { "bani", op_bani },
    { "borr", op_borr },
    { "bori", op_bori },
    { "setr", op_setr },
    { "seti", op_seti },
    { "gtir", op_gtir },
    { "gtri", op_gtri },
    { "gtrr", op_gtrr },
    { "eqir", op_eqir },
    { "eqri", op_eqri },
    { "eqrr", op_eqrr }
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

struct cmd_t {
    op_t op;
    int in1, in2, out;
};

int main() {
    string s;
    smatch linematch;
    const regex oprex("#ip (\\d+)");

    int ipr = -1;
    getline(cin, s);
    if (regex_match(s, linematch, oprex) && linematch.size() == 2)
        ipr = stoi(linematch[1].str());

    vector<cmd_t> program;
    const regex linerex("(.*) (.*) (.*) (.*)");
    while (true) {
        getline(cin, s);
        if (!cin) break;

        if (regex_match(s, linematch, linerex) && linematch.size() == 5)
            program.push_back(cmd_t {
                    name_op[linematch[1].str()],
                    stoi(linematch[2].str()),
                    stoi(linematch[3].str()),
                    stoi(linematch[4].str())
            });
    }

    unordered_set<long> met;
    memset(regs, 0, sizeof(regs));
    for (int ip = 0; ip >= 0 && ip < program.size(); ip++) {
        if (ip == 28) {
            if (met.find(regs[3]) == met.end()) {
                cout << regs[3] << endl;
                met.insert(regs[3]);
            }
            //break;
        }
        regs[ipr] = ip;
        cmd_t cmd = program[ip];
        ops[cmd.op](cmd.in1, cmd.in2, cmd.out);
        ip = regs[ipr];
    }

    return 0;
}

