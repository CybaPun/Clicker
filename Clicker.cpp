#include <Windows.h>
#include <conio.h>
#include <iostream>
#include <thread>
#include <atomic>

using namespace std;

atomic<bool> running(false);

void simulateClick(int choice, DWORD interval) {
    while (running) {
        if (choice == 1 || choice == 2) { // 左键或右键
            INPUT inputs[2] = {0};
            
            // 按下事件
            inputs[0].type = INPUT_MOUSE;
            inputs[0].mi.dwFlags = (choice == 1) ? MOUSEEVENTF_LEFTDOWN : MOUSEEVENTF_RIGHTDOWN;
            
            // 释放事件
            inputs[1].type = INPUT_MOUSE;
            inputs[1].mi.dwFlags = (choice == 1) ? MOUSEEVENTF_LEFTUP : MOUSEEVENTF_RIGHTUP;
            
            SendInput(2, inputs, sizeof(INPUT));
        }
        else if (choice == 3 || choice == 4) { // 滚轮
            INPUT input = {0};
            input.type = INPUT_MOUSE;
            input.mi.dwFlags = MOUSEEVENTF_WHEEL;
            input.mi.mouseData = (choice == 3) ? WHEEL_DELTA : -WHEEL_DELTA;
            
            SendInput(1, &input, sizeof(INPUT));
        }
        
        this_thread::sleep_for(chrono::milliseconds(interval));
    }
}

int main() {
    int choice;
    double timeInput;
    char unit;
    DWORD interval;

    cout << "鼠标连点器\n\n";
    cout << "1. 左键连点\n2. 右键连点\n3. 滚轮上滑\n4. 滚轮下滑\n";
    cout << "请选择操作类型:（输入要进行操作前面的序号）\n";
	cin >> choice;
    cout << "请输入时间间隔长度，仅以毫秒或秒为单位 (例如, 500 ms 或 1 s):\n";
    cin >> timeInput >> unit;

    // 转换时间单位为毫秒
    if (unit == 's' || unit == 'S') {
        interval = static_cast<DWORD>(timeInput * 1000);
    } else {
        interval = static_cast<DWORD>(timeInput);
    }

    cout << "\n鼠标连点器启动成功! 按空格键以结束运行...\n";

    running = true;
    thread worker(simulateClick, choice, interval);

    // 等待空格键按下
    while (running) {
        if (GetAsyncKeyState(VK_SPACE) & 0x8000) {
            running = false;
        }
        this_thread::sleep_for(50ms);
    }

    worker.join();
    cout << "\n程序已结束\n";
    return 0;
}