#include <windows.h>
#include <iostream>
#include <string>

HWND tarkov = NULL; // handle for window
std::string f; // name of window
char *title; // name of window found

// Callback function to enumerate all windows and print their titles
BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam) {
    char windowTitle[256];
    if (GetWindowTextA(hwnd, windowTitle, sizeof(windowTitle))) {
        if (IsWindowVisible(hwnd)) {
            // std::cout << "Window: " << windowTitle << std::endl;
            if (strcmp(f.c_str(), windowTitle) == 0) {
                std::cout << "Window Found!" << std::endl;
                title = windowTitle;
                tarkov = hwnd;
            }
        }
    }
    return TRUE;  // Continue enumerating
}

// Function to get window handle by window title
HWND GetWindowByTitle() {
    EnumWindows(EnumWindowsProc, 0);  // Enumerate all windows and list their titles
    return tarkov;
}

// Function to lock cursor within a window's client area
void LockCursorToWindow(HWND hwnd) {
    if (hwnd != NULL) {
        RECT rect;
        RECT local;
        RECT pos;
        GetCursorPos(LPPOINT(&pos));
        GetWindowRect(hwnd, &rect);
        GetClientRect(hwnd, &local);
        if (IsWindowVisible(hwnd)) {
            // std:: cout << "Cursor pos: " << pos.left << ", " << pos.top << std::endl; // Works
            if (pos.left > rect.right || pos.left < rect.left || pos.top > rect.bottom || pos.top < rect.top) { // Detect if the cursor is beyond the rectangular border of the window
                SetCursorPos(rect.right - local.right / 2, rect.bottom - local.bottom / 2); // Set cursor position in the middle of the window
            }

            // std::cout << "Local: " << local.top << "," << local.bottom << "," << local.left  << "," << local.right << std::endl; // For debugging

            // std::cout << "Clipping cursor to: " << rect.top << "," << rect.bottom << "," << rect.left  << "," << rect.right << std::endl; // For debugging

        } else {
            std::cout << "HWND is NULL, can't lock cursor!" << std::endl;
        }
    }
}

// Function to release the cursor lock
void ReleaseCursor() {
    ClipCursor(NULL);  // Release the cursor lock
}

int main() {
    std::cout << "Enter name of window to be locked: ";
    std::getline(std::cin, f);

    std::cout << std::endl;

    HWND hwnd = GetWindowByTitle();

    if (hwnd == NULL) {
        std::cout << "Window not found!" << std::endl;
        return 1;
    }

    std::cout << "Monitoring window for cursor lock..." << std::endl;



    while (true) {
        HWND activeWindow = GetForegroundWindow();  // Get the currently active window

        if (activeWindow == hwnd) {
            // If the game window is active, lock the cursor to it
            LockCursorToWindow(hwnd);
        } else {
            // If another window is active, release the cursor
            ReleaseCursor();
        }

        Sleep(10);  // Check every 10 milliseconds

        if (!IsWindow(hwnd)) {
            std::cout << "Window closed!" << std::endl;
            ReleaseCursor();  // Ensure cursor lock is released when window is closed
            break;  // Exit the loop since the window no longer exists
        }
    }

    return 0;
}



