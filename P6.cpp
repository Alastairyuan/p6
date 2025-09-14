#include "splashkit.h"
using std::to_string;
using std::stod;

// tiny helper: read_string(prompt)
string read_string(const string &prompt)
{
    write(prompt);
    return read_line();
}

// constants
const int WINDOW_W = 640;
const int WINDOW_H = 480;
const double TARGET_RADIUS = 40;

// read an int with validation
int read_int_validated(string prompt)
{
    while (true)
    {
        string s = read_string(prompt);
        if (is_integer(s)) return convert_to_integer(s);
        write_line("Please enter a whole number");
    }
}

// draw one frame
void draw_frame(int total, int remaining, const circle &target)
{
    clear_screen(COLOR_WHITE);

    draw_text("Welcome to target click.", COLOR_BLACK, 20, 20);
    draw_text("Click the circle " + to_string(total) + " times to end the game.",
              COLOR_BLACK, 20, 60);
    draw_text("Targets remaining: " + to_string(remaining),
              COLOR_BLACK, 20, 100);

    fill_circle(COLOR_RED, target);
    draw_circle(COLOR_BLACK, target);

    refresh_screen(60);
}

// check click inside target
bool clicked_target_this_frame(const circle &target)
{
    if (mouse_clicked(LEFT_BUTTON))
    {
        point_2d m = mouse_position();
        return point_in_circle(m, target);
    }
    return false;
}

int main()
{
    write_line("Welcome to target click.");
    int target_total = read_int_validated("What is your target score: ");
    write_line("Click " + to_string(target_total) + " targets to end the game.");

    open_window("Target Click (Part 2 core)", WINDOW_W, WINDOW_H);

    int remaining = target_total;
    circle target = circle_at(WINDOW_W / 2.0, WINDOW_H / 2.0, TARGET_RADIUS);

    while (!quit_requested() && remaining > 0)
    {
        process_events();
        if (clicked_target_this_frame(target))
        {
            remaining -= 1;
        }
        draw_frame(target_total, remaining, target);
    }

    if (!quit_requested())
    {
        clear_screen(COLOR_WHITE);
        if (remaining == 0)
            draw_text("Done! Thanks for playing.", COLOR_BLACK, 200, WINDOW_H / 2.0);
        else
            draw_text("Goodbye!", COLOR_BLACK, 260, WINDOW_H / 2.0);
        refresh_screen();
        delay(1200);
    }
    return 0;
}