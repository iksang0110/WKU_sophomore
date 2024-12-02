// game.c
#include "game.h"
#include "input.h"
#include <stdlib.h>
#include <math.h>
#include <stdio.h>

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_CLOSE:
        PostQuitMessage(0);
        return 0;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
}

bool InitGame(GameState* game, HINSTANCE hInstance) {
    // 기본값 초기화
    game->screenWidth = SCREEN_WIDTH;
    game->screenHeight = SCREEN_HEIGHT;
    game->isRunning = true;
    game->lastTime = GetTime();
    game->deltaTime = 0.0;

    // 윈도우 클래스 등록
    WNDCLASSEXA wc = { 0 };
    wc.cbSize = sizeof(WNDCLASSEXA);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wc.lpszClassName = "GameClass";

    if (!RegisterClassExA(&wc)) {
        DWORD error = GetLastError();
        char msg[256];
        sprintf_s(msg, sizeof(msg), "RegisterClassEx failed: %d", error);
        MessageBoxA(NULL, msg, "Error", MB_OK);
        return false;
    }

    // 윈도우 생성
    game->window = CreateWindowExA(
        0,                      // 확장 스타일
        "GameClass",            // 클래스 이름
        "Game",                 // 윈도우 제목
        WS_OVERLAPPEDWINDOW,    // 스타일
        CW_USEDEFAULT,         // x 위치
        CW_USEDEFAULT,         // y 위치
        game->screenWidth,      // 너비
        game->screenHeight,     // 높이
        NULL,                   // 부모 윈도우
        NULL,                   // 메뉴
        hInstance,              // 인스턴스
        NULL                    // 추가 파라미터
    );

    if (!game->window) {
        DWORD error = GetLastError();
        char msg[256];
        sprintf_s(msg, sizeof(msg), "CreateWindowEx failed: %d", error);
        MessageBoxA(NULL, msg, "Error", MB_OK);
        return false;
    }

    // 디바이스 컨텍스트 가져오기
    game->deviceContext = GetDC(game->window);
    if (!game->deviceContext) {
        MessageBoxA(NULL, "Failed to get device context", "Error", MB_OK);
        return false;
    }

    // 백 버퍼 생성
    game->backBuffer = CreateCompatibleDC(game->deviceContext);
    if (!game->backBuffer) {
        MessageBoxA(NULL, "Failed to create back buffer", "Error", MB_OK);
        return false;
    }

    // 백 버퍼 비트맵 생성
    game->backBufferBitmap = CreateCompatibleBitmap(
        game->deviceContext,
        game->screenWidth,
        game->screenHeight
    );
    if (!game->backBufferBitmap) {
        MessageBoxA(NULL, "Failed to create back buffer bitmap", "Error", MB_OK);
        return false;
    }

    // 백 버퍼에 비트맵 선택
    HBITMAP oldBitmap = (HBITMAP)SelectObject(game->backBuffer, game->backBufferBitmap);
    if (!oldBitmap) {
        MessageBoxA(NULL, "Failed to select bitmap into back buffer", "Error", MB_OK);
        return false;
    }

    // Z-버퍼 할당
    game->zBuffer = (double*)malloc(sizeof(double) * game->screenWidth);
    if (!game->zBuffer) {
        MessageBoxA(NULL, "Failed to allocate Z-buffer", "Error", MB_OK);
        return false;
    }

    // 플레이어 초기화
    game->player.position = (Vector2D){ 22.0, 12.0 };
    game->player.direction = (Vector2D){ -1.0, 0.0 };
    game->player.plane = (Vector2D){ 0.0, 0.66 };
    game->player.moveSpeed = MOVE_SPEED;
    game->player.rotSpeed = ROT_SPEED;

    // 맵 초기화
    game->map.width = MAP_WIDTH;
    game->map.height = MAP_HEIGHT;
    game->map.data = (int*)malloc(sizeof(int) * MAP_WIDTH * MAP_HEIGHT);
    if (!game->map.data) {
        MessageBoxA(NULL, "Failed to allocate map data", "Error", MB_OK);
        return false;
    }

    // 창 보이기
    ShowWindow(game->window, SW_SHOW);
    UpdateWindow(game->window);

    // 초기 화면 지우기
    RECT rect;
    GetClientRect(game->window, &rect);
    HBRUSH blackBrush = CreateSolidBrush(RGB(0, 0, 0));
    FillRect(game->backBuffer, &rect, blackBrush);
    DeleteObject(blackBrush);

    return true;
}

