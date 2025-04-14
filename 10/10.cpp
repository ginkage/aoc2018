#include "../lib.hpp"

int main() {
    vector<pair<vec, vec>> stars;
    const regex linerex("position=<(.*), (.*)> velocity=<(.*), (.*)>");
    while (true) {
        string s;
        getline(cin, s);
        if (!cin) break;

        smatch linematch;
        if (regex_match(s, linematch, linerex) && linematch.size() == 5)
            stars.emplace_back(
                    vec(stoi(linematch[1].str()), stoi(linematch[2].str())),
                    vec(stoi(linematch[3].str()), stoi(linematch[4].str())));
    }

    long last = 0;
    int frame = 0;
    for (int i = 1; i < 10000000; i++) {
        long sum = 0;
        for (auto &star : stars) {
            vec &pos = star.first;
            vec &speed = star.second;
            sum += abs(pos[0]) + abs(pos[1]);
            pos += speed;
        }

        if (frame > 0) {
            png::image<png::index_pixel> image;
            png::palette pal(2);
            pal[0] = png::color(255, 255, 255);
            pal[1] = png::color(0, 0, 0);
            image.set_palette(pal);
            image.resize(400, 400);

            for (png::uint_32 y = 0; y < image.get_height(); ++y)
                for (png::uint_32 x = 0; x < image.get_width(); ++x)
                    image[y][x] = png::index_pixel(0);

            for (auto &star : stars) {
                vec &pos = star.first;
                image[pos[1] + 50][pos[0] + 50] = png::index_pixel(1);
            }

            image.write("output" + to_string(frame) + ".png");
            if (frame == 9)
                cout << i << endl;
            if (--frame == 0)
                break;
        }

        if (last > 0 && sum > last && frame == 0)
            frame = 30;
        last = sum;
    }

    return 0;
}

