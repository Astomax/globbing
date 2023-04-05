#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <random>
#include <vector>
#include <math.h>
#include <array>
#include <thread>
#include <chrono>
#include <cmath>
#include <cstdlib>
#include <stdio.h>



using namespace std::chrono;


float color_count = 10000;
float rainbow_colors[500][3] = { { 0, 0, 0 } };

const float glow = 5.4f;
const int window_size = 600;
const int glob_count = 8;
const float speed = 4;
const float PI = 3.14159265359f;

float points[window_size * window_size] = { 0 };

const int dis_tot = window_size * window_size * 2;
float dis_look_up[dis_tot] = { 0 };


void FillRainbowColors() {

    int filling[6][2] = {
        { 1, 0 },
        { 0, 1 },
        { 2, 0 },
        { 1, 1 },
        { 0, 0 },
        { 2, 1}
    };

    float start_colors[6][3] = {
        { 1, 0, 0 },
        { 1, 1, 0 },
        { 0, 1, 0 },
        { 0, 1, 1 },
        { 0, 0, 1 },
        { 1, 0, 1 }
    };

    for (int i = 0; i < 6; i++) {

        float r = start_colors[i][0];
        float g = start_colors[i][1];
        float b = start_colors[i][2];

        bool removing = false;
        if (filling[i][1] == 1) {
            removing = true;
        }

        for (int i2 = 0; i2 < color_count / 6; i2++) {
            int index = (i * color_count / 6) + i2;

            switch (filling[i][0]) {
            case 0:
                if (removing) {
                    r -= 1.0f / (color_count / 6);
                    break;
                }
                r += 1.0f / (color_count / 6);
                break;

            case 1:
                if (removing) {
                    g -= 1.0f / (color_count / 6);
                    break;
                }
                g += 1.0f / (color_count / 6);
                break;

            case 2:
                if (removing) {
                    b -= 1.0f / (color_count / 6);
                    break;
                }
                b += 1.0f / (color_count / 6);
                break;
            }

            rainbow_colors[index][0] = r;
            rainbow_colors[index][1] = g;
            rainbow_colors[index][2] = b;

        }
    }
}

int RandomMinPlus() {
    if (rand() % 2 == 0) {
        return -1;
    }
    return 1;
}

float RandomFloat(float Max) {
    return static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / Max));
}

struct Glob {
    float radius = 40;
    float vel = speed;
    float angle = RandomFloat(2 * PI);
    float x = float(rand() % window_size);
    float y = float(rand() % window_size);

    void Move() {
        x += sin(angle) * vel;
        y += cos(angle) * vel;

        if (x < 0) {
            vel *= -1;
            angle += PI * 0.5f;
            x = 0;
        }

        if (y < 0) {
            vel *= -1;
            angle += PI * 0.5f;
            y = 0;
        }

        if (x >= window_size) {
            vel *= -1;
            angle += PI * 0.5f;
            x = window_size - 1;
        }

        if (y >= window_size) {
            vel *= -1;
            angle += PI * 0.5f;
            y = window_size - 1;
        }

    }

};





Glob globs[glob_count] = { Glob() };

void Draw(int x_start, int x_end) {

    for (int x = x_start; x < x_end; x++) {
        for (int y = 0; y < window_size; y++) {
            float sum = 0;

            for (Glob glob : globs) {
                int X = (glob.x - x);
                int Y = (glob.y - y);


                float distance = dis_look_up[(X* X) + (Y * Y)] * glob_count;
                if (distance <= 0) {
                    distance = 1.0f;
                }
                sum += glow * glob.radius / distance;
            }

            points[(x * window_size) + y] = sum;

        }
    }

}


int main(void)
{


    std::srand(time(NULL));

    for (int i = 0; i < glob_count; i++) {
        globs[i] = Glob();
    }

    GLFWwindow* window;

    for (int i = 0; i < dis_tot; i++) {
        dis_look_up[i] = std::sqrt(i);
    }


    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(window_size, window_size, "enge dingen", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);


    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    Glob A = Glob();
    unsigned int numThreads = std::thread::hardware_concurrency();

    std::cout << numThreads << "\n";

    gluOrtho2D(0.0, window_size, window_size, 0.0);
    FillRainbowColors();
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {


        std::cout << "\a";
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        for (int i = 0; i < glob_count; i++) {
            globs[i].Move();
        }

         

        int t_count = 20;

        std::thread t1(Draw, (window_size / t_count) * 0, (window_size / t_count) * 1);
        std::thread t2(Draw, (window_size / t_count) * 1, (window_size / t_count) * 2);
        std::thread t3(Draw, (window_size / t_count) * 2, (window_size / t_count) * 3);
        std::thread t4(Draw, (window_size / t_count) * 3, (window_size / t_count) * 4);
        std::thread t5(Draw, (window_size / t_count) * 4, (window_size / t_count) * 5);
        std::thread t6(Draw, (window_size / t_count) * 5, (window_size / t_count) * 6);
        std::thread t7(Draw, (window_size / t_count) * 6, (window_size / t_count) * 7);
        std::thread t8(Draw, (window_size / t_count) * 7, (window_size / t_count) * 8);
        std::thread t9(Draw, (window_size / t_count) * 8, (window_size / t_count) * 9);

        std::thread t10(Draw, (window_size / t_count) * 9, (window_size / t_count) * 10);
        std::thread t11(Draw, (window_size / t_count) * 10, (window_size / t_count) * 11);
        std::thread t12(Draw, (window_size / t_count) * 11, (window_size / t_count) * 12);
        std::thread t13(Draw, (window_size / t_count) * 12, (window_size / t_count) * 13);
        std::thread t14(Draw, (window_size / t_count) * 13, (window_size / t_count) * 14);
        std::thread t15(Draw, (window_size / t_count) * 14, (window_size / t_count) * 15);
        std::thread t16(Draw, (window_size / t_count) * 15, (window_size / t_count) * 16);
        std::thread t17(Draw, (window_size / t_count) * 16, (window_size / t_count) * 17);
        std::thread t18(Draw, (window_size / t_count) * 17, (window_size / t_count) * 18);
        std::thread t19(Draw, (window_size / t_count) * 18, (window_size / t_count) * 19);
        std::thread t20(Draw, (window_size / t_count) * 19, (window_size / t_count) * 20);




        t1.join();
        t2.join();
        t3.join();
        t4.join();
        t5.join();
        t6.join();
        t7.join();
        t8.join();
        t9.join();

        t10.join();
        t11.join();
        t12.join();
        t13.join();
        t14.join();
        t15.join();
        t16.join();
        t17.join();
        t18.join();
        t19.join();
        t20.join();

      



        glBegin(GL_POINTS);
        for (int x = 0; x < window_size; x++) {
            for (int y = 0; y < window_size; y++) {
                float lerp = points[(x * window_size) + y];

                if (lerp > 1) {
                    lerp -= std::floor(lerp);
                }

                int base = int(lerp * color_count);

                glColor3f(rainbow_colors[base][0], rainbow_colors[base][1], rainbow_colors[base][2]);
                glVertex2i(x, y);
            }
        }




        glEnd();

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
