//
// Created by GengG on 2023/1/7.
//

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>
#include <windows.h>

struct vec2d{
    double x;
    double y;
};
typedef vec2d vec;
int main()
{
    // 一个模拟万有引力的程序
    // 空间中每个像素就是一个粒子，每个粒子的质量都是1
    // 每个粒子都受到其他粒子的引力
    using namespace std;
    
    int world_width = 50;
    int world_height = 50;
    vector<vector<int>> world_space(world_width, vector<int>(world_height, 0));
    vector<vector<int>> world_space_next(world_width, vector<int>(world_height, 0));
    // 粒子列表
    vector<vec> particles_position;
    // 粒子的质量
    vector<double> particles_mass;
    // 粒子的速度
    vector<vec> particles_speed;
    // 粒子的加速度
    vector<vec> particles_acceleration;
    // 粒子所受的引力
    vector<vec> particles_gravity;
    
    // 初始化粒子
    int particle_count = 3;
    for (int i = 0; i < particle_count; ++i) {
        vec pos;
        pos.x = rand() % world_width;
        pos.y = rand() % world_height;
        particles_position.push_back(pos);
        particles_mass.push_back(10000);
        vec speed;
        speed.x = 0;
        speed.y = 0;
        particles_speed.push_back(speed);
        vec acceleration;
        acceleration.x = 0;
        acceleration.y = 0;
        particles_acceleration.push_back(acceleration);
        vec gravity;
        gravity.x = 0;
        gravity.y = 0;
        particles_gravity.push_back(gravity);
    }
    
    // 开始模拟
    
    
    // 通过cmd绘制
    while (true) {
        // 清空屏幕
        system("cls");
        // 计算引力
        for (int i = 0; i < particle_count; ++i) {
            vec gravity;
            gravity.x = 0;
            gravity.y = 0;
            for (int j = 0; j < particle_count; ++j) {
                if (i == j) {
                    continue;
                }
                vec    pos1     = particles_position[i];
                vec    pos2     = particles_position[j];
                double distance = sqrt(pow(pos1.x - pos2.x, 2) + pow(pos1.y - pos2.y, 2));
                double force    = particles_mass[i] * particles_mass[j] / pow(distance, 2);
                vec    direction;
                direction.x     = pos2.x - pos1.x;
                direction.y     = pos2.y - pos1.y;
                double direction_length = sqrt(pow(direction.x, 2) + pow(direction.y, 2));
                direction.x /= direction_length;
                direction.y /= direction_length;
                gravity.x += force * direction.x;
                gravity.y += force * direction.y;
            }
            particles_gravity[i] = gravity;
        }
        // 计算加速度
        for (int i = 0; i < particle_count; ++i) {
            vec acceleration;
            acceleration.x = particles_gravity[i].x / particles_mass[i];
            acceleration.y = particles_gravity[i].y / particles_mass[i];
            particles_acceleration[i] = acceleration;
        }
        // 计算速度
        for (int i = 0; i < particle_count; ++i) {
            vec speed;
            speed.x = particles_speed[i].x + particles_acceleration[i].x;
            speed.y = particles_speed[i].y + particles_acceleration[i].y;
            particles_speed[i] = speed;
        }
        // 计算位置
        for (int i = 0; i < particle_count; ++i) {
            vec pos;
            pos.x = particles_position[i].x + particles_speed[i].x;
            pos.y = particles_position[i].y + particles_speed[i].y;
            particles_position[i] = pos;
            // 碰撞检测
            if (pos.x < 0) {
                pos.x = 0;
                particles_speed[i].x = -particles_speed[i].x;
            }
            if (pos.x >= world_width) {
                pos.x = world_width - 1;
                particles_speed[i].x = -particles_speed[i].x;
            }
            if (pos.y < 0) {
                pos.y = 0;
                particles_speed[i].y = -particles_speed[i].y;
            }
            if (pos.y >= world_height) {
                pos.y = world_height - 1;
                particles_speed[i].y = -particles_speed[i].y;
            }
        }
        // 绘制
        for (int i = 0; i < particle_count; ++i) {
            int x             = particles_position[i].x;
            int y             = particles_position[i].y;
            if (x < 0 || x >= world_width || y < 0 || y >= world_height) {
                continue;
            }
            world_space[x][y] = 1;
        }
        // 控制控制台x,y处的像素为白色
        HWND hwnd = GetConsoleWindow();
        HDC hdc = GetDC(hwnd);
        for (int i = 0; i < world_width; ++i) {
            for (int j = 0; j < world_height; ++j) {
                if (world_space[i][j] == 1) {
                    // 设置控制台像素颜色
                    //SetPixel(hdc, i, j, RGB(255, 255, 255));
                    
                    cout << "*";
                } else {
                    //SetPixel(hdc, i, j, RGB(0, 0, 0));
                    cout << " ";
                }
            }
            cout << endl;
        }
        world_space = world_space_next;
        Sleep(100);
    }
}
