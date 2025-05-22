#include "board.h"
#include "moves.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define WINDOW_WIDTH 600
#define WINDOW_HEIGHT 600
#define BOARD_SIZE 8
#define SQUARE_SIZE (WINDOW_HEIGHT / BOARD_SIZE)

int is_button_clicked(Button* btn, int x, int y) {
    return (x >= btn->rect.x && x <= btn->rect.x + btn->rect.w &&
            y >= btn->rect.y && y <= btn->rect.y + btn->rect.h);
}
void draw_button(SDL_Renderer* renderer, TTF_Font* font, Button* btn) {
    SDL_Color color = btn->hovered ? btn->color_hover : btn->color_normal;
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 255);
    SDL_RenderFillRect(renderer, &btn->rect);

    SDL_Color textColor = {255, 255, 255, 255};
    SDL_Surface* surface = TTF_RenderText_Blended(font, btn->text, textColor);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

    int textW, textH;
    SDL_QueryTexture(texture, NULL, NULL, &textW, &textH);
    SDL_Rect textRect = {
        btn->rect.x + (btn->rect.w - textW) / 2,
        btn->rect.y + (btn->rect.h - textH) / 2,
        textW, textH
    };

    SDL_RenderCopy(renderer, texture, NULL, &textRect);

    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
}
void draw_name_input(GameState* state, TTF_Font* font, Button* nameBtn, const char* player_name) {
    // Draw the input box
    SDL_Color color = nameBtn->hovered ? nameBtn->color_hover : nameBtn->color_normal;
    SDL_SetRenderDrawColor(state->renderer, color.r, color.g, color.b, 255);
    SDL_RenderFillRect(state->renderer, &nameBtn->rect);

    // Determine what text to display
    const char* display_text = strlen(player_name) > 0 ? player_name : "Enter Name";
    SDL_Color text_color = {0, 0, 0, 255};  // black text

    // Get the width and height of the rendered text
    int textW = 0, textH = 0;
    TTF_SizeText(font, display_text, &textW, &textH);

    // Center the text in the input box
    int center_x = nameBtn->rect.x + (nameBtn->rect.w - textW) / 2;
    int center_y = nameBtn->rect.y + (nameBtn->rect.h - textH) / 2;

    SDL_Surface* surface = TTF_RenderText_Blended(font, display_text, text_color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(state->renderer, surface);

    SDL_Rect textRect = {
        center_x,
        center_y,
        textW, textH
    };
    SDL_RenderCopy(state->renderer, texture, NULL, &textRect);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}
void render_background(SDL_Renderer* renderer, const char* image_path) {
    SDL_Surface* surface = IMG_Load(image_path);
    if (!surface) {
        printf("Failed to load background image: %s\n", IMG_GetError());
        return;
    }
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    if (!texture) {
        printf("Failed to create background texture: %s\n", SDL_GetError());
        return;
    }
    SDL_RenderCopy(renderer, texture, NULL, NULL); // Stretch to fill the window
    SDL_DestroyTexture(texture);
}

void menu(GameState* state,SOCKET* socked_fd, bool is_server, App_State* app_state, Button* startBtn, Button* exitBtn, Button* nameBtn) {
    // Menu screen => buttons: play, exit; a box for writing the name of the player
    // Play button: start the game
    // Exit button: close the game
    // Player name: write the name of the player
    
    SDL_Event event;
    TTF_Font* font = TTF_OpenFont("assets/fonts/arial.ttf", 28);
    TTF_Font* font_small = TTF_OpenFont("assets/fonts/arial.ttf", 18);
    if(!font || !font_small) {
        printf("Failed to load font: %s\n", TTF_GetError());
        return;
    }

    bool input_active = false;
    char player_name[MAX_NAME_LENGTH] = "";
    while (*app_state == MENU) {
        // Check for events
        while (SDL_PollEvent(&event)) {
            int x = event.button.x;
            int y = event.button.y;
            // Check if the mouse is over the buttons
            startBtn->hovered = is_button_clicked(startBtn, x, y);
            exitBtn->hovered = is_button_clicked(exitBtn, x, y);
            nameBtn->hovered = is_button_clicked(nameBtn, x, y);
            if (event.type == SDL_QUIT) {
                *app_state = EXIT_APP;
                return;
            } else if(event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
                *app_state = EXIT_APP;
                return;
            } else if (event.type == SDL_MOUSEBUTTONDOWN) {
                // Check if the play button is clicked
                if(is_button_clicked(startBtn, x, y)){
                    *app_state = GAME;
                    // Set the player names based on the input
                    if(is_server){
                        strcpy(state->player.name_player_1, player_name);
                        state->player.is_ready_player_1 = true;
                    } else {
                        //strcpy(state->player.name_player_2, player_name);
                        send(*socked_fd, player_name, sizeof(player_name), 0);
                        state->player.is_ready_player_2 = true;
                    }
                    return;
                }
                // Check if the exit button is clicked
                else if(is_button_clicked(exitBtn, x, y)){
                    *app_state = EXIT_APP;
                    return;
                }
                else if(is_button_clicked(nameBtn, x, y)){ // Check if the name button is clicked
                    input_active = true;
                    SDL_StartTextInput();
                }
            } else if(event.type == SDL_TEXTINPUT && input_active){
                if(strlen(player_name) + strlen(event.text.text) < sizeof(player_name) - 1){
                    strcat(player_name, event.text.text);
                }
            } else if (event.type == SDL_KEYDOWN && input_active) {
                    if (event.key.keysym.sym == SDLK_BACKSPACE && strlen(player_name) > 0) {
                        player_name[strlen(player_name) - 1] = '\0';
                    } else if (event.key.keysym.sym == SDLK_RETURN) {
                        input_active = false;
                        SDL_StopTextInput();
                    }
                }
        }
        render_background(state->renderer, "assets/img/background/menu_background.jpg");
        draw_name_input(state, font_small, nameBtn, player_name);
        draw_button(state->renderer, font, exitBtn);
        draw_button(state->renderer, font, startBtn);
        
        SDL_RenderPresent(state->renderer);
        SDL_Delay(16); // ~60 FPS
    }
    TTF_CloseFont(font);
    TTF_CloseFont(font_small);
    return;
}

// Inițializare joc
void init_game(GameState* state, Button* startBtn, Button* exitBtn, Button* nameBtn) {
    state->window = SDL_CreateWindow("Chess", SDL_WINDOWPOS_CENTERED, 
                                   SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, 
                                   WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    state->renderer = SDL_CreateRenderer(state->window, -1, SDL_RENDERER_ACCELERATED);
    
    // Inițializare tablă
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            state->board[i][j] = (Piece){EMPTY, WHITE};
        }
    }
    
    // Configurare piese inițiale
    state->board[0][0] = (Piece){ROOK, BLACK};
    state->board[0][1] = (Piece){KNIGHT, BLACK};
    state->board[0][2] = (Piece){BISHOP, BLACK};
    state->board[0][3] = (Piece){QUEEN, BLACK};
    state->board[0][4] = (Piece){KING, BLACK};
    state->board[0][5] = (Piece){BISHOP, BLACK};
    state->board[0][6] = (Piece){KNIGHT, BLACK};
    state->board[0][7] = (Piece){ROOK, BLACK};
    
    for (int i = 0; i < BOARD_SIZE; i++) {
        state->board[1][i] = (Piece){PAWN, BLACK};
        state->board[6][i] = (Piece){PAWN, WHITE};
    }
    
    state->board[7][0] = (Piece){ROOK, WHITE};
    state->board[7][1] = (Piece){KNIGHT, WHITE};
    state->board[7][2] = (Piece){BISHOP, WHITE};
    state->board[7][3] = (Piece){QUEEN, WHITE};
    state->board[7][4] = (Piece){KING, WHITE};
    state->board[7][5] = (Piece){BISHOP, WHITE};
    state->board[7][6] = (Piece){KNIGHT, WHITE};
    state->board[7][7] = (Piece){ROOK, WHITE};
    
    state->is_white_turn = true;
    state->selected_x = -1;
    state->selected_y = -1;

    // Inițializare jucători
    state->player.is_ready_player_1 = false;
    state->player.is_ready_player_2 = false;
    strcpy(state->player.name_player_1, "Guest");
    strcpy(state->player.name_player_2, "Guest");

    // Inițializare butoane
    startBtn->rect = (SDL_Rect){200, 200, 200, 50};
    startBtn->color_normal = (SDL_Color){0, 128, 0, 255}; // verde
    startBtn->color_hover = (SDL_Color){0, 180, 0, 255}; // verde deschis
    startBtn->text = "Start Game";
    startBtn->hovered = false;

    exitBtn->rect = (SDL_Rect){200, 300, 200, 50};
    exitBtn->color_normal = (SDL_Color){255, 0, 0, 255}; // roșu
    exitBtn->color_hover = (SDL_Color){255, 100, 100, 255}; // roșu deschis
    exitBtn->text = "Exit";
    exitBtn->hovered = false;

    nameBtn->rect = (SDL_Rect){150, 100, 300, 50};
    nameBtn->color_normal = (SDL_Color){192, 190, 190, 255}; // gri
    nameBtn->color_hover =  (SDL_Color){216, 214, 214, 255}; // 
    nameBtn->text = "Enter Name";
    nameBtn->hovered = false;
}

