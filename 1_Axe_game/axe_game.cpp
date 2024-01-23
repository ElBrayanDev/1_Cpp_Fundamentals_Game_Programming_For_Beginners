#include "raylib.h"

int main()
{
    /*
    float root_beer{1.99};
    double cheese_burger{5.99};
    bool shouldHaveLunch{};
    */

    // Window dimensions

    auto width = 1900;
    auto height = 1050;
    int radius{100};
    int circle_x{radius};
    int circle_y{height / 2};
    int axe_x{width / 2};
    int axe_y{0};
    int axe_lenght{radius};

    int direction{10};

    int l_circle_x{circle_x - radius};
    int r_circle_x{circle_x + radius};
    int u_circle_y{circle_y - radius};
    int b_circle_y{circle_y + radius};

    int l_axe_x{axe_x};
    int r_axe_x{axe_x + axe_lenght};
    int u_axe_y{axe_y};
    int b_axe_y{axe_y + axe_lenght};

    bool collision{b_axe_y > +u_circle_y &&
                   u_axe_y <= b_circle_y &&
                   r_axe_x >= l_circle_x &&
                   l_axe_x <= r_circle_x};

    InitWindow(width, height, "Axe Game");

    SetTargetFPS(144);
    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(BLACK); // Clear background to avoid flickering

        if (collision)
        {
            DrawText("Game Over", (width / 2) - (50 * 2), (height / 2) - (50), 50, RED);
        }
        else
        {
            // # LOGIC

            // Update edges

            l_circle_x = circle_x - radius;
            r_circle_x = circle_x + radius;
            u_circle_y = circle_y - radius;
            b_circle_y = circle_y + radius;

            l_axe_x = axe_x;
            r_axe_x = axe_x + axe_lenght;
            u_axe_y = axe_y;
            b_axe_y = axe_y + axe_lenght;

            // update collision bool
            collision = b_axe_y > +u_circle_y &&
                        u_axe_y <= b_circle_y &&
                        r_axe_x >= l_circle_x &&
                        l_axe_x <= r_circle_x;

            DrawCircle(circle_x, circle_y, radius, RED);
            DrawRectangle(axe_x, axe_y, axe_lenght, axe_lenght, BLUE);

            axe_y += direction;

            if (axe_y > height - axe_lenght || axe_y < 0)
            {
                direction = -direction;
            }

            if (IsKeyDown(KEY_D) && circle_x <= width - axe_lenght)
            {
                circle_x = circle_x + 10;
            }
            if (IsKeyDown(KEY_A) && circle_x >= 0 + axe_lenght)
            {
                circle_x = circle_x - 10;
            }
        }

        EndDrawing();
    }
    CloseWindow();
}