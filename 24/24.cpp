#include "../lib.hpp"

struct unit_t {
    int count;
    int hp;
    vector<string> immune;
    vector<string> weak;
    int damage;
    string dmg_type;
    int initiative;
    bool infection;

    unit_t *target;
    bool chosen;
    int deal;
};

int main() {
    const regex linerex("(.*) units each with (.*) hit points (\\((.*)\\) )?with an attack that does (.*) (.*) damage at initiative (.*)");
    const regex immurex("immune to (.*)");
    const regex weakrex("weak to (.*)");
    vector<unit_t> units;

    bool infection = false;
    while (true) {
        string s;
        getline(cin, s);
        if (!cin) break;

        smatch linematch;
        if (regex_match(s, linematch, linerex) && linematch.size() == 8) {
            unit_t unit;
            unit.infection = infection;
            unit.count = stoi(linematch[1].str());
            unit.hp = stoi(linematch[2].str());
            unit.damage = stoi(linematch[5].str());
            unit.dmg_type = linematch[6].str();
            unit.initiative = stoi(linematch[7].str());

            for (string ss : split_s(linematch[4].str(), "; "))
                if (regex_match(ss, linematch, immurex) && linematch.size() == 2)
                    unit.immune = split_s(linematch[1].str(), ", ");
                else if (regex_match(ss, linematch, weakrex) && linematch.size() == 2)
                    unit.weak = split_s(linematch[1].str(), ", ");

            units.push_back(unit);
        }
        else if (s.size() == 0)
            infection = !infection;
    }

    bool fail = true;
    vector<unit_t> original = units;
    for (int boost = 0; boost < 10000 && fail; boost++) {
        units = original;
        fail = false;

        vector<unit_t*> sorted;
        for (unit_t &unit : units) {
            if (!unit.infection)
                unit.damage += boost;
            sorted.push_back(&unit);
        }

        while (true) {
            for (unit_t &unit : units)
                if (unit.count > 0) {
                    unit.target = nullptr;
                    unit.chosen = false;
                    unit.deal = 0;
                }

            sort(sorted.begin(), sorted.end(), [](const unit_t *u1, const unit_t *u2){
                    int power1 = u1->count * u1->damage, power2 = u2->count * u2->damage;
                    if (power1 != power2) return power1 > power2;
                    return u1->initiative > u2->initiative; });

            bool chosen = false;
            for (unit_t *unit : sorted) {
                if (unit->count > 0) {
                    unit_t *best = nullptr;
                    for (unit_t *target : sorted) {
                        if (target->count > 0 && target->infection != unit->infection && !target->chosen) {
                            int deal = unit->count * unit->damage;
                            for (string &immune : target->immune)
                                if (unit->dmg_type == immune)
                                    deal = 0;
                            for (string &weak : target->weak)
                                if (unit->dmg_type == weak)
                                    deal *= 2;

                            if (unit->deal < deal) {
                                unit->deal = deal;
                                best = target;
                            }
                        }
                    }

                    if (best) {
                        unit->deal /= unit->count;
                        unit->target = best;
                        best->chosen = true;
                        chosen = true;
                    }
                }
            }

            if (!chosen) {
                int result = 0;
                for (unit_t &unit : units)
                    if (unit.count > 0) {
                        result += unit.count;
                        if (unit.infection)
                            fail = true;
                    }
                if (!fail)
                    cout << result << endl;
                break;
            }

            sort(sorted.begin(), sorted.end(), [](const unit_t *u1, const unit_t *u2){
                    return u1->initiative > u2->initiative; });

            for (unit_t *unit : sorted)
                if (unit->count > 0 && unit->target) {
                    int deal = unit->count * unit->deal;
                    int kills = min(deal / unit->target->hp, unit->target->count);
                    // cout << "Deal " << deal << ", killing " << kills << endl;
                    unit->target->count = unit->target->count - kills;
                }
        }
    }

    return 0;
}

