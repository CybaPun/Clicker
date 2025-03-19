#include <Windows.h>
#include <iostream>
#include <thread>
#include <chrono>
#include <atomic>
#include <conio.h>

using namespace std;

// 全局控制变量
atomic<bool> running(false);
const int SWING_RANGE = 1; // 摆动幅度（像素）

// 光标摆动函数
void cursorSwing(DWORD fullCycleTime) {
    DWORD halfCycle = fullCycleTime / 2;
    int direction = 1;

    while (running) {
        INPUT input = {0};
        input.type = INPUT_MOUSE;
        input.mi.dwFlags = MOUSEEVENTF_MOVE;
        input.mi.dx = SWING_RANGE * direction;
        
        SendInput(1, &input, sizeof(INPUT));
        direction *= -1;
        
        this_thread::sleep_for(chrono::milliseconds(halfCycle));
    }
}

int main() {
    DWORD interval;

    interval = static_cast<DWORD>(500);
    
    cout << "光标摆动启动成功! 按空格键以停止...\n";

    running = true;
    thread worker(cursorSwing, interval);

    // 控制循环
    while (running) {
        if (GetAsyncKeyState(VK_SPACE) & 0x8000) {
            running = false;
        }
        this_thread::sleep_for(50ms);
    }

    worker.join();
    cout << "\n程序结束\n";
    return 0;
}