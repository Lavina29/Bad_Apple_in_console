#include <windows.h>
#include <stdio.h>
#include <mmsystem.h>   // для звука (необязательно)

#pragma comment(lib, "winmm.lib")

#define W 80
#define H 24
#define FPS 15

int main() {
    system("mode con: cols=80 lines=25");
    system("cls");

    // скрываем курсор
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO ci = {1, FALSE};
    SetConsoleCursorInfo(h, &ci);

    FILE *f = fopen("frames.raw", "rb");
    if (!f) {
        printf("Не найден frames.raw\n");
        return 1;
    }

    // необязательно: звук, если есть badapple.wav
    // (из видео: ffmpeg -i badapple.mp4 badapple.wav)
    PlaySound("badapple.wav", NULL, SND_FILENAME | SND_ASYNC);

    unsigned char frame[W * H];
    char out[(W + 1) * H + 1];
    COORD home = {0, 0};

    ULONGLONG start = GetTickCount64();
    int n = 0;

    while (fread(frame, 1, W * H, f) == W * H) {
        int p = 0;
        for (int y = 0; y < H; y++) {
            for (int x = 0; x < W; x++) {
                out[p++] = frame[y * W + x] > 127 ? '*' : ' ';
            }
            if (y < H - 1) out[p++] = '\n';
        }
        out[p] = '\0';

        SetConsoleCursorPosition(h, home);
        fputs(out, stdout);

        // ждём, пока наступит время следующего кадра
        n++;
        ULONGLONG target = start + (ULONGLONG)n * 1000 / FPS;
        while (GetTickCount64() < target) Sleep(1);
    }

    fclose(f);
    SetConsoleCursorPosition(h, home);
    return 0;
}