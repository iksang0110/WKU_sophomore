
#include <windows.h>
#include <stdbool.h>
#include <stdio.h>
#include "game.h"
#include "map.h"
#include "player.h"
#include "raycaster.h"
#include "input.h"

/**
 * @brief FPS를 화면에 표시하기 위한 함수
 * @param game 게임 상태 구조체 포인터
 */
void DisplayFPS(GameState* game) {
    static int frameCount = 0;
    static double lastTime = 0;
    static double fps = 0;

    frameCount++;
    double currentTime = GetTime();

    // 1초마다 FPS 계산
    if (currentTime - lastTime > 1.0) {
        fps = frameCount / (currentTime - lastTime);
        frameCount = 0;
        lastTime = currentTime;

        char title[64];
        sprintf_s(title, sizeof(title), "Maze Game - FPS: %.2f", fps);
        SetWindowTextA(game->window, title);
    }
}

/**
 * @brief 게임 초기화 수행
 * @param game 게임 상태 구조체 포인터
 * @param hInstance 프로그램 인스턴스 핸들
 * @return 초기화 성공 여부
 */
bool GameStartup(GameState* game, HINSTANCE hInstance) {
    // 게임 상태 초기화
    if (!InitGame(game, hInstance)) {
        MessageBoxA(NULL, "Failed to initialize game", "Error", MB_ICONERROR);
        return false;
    }
    // 맵 생성
    if (!CreateDefaultMap(&game->map)) {
        MessageBoxA(NULL, "Failed to create map", "Error", MB_ICONERROR);
        return false;
    }
    // 레이캐스터 초기화
    if (!InitRaycaster(game)) {
        MessageBoxA(NULL, "Failed to initialize raycaster", "Error", MB_ICONERROR);
        return false;
    }

    return true;
}

/**
 * @brief 게임 리소스 정리
 * @param game 게임 상태 구조체 포인터
 */
void GameShutdown(GameState* game) {
    CleanupRaycaster(game);
    CleanupMap(&game->map);
    CleanupInput();
    CleanupGame(game);
}

/**
 * @brief 메시지 처리 함수
 */
bool ProcessMessages(void) {
    MSG msg;
    while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
        if (msg.message == WM_QUIT) {
            return false;
        }
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return true;
}

/**
 * @brief Windows 메인 진입점
 */
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);
    UNREFERENCED_PARAMETER(nCmdShow);

    GameState game = { 0 };
    if (!GameStartup(&game, hInstance)) {
        return 1;
    }

    // 입력 상태 초기화
    InputState inputState = { 0 };
    InitInput(&inputState);

    MSG msg = { 0 };  // msg 변수를 여기서 한 번만 선언

    // 메인 게임 루프
    while (game.isRunning) {
        // 메시지 처리
        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT) {
                game.isRunning = false;
                break;
            }
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        if (!game.isRunning) break;

        // 입력 처리
        UpdateInput(&inputState, &game);

        // 배경 지우기 (검은색)
        RECT rect;
        GetClientRect(game.window, &rect);
        HBRUSH blackBrush = CreateSolidBrush(RGB(0, 0, 0));
        FillRect(game.backBuffer, &rect, blackBrush);
        DeleteObject(blackBrush);

        // 테스트용 사각형 그리기 (플레이어 위치)
        HBRUSH whiteBrush = CreateSolidBrush(RGB(255, 255, 255));
        RECT testRect = {
            (int)(game.player.position.x * 20),  // 화면에서 더 크게 보이도록 위치 조정
            (int)(game.player.position.y * 20),
            (int)(game.player.position.x * 20) + 20,
            (int)(game.player.position.y * 20) + 20
        };
        FillRect(game.backBuffer, &testRect, whiteBrush);
        DeleteObject(whiteBrush);

        // 플레이어 위치 디버깅용 출력 추가
        char debugTitle[128];
        sprintf_s(debugTitle, sizeof(debugTitle),
            "Player Position: (%.2f, %.2f)",
            game.player.position.x,
            game.player.position.y);
        SetWindowTextA(game.window, debugTitle);

        // 백 버퍼를 화면에 그리기
        BitBlt(game.deviceContext, 0, 0, game.screenWidth, game.screenHeight,
            game.backBuffer, 0, 0, SRCCOPY);

        // FPS 표시
        DisplayFPS(&game);

        // 프레임 제한
        Sleep(1);
    }

    GameShutdown(&game);
    return (int)msg.wParam;
}

#ifdef _DEBUG
int main(void) {
    return WinMain(GetModuleHandle(NULL), NULL, GetCommandLine(), SW_SHOWNORMAL);
}
#endif