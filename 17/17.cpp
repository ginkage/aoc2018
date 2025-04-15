#include "../lib.hpp"

int main() {
    png::image<png::index_pixel> image;
    png::palette pal(4);
    pal[0] = png::color(255, 255, 255);
    pal[1] = png::color(0, 0, 0);
    pal[2] = png::color(0, 127, 255);
    pal[3] = png::color(0, 0, 255);
    image.set_palette(pal);
    image.resize(1000, 2000);

    for (png::uint_32 y = 0; y < image.get_height(); ++y)
        for (png::uint_32 x = 0; x < image.get_width(); ++x)
            image[y][x] = png::index_pixel(0);

    vector<string> grid(2000, string(1000, '.'));
    int mn = 2000, mx = 0;
    const regex linerex1("x=(.*), y=(.*)\\.\\.(.*)");
    const regex linerex2("y=(.*), x=(.*)\\.\\.(.*)");
    while (true) {
        string s;
        getline(cin, s);
        if (!cin) break;

        smatch linematch;
        if (regex_match(s, linematch, linerex1) && linematch.size() == 4) {
            int x = stoi(linematch[1].str());
            int y1 = stoi(linematch[2].str());
            int y2 = stoi(linematch[3].str());
            for (int i = y1; i <= y2; i++) {
                grid[i][x] = '#';
                image[i][x] = png::index_pixel(1);
            }
            mn = min(mn, y1);
            mx = max(mx, y2);
        }
        else if (regex_match(s, linematch, linerex2) && linematch.size() == 4) {
            int y = stoi(linematch[1].str());
            int x1 = stoi(linematch[2].str());
            int x2 = stoi(linematch[3].str());
            for (int j = x1; j <= x2; j++) {
                grid[y][j] = '#';
                image[y][j] = png::index_pixel(1);
            }
            mn = min(mn, y);
            mx = max(mx, y);
        }
    }

    int sx = 500, sy = 0;
    stack<vec> source;
    for (int i = sy; i <= mx && grid[i][sx] == '.'; i++)
        source.emplace(i, sx);

    while (!source.empty()) {
        vec pos = source.top();
        source.pop();
        int i = pos[0], j = pos[1];

        if (grid[i][j] == '.') {
            grid[i][j] = '|';
            image[i][j] = png::index_pixel(2);
        }

        if (grid[i + 1][j] == '|') {
            // Nowhere to stand on...
            continue;
        }

        // Below us is either land or water... Test left and right.
        int left = -1, right = -1;

        for (int x = j - 1; x > 0 && left < 0; x--) {
            if (grid[i + 1][x] == '~' || grid[i + 1][x] == '#') {
                if (grid[i][x] == '#') // Hit the wall
                    left = x + 1;
                else {
                    grid[i][x] = '|';
                    image[i][x] = png::index_pixel(2);
                }
            }
            else {
                if (grid[i + 1][x + 1] == '#') // Overflow
                    for (int y = i; y <= mx && grid[y][x] == '.'; y++)
                        source.emplace(y, x);
                break;
            }
        }

        for (int x = j + 1; x < 1000 && right < 0; x++) {
            if (grid[i + 1][x] == '~' || grid[i + 1][x] == '#') {
                if (grid[i][x] == '#') // Hit the wall
                    right = x - 1;
                else {
                    grid[i][x] = '|';
                    image[i][x] = png::index_pixel(2);
                }
            }
            else {
                if (grid[i + 1][x - 1] == '#') // Overflow
                    for (int y = i; y <= mx && grid[y][x] == '.'; y++)
                        source.emplace(y, x);
                break;
            }
        }

        if (left > 0 && right > 0)
            for (int x = left; x <= right; x++) {
                grid[i][x] = '~';
                image[i][x] = png::index_pixel(3);
            }
    }

    image.write("output.png");

    long result1 = 0, result2 = 0;
    for (int i = mn; i <= mx; i++)
        for (int j = 0; j < 1000; j++)
            if (grid[i][j] == '~')
                result1++;
            else if (grid[i][j] == '|')
                result2++;
    cout << (result1 + result2) << ' ' << result1 << endl;

    return 0;
}

