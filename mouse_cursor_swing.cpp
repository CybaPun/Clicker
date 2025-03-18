#include <Windows.h>
#include <iostream>
#include <thread>
#include <chrono>
#include <atomic>
#include <conio.h>

using namespace std;

atomic<bool> running(false);

// 光标摆动函数，接收一个DWORD类型的参数fullCycleTime，表示光标摆动的完整周期时间
// 接收一个int类型的参数swingType，表示摆动类型（1: 左右，2: 上下，3: 斜对角线）
void cursorSwing(DWORD fullCycleTime, int swingType ,int SWING_RANGE) {
    DWORD halfCycle = fullCycleTime / 2;
    int direction = 1;
    int verticalDirection = 1;

    while (running) {
        INPUT input = {0};
        input.type = INPUT_MOUSE;
        input.mi.dwFlags = MOUSEEVENTF_MOVE;

        switch (swingType) {
            case 1: // 左右摆动
                input.mi.dx = SWING_RANGE * direction;
                input.mi.dy = 0;
                break;
            case 2: // 上下摆动
                input.mi.dx = 0;
                input.mi.dy = SWING_RANGE * verticalDirection;
                break;
            case 3: // 斜对角线摆动
                input.mi.dx = SWING_RANGE * direction;
                input.mi.dy = SWING_RANGE * verticalDirection;
                break;
            default:
                break;
        }

        SendInput(1, &input, sizeof(INPUT));

        if (swingType == 1) {
            direction *= -1;
        } else if (swingType == 2) {
            verticalDirection *= -1;
        } else if (swingType == 3) {
            direction *= -1;
            verticalDirection *= -1;
        }

        this_thread::sleep_for(chrono::milliseconds(halfCycle));
    }
}

int main() {
    double timeInput;
    char unit;
    DWORD interval;
    int swingType;
	int SWING_RANGE;
	
    cout << "\n鼠标光标摆动\n";
    cout << "1. 左右摆动\n2. 上下摆动\n3. 斜对角线摆动\n";
    cout << "请选择摆动类型:（输入操作类型前的序号）\n";
    cin >> swingType;

	cout << "\n请输入一个整数表示摆动的幅度（以像素为单位，建议幅度为5个像素）:\n";
	cin >> SWING_RANGE;

    cout << "\n请输入光标摆动周期，仅以毫秒或秒为单位 (例如. 500 ms 或 1 s): \n";
    cin >> timeInput >> unit;
	
    if (unit == 's' || unit == 'S') {
        interval = static_cast<DWORD>(timeInput * 1000);
    } else {
        interval = static_cast<DWORD>(timeInput);
    }

    cout << "\n光标摆动启动成功! 按空格键以停止...\n";

    running = true;
    thread worker(cursorSwing, interval, swingType, SWING_RANGE);

    while (running) {
        if (GetAsyncKeyState(VK_SPACE) & 0x8000) {
            running = false;
        }
        this_thread::sleep_for(50ms);
    }

    worker.join();
    cout << "程序结束\n";
    return 0;
}    