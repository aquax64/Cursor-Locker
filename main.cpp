#include <windows.h>
#include <iostream>
#include <string>

HWND tarkov = NULL;
char *f = "EscapeFromTarkov"; // name of window
char *title;

// Callback function to enumerate all windows and print their titles
BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam) {
    char windowTitle[256];
    if (GetWindowTextA(hwnd, windowTitle, sizeof(windowTitle))) {
        if (IsWindowVisible(hwnd)) {
            std::cout << "Window: " << windowTitle << std::endl;
            if (strcmp(f, windowTitle) == 0) {
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
        RECT pos;
        GetCursorPos(LPPOINT(&pos));
        GetWindowRect(hwnd, &rect);
        if (IsWindowVisible(hwnd)) {
            std:: cout << "Cursor pos: " << pos.left << " " << pos.top << " " << pos.right << " " << pos.bottom << std::endl; // Works
            if (pos.left > rect.right) { // Under certain circumstances you would clip the right-left instead just left-right and top and bottom as well if they had other moniters
                SetCursorPos(rect.right / 2, rect.bottom / 2);
            } /*else if (rect.bottom < pos.bottom || rect.top < pos.top || rect.left < pos.left || rect.right < pos.right) {
                SetCursorPos(rect.bottom / 2, rect.top / 2);
            }*/

            std::cout << "Clipping cursor to: " << rect.top << "," << rect.bottom << "," << rect.left  << "," << rect.right << std::endl;

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
    HWND hwnd = GetWindowByTitle();
    // hwnd = tarkov;

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

        // Sleep(10);  // Check every 100 milliseconds

        if (!IsWindow(hwnd)) {
            std::cout << "Window closed!" << std::endl;
            ReleaseCursor();  // Ensure cursor lock is released when window is closed
            break;  // Exit the loop since the window no longer exists
        }
    }

    return 0;
}