//Randare piesă
void render_piece(GameState* state, const char* piece, int x, int y) {
    SDL_Surface* surface = IMG_Load(piece);
    if (!surface) {
        printf("Failed to load image: %s\n", IMG_GetError());
        return;
    }
    
    SDL_Texture* texture = SDL_CreateTextureFromSurface(state->renderer, surface);
    if (!texture) {
        printf("Failed to create texture: %s\n", SDL_GetError());
        SDL_FreeSurface(surface);
        return;
    }
    
    SDL_Rect dest_rect = {x * SQUARE_SIZE + 10, y * SQUARE_SIZE + 10, 
                        SQUARE_SIZE - 20, SQUARE_SIZE - 20};
    SDL_RenderCopy(state->renderer, texture, NULL, &dest_rect);
    
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
}

// Randare tablă
void render_board(GameState* state) {
    SDL_SetRenderDrawColor(state->renderer, 255, 255, 255, 255);
    SDL_RenderClear(state->renderer);
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            SDL_Rect rect = {j * SQUARE_SIZE, i * SQUARE_SIZE, 
                           SQUARE_SIZE, SQUARE_SIZE};
            
            // Desenare pătrate
            if ((i + j) % 2 == 0) {
                SDL_SetRenderDrawColor(state->renderer, 235,236,208,255);//alb
            } else {
                SDL_SetRenderDrawColor(state->renderer, 115,149,82,255); //negru
            }
            SDL_RenderFillRect(state->renderer, &rect);
            
            // Desenare selecție
            if (i == state->selected_y && j == state->selected_x) {
                SDL_SetRenderDrawColor(state->renderer, 101, 101, 101, 100);
                SDL_RenderFillRect(state->renderer, &rect);
            }
          
            // Desenare piese (text simplu)
            if (state->board[i][j].type != EMPTY) {
                char piece[100] = ""; // Initializează un buffer pentru piesă
                if(state->board[i][j].color == WHITE){
                    switch (state->board[i][j].type) {
                        case PAWN:strncpy(piece, "assets/img/pieces/white/pawn.png", 100); break;
                        case KNIGHT: strncpy(piece, "assets/img/pieces/white/knight.png", 100); break;
                        case BISHOP: strncpy(piece, "assets/img/pieces/white/bishop.png", 100); break;
                        case ROOK: strncpy(piece, "assets/img/pieces/white/rook.png", 100); break;
                        case QUEEN: strncpy(piece, "assets/img/pieces/white/queen.png", 100); break;
                        case KING: strncpy(piece, "assets/img/pieces/white/king.png", 100); break;
                        default: break;
                }
                }
                else{
                    switch (state->board[i][j].type) {
                        case PAWN: strncpy(piece, "assets/img/pieces/black/pawn.png", 100); break;
                        case KNIGHT: strncpy(piece, "assets/img/pieces/black/knight.png", 100); break;
                        case BISHOP: strncpy(piece, "assets/img/pieces/black/bishop.png", 100); break;
                        case ROOK: strncpy(piece, "assets/img/pieces/black/rook.png", 100); break;
                        case QUEEN: strncpy(piece, "assets/img/pieces/black/queen.png", 100); break;
                        case KING: strncpy(piece, "assets/img/pieces/black/king.png", 100); break;
                        default: break;
                    }
                }
                render_piece(state, piece, j, i);
            }
        }
    }
    SDL_RenderPresent(state->renderer);
}