void RunGameLoop(GameState* game) {
    MSG msg = { 0 };
    char titleBuffer[128];
    int frameCount = 0;
    double lastFPSTime = GetTime();

    // 입력 상태 초기화
    InputState input = { 0 };
    InitInput(&input);

    while (game->isRunning) {
        // 윈도우 메시지 처리
        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT) {
                game->isRunning = false;
                break;
            }
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        if (!game->isRunning) break;

        // 시간 업데이트
        double currentTime = GetTime();
        game->deltaTime = currentTime - game->lastTime;
        game->lastTime = currentTime;

        // 입력 처리
        UpdateInput(&input, game);

        // 게임 로직 업데이트
        UpdateGame(game);

        // 배경 지우기
        RECT rect;
        GetClientRect(game->window, &rect);
        HBRUSH blackBrush = CreateSolidBrush(RGB(0, 0, 0));
        FillRect(game->backBuffer, &rect, blackBrush);
        DeleteObject(blackBrush);

        // 테스트용 사각형 그리기
        HBRUSH whiteBrush = CreateSolidBrush(RGB(255, 255, 255));
        RECT testRect = {
            (int)game->player.position.x,  // 플레이어 위치를 기반으로 사각형 위치 설정
            (int)game->player.position.y,
            (int)game->player.position.x + 50,
            (int)game->player.position.y + 50
        };
        FillRect(game->backBuffer, &testRect, whiteBrush);
        DeleteObject(whiteBrush);

        // 백 버퍼를 화면에 그리기
        BitBlt(game->deviceContext, 0, 0, game->screenWidth, game->screenHeight,
            game->backBuffer, 0, 0, SRCCOPY);

        // FPS 계산 및 표시
        frameCount++;
        if (currentTime - lastFPSTime >= 1.0) {
            sprintf_s(titleBuffer, sizeof(titleBuffer),
                "Game - FPS: %d, Pos: (%.1f, %.1f)",
                frameCount,
                game->player.position.x,
                game->player.position.y);
            SetWindowTextA(game->window, titleBuffer);
            frameCount = 0;
            lastFPSTime = currentTime;
        }

        // 프레임 제한
        Sleep(1);
    }
}

void UpdateGame(GameState* game) {
    CalculateDeltaTime(game);
}

double GetTime(void) {
    LARGE_INTEGER frequency, counter;
    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&counter);
    return (double)counter.QuadPart / (double)frequency.QuadPart;
}

void CalculateDeltaTime(GameState* game) {
    double currentTime = GetTime();
    game->deltaTime = currentTime - game->lastTime;
    game->lastTime = currentTime;

    if (game->deltaTime > 0.016667f) {
        game->deltaTime = 0.016667f;
    }
}

void CleanupGame(GameState* game) {
    if (game->zBuffer) {
        free(game->zBuffer);
        game->zBuffer = NULL;
    }

    if (game->map.data) {
        free(game->map.data);
        game->map.data = NULL;
    }

    if (game->backBufferBitmap) {
        DeleteObject(game->backBufferBitmap);
        game->backBufferBitmap = NULL;
    }

    if (game->backBuffer) {
        DeleteDC(game->backBuffer);
        game->backBuffer = NULL;
    }

    if (game->deviceContext) {
        ReleaseDC(game->window, game->deviceContext);
        game->deviceContext = NULL;
    }

    if (game->window) {
        DestroyWindow(game->window);
        game->window = NULL;
    }
}