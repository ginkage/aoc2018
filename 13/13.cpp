#include "../lib.hpp"

struct cart_t {
    vec pos, dir;
    int turn;
    bool crashed;
};

map<char, vec> direction {
    { '<', vec(0, -1) },
    { 'v', vec(1, 0) },
    { '>', vec(0, 1) },
    { '^', vec(-1, 0) }
};

vector<string> grid;
int m, n;

vec left(vec dir) {
    return vec(-dir[1], dir[0]);
}

vec right(vec dir) {
    return vec(dir[1], -dir[0]);
}

int main() {
    while (true) {
        string s;
        getline(cin, s);
        if (!cin) break;
        grid.push_back(s);
    }

    m = grid.size();
    n = grid[0].size();

    vector<vector<bool>> is_cart(m, vector<bool>(n, false));
    vector<cart_t> carts;
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            char c = grid[i][j];
            if (c == '<' || c == '>' || c == 'v' || c == '^') {
                if (c == '<' || c == '>')
                    grid[i][j] = '-';
                else
                    grid[i][j] = '|';
                carts.push_back(cart_t { vec(i, j), direction[c], 0 });
                is_cart[i][j] = false;
            }
        }
    }

    for (int t = 0; t < 1500000; t++) {
        int remain = 0;
        map<pair<int, int>, cart_t*> sorted;
        for (cart_t &cart : carts)
            if (!cart.crashed) {
                sorted[make_pair(cart.pos[0], cart.pos[1])] = &cart;
                remain++;
            }

        for (auto &p : sorted) {
            cart_t *cart = p.second;
            if (cart->crashed) continue;

            is_cart[cart->pos[0]][cart->pos[1]] = false;
            cart->pos += cart->dir;
            int i = cart->pos[0], j = cart->pos[1];
            if (is_cart[i][j]) {
                cout << j << ',' << i << endl;
                for (cart_t &c : carts)
                    if (c.pos == cart->pos && !c.crashed) {
                        c.crashed = true;
                        remain--;
                    }
                is_cart[i][j] = false;
            }
            else {
                char c = grid[i][j];
                if (c == '+') {
                    int turn = cart->turn;
                    if (turn == 0)
                        cart->dir = left(cart->dir);
                    else if (turn == 2)
                        cart->dir = right(cart->dir);
                    cart->turn = (turn + 1) % 3;
                }
                else if (c == '/') {
                    if (cart->dir[0] == 0)
                        cart->dir = left(cart->dir);
                    else
                        cart->dir = right(cart->dir);
                }
                else if (c == '\\') {
                    if (cart->dir[1] == 0)
                        cart->dir = left(cart->dir);
                    else
                        cart->dir = right(cart->dir);
                }
                is_cart[i][j] = true;
            }
        }
/*
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                char c = grid[i][j];
                if (is_cart[i][j])
                    cout << '#';
                else
                    cout << c;
            }
            cout << endl;
        }
        cout << endl;
*/
        if (remain == 1) {
            for (cart_t &c : carts)
                if (!c.crashed)
                    cout << c.pos[1] << ',' << c.pos[0] << endl;
            break;
        }
    }

    return 0;
}

