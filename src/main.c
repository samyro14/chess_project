#define SDL_MAIN_HANDLED
//#define _WIN32_WINNT 0x0600
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <time.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#include "board.h"
#include "moves.h"
#include "socket_utils.h"
#define WINDOW_WIDTH 600
#define WINDOW_HEIGHT 600
#define BOARD_SIZE 8
#define SQUARE_SIZE (WINDOW_HEIGHT / BOARD_SIZE)


int main(int argc, char* argv[]) {
    
    FILE* results_file = fopen("results.txt", "a");
    if (results_file == NULL) {
        printf("Could not open results.txt for writing.\n");
        return 1;
    }
    // Socket setup
    SOCKET socket_fd = -1;
    bool is_server = false;
    if (argc >= 2 && strcmp(argv[1], "server") == 0) {
        int port = (argc >= 3) ? atoi(argv[2]) : 12345;
        SOCKET server_fd = setup_server(port);
        struct sockaddr_in client_addr;
        int addrlen = sizeof(client_addr);
        printf("Waiting for client to connect...\n");
        socket_fd = accept(server_fd, (struct sockaddr*)&client_addr, &addrlen);
        if (socket_fd == INVALID_SOCKET) {
            printf("Accept failed.\n");
            cleanup_winsock();
            return 1;
        }
        printf("Client connected.\n");
        is_server = true;
        closesocket(server_fd);
    } else if (argc >= 4 && strcmp(argv[1], "client") == 0) {
        const char* ip = argv[2];
        int port = atoi(argv[3]);
        socket_fd = setup_client(ip, port);
        is_server = false;
    } else {
        printf("Usage:\n  %s server [port]\n  %s client <ip> <port>\n", argv[0], argv[0]);
        return 1;
    }

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL initialization failed: %s\n", SDL_GetError());
        return 1;
    }
    if(IMG_Init(IMG_INIT_PNG) == 0) {
        printf("SDL_image initialization failed: %s\n", IMG_GetError());
        return 1;
    }
    if (TTF_Init() == -1) {
    printf("SDL_ttf initialization failed: %s\n", TTF_GetError());
    return 1;
    }
    GameState state;
    App_State app_state = MENU;
    Button startBtn, exitBtn, nameBtn;
    init_game(&state, &startBtn, &exitBtn, &nameBtn);
    menu(&state, &socket_fd, is_server, &app_state, &startBtn, &exitBtn, &nameBtn);

    // Set up the player names
    if (is_server) {
        char buffer[MAX_NAME_LENGTH] = "";
        int received = recv(socket_fd, buffer, sizeof(buffer), 0);
        if (received > 0) {
            buffer[received] = '\0'; // Ensure null-terminated
            strcpy(state.player.name_player_2, buffer);
            state.player.is_ready_player_2 = true;
        } else {
            strcpy(state.player.name_player_2, "Guest");
        }
    }
    bool running = true;
    SDL_Event event;
    u_long mode = 1;
    ioctlsocket(socket_fd, FIONBIO, &mode); // Set socket to non-blocking mode

    while (running && app_state == GAME) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            } else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
                running = false;
            } else if ((is_server && state.is_white_turn) || (!is_server && !state.is_white_turn)) {
                if (event.type == SDL_MOUSEBUTTONDOWN) {
                    int x = event.button.x / SQUARE_SIZE;
                    int y = event.button.y / SQUARE_SIZE;
                    if (state.selected_x == -1) {
                        if (state.board[y][x].type != EMPTY &&
                            ((state.is_white_turn && state.board[y][x].color == WHITE) ||
                            (!state.is_white_turn && state.board[y][x].color == BLACK))) {
                            state.selected_x = x;
                            state.selected_y = y;
                        }
                    } else {
                        handle_move(&state, state.selected_x, state.selected_y, x, y, results_file, is_server);
                        // Send move to opponent
                        char move[5];
                        snprintf(move, sizeof(move), "%d%d%d%d", state.selected_x, state.selected_y, x, y);
                        send(socket_fd, move, sizeof(move), 0);
                        state.selected_x = -1;
                        state.selected_y = -1;
                    }
                }
            }
      } 
        // If it's not your turn, check for opponent's move (non-blocking)
        if ((!is_server && state.is_white_turn) || (is_server && !state.is_white_turn)) {
            char opponent_move[5] = {0};
            int received = recv(socket_fd, opponent_move, sizeof(opponent_move), 0);
            if (received == 5) {
                int opp_from_x = opponent_move[0] - '0';
                int opp_from_y = opponent_move[1] - '0';
                int opp_to_x = opponent_move[2] - '0';
                int opp_to_y = opponent_move[3] - '0';
                handle_move(&state, opp_from_x, opp_from_y, opp_to_x, opp_to_y, results_file, is_server);
            }
        }
        
        render_board(&state);
        SDL_Delay(10);
    }
    fclose(results_file);
    closesocket(socket_fd);
    cleanup_winsock();
    SDL_DestroyRenderer(state.renderer);
    SDL_DestroyWindow(state.window);
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
    
    return 0;
}