#include "splashkit.h"
#include <iostream>
#include <cctype>
#include <string>

using namespace std;

// constants
const int WINDOW_W = 800;
const int WINDOW_H = 600;
const double TARGET_RADIUS = 35;
const double BAR_W = 500;
const double BAR_H = 24;
const double BAR_X = (WINDOW_W - BAR_W) / 2.0;
const double BAR_Y = 40;

// check if string is an int
bool is_integer_str(const string &s)
{
    if (s.empty()) return false;
    size_t i = 0;
    if (s[0] == '+' || s[0] == '-') i = 1;
    if (i >= s.size()) return false;
    for (; i < s.size(); ++i)
        if (!isdigit(static_cast<unsigned char>(s[i]))) return false;
    return true;
}

// read int from console
int read_int_validated(const string &prompt)
{
    while (true)
    {
        cout << prompt;
        string s;
        if (!getline(cin, s)) return 0;
        if (is_integer_str(s))
        {
            try { return stoi(s); }
            catch (...) {}
        }
        cout << "Please enter a whole number" << endl;
    }
}

// random double between min and max
double rnd_between(double min_val, double max_val)
{
    return min_val + rnd() * (max_val - min_val);
}

// make a circle at random spot
circle random_target()
{
    double x = rnd_between(TARGET_RADIUS + 5, WINDOW_W - TARGET_RADIUS - 5);
    double y = rnd_between(TARGET_RADIUS + 120, WINDOW_H - TARGET_RADIUS - 5);
    return circle_at(x, y, TARGET_RADIUS);
}

// draw progress bar
void draw_health_bar(double x, double y, double w, double h, double ratio)
{
    if (ratio < 0) ratio = 0;
    if (ratio > 1) ratio = 1;
    fill_rectangle(COLOR_GRAY, x, y, w, h);
    fill_rectangle(COLOR_LIME_GREEN, x, y, w * ratio, h);
    draw_rectangle(COLOR_BLACK, x, y, w, h);
}

// check if clicked inside target
bool target_hit_this_frame(const circle &target)
{
    if (mouse_clicked(LEFT_BUTTON))
    {
        point_2d m = mouse_position();
        return point_in_circle(m, target);
    }
    return false;
}

// draw game stuff
void draw_game(int initial_targets, int remaining, const circle &target)
{
    clear_screen(COLOR_WHITE);
    draw_text("Welcome to target click.", COLOR_BLACK, 20, 10);
    draw_text("Click the circle " + to_string(initial_targets) + " times to end the game.",
              COLOR_BLACK, 20, 70);
    draw_text("Targets remaining: " + to_string(remaining), COLOR_BLACK, 20, 100);

    double ratio = (initial_targets > 0) ? (remaining * 1.0 / initial_targets) : 0.0;
    draw_health_bar(BAR_X, BAR_Y, BAR_W, BAR_H, ratio);

    fill_circle(COLOR_RED, target);
    draw_circle(COLOR_BLACK, target);
    refresh_screen(60);
}

int main()
{
    cout << "Welcome to target click." << endl;
    int target_total = read_int_validated("What is your target score: ");
    cout << "\nClick " << target_total << " targets to end the game." << endl;

    open_window("Target Click", WINDOW_W, WINDOW_H);

    int remaining = target_total;
    circle target = random_target();

    while (remaining > 0 && !quit_requested())
    {
        process_events();
        if (target_hit_this_frame(target))
        {
            remaining -= 1;
            if (remaining > 0) target = random_target();
        }
        draw_game(target_total, remaining, target);
    }

    if (!quit_requested())
    {
        clear_screen(COLOR_WHITE);
        if (remaining == 0)
            draw_text("Great job! You hit all targets!", COLOR_BLACK, 220, WINDOW_H / 2.0 - 10);
        else
            draw_text("Goodbye!", COLOR_BLACK, 360, WINDOW_H / 2.0 - 10);
        refresh_screen();
        delay(1500);
    }
    return 0;
}
