#include "utils.h"
#include "../game/game.h"
#include "../menu/menu.h"
#include "../global.h"
#include <stdio.h>

// Variáveis para fontes
TTF_Font *font_small = NULL;
TTF_Font *font_medium = NULL;
TTF_Font *font_large = NULL;

void init_fonts()
{
    if (TTF_Init() == -1)
    {
        printf("Erro ao inicializar SDL_ttf: %s\n", TTF_GetError());
        return;
    }

    font_small = TTF_OpenFont("assets/fonts/OpenSans.ttf", 20);
    font_medium = TTF_OpenFont("assets/fonts/OpenSans.ttf", 24);
    font_large = TTF_OpenFont("assets/fonts/OpenSans.ttf", 32);

    if (!font_small || !font_medium || !font_large)
    {
        printf("Erro ao carregar fontes: %s\n", TTF_GetError());
        printf("Certifique-se de que a pasta assets/fonts existe com a fonte OpenSans.ttf\n");
    }
}

void cleanup_fonts()
{
    if (font_small)
        TTF_CloseFont(font_small);
    if (font_medium)
        TTF_CloseFont(font_medium);
    if (font_large)
        TTF_CloseFont(font_large);
    TTF_Quit();
}

void draw_text(const char *text, int x, int y, int size, SDL_Color color)
{
    TTF_Font *font_to_use;

    // Seleciona a fonte baseada no tamanho
    if (size <= 20)
        font_to_use = font_small;
    else if (size <= 24)
        font_to_use = font_medium;
    else
        font_to_use = font_large;

    if (!font_to_use)
    {
        // Fallback: desenha retângulos (como antes)
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
        int len = strlen(text);
        for (int i = 0; i < len; i++)
        {
            SDL_Rect char_rect = {x + i * (size / 2), y, size / 3, size};
            SDL_RenderFillRect(renderer, &char_rect);
        }
        return;
    }

    SDL_Surface *surface = TTF_RenderText_Solid(font_to_use, text, color);
    if (surface)
    {
        SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_Rect rect = {x, y, surface->w, surface->h};
        SDL_RenderCopy(renderer, texture, NULL, &rect);
        SDL_DestroyTexture(texture);
        SDL_FreeSurface(surface);
    }
}

void draw_button(Button *button)
{
    // Cor do botão baseada no estado
    if (button->hovered)
    {
        SDL_SetRenderDrawColor(renderer, 100, 100, 255, 255);
    }
    else
    {
        SDL_SetRenderDrawColor(renderer, 80, 80, 200, 255);
    }

    SDL_RenderFillRect(renderer, &button->rect);

    // Borda do botão
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawRect(renderer, &button->rect);

    // Texto do botão (centralizado)
    int text_width = strlen(button->text) * 12;
    int text_x = button->rect.x + (button->rect.w - text_width) / 2;
    int text_y = button->rect.y + (button->rect.h - 20) / 2;
    draw_text(button->text, text_x, text_y, 20, (SDL_Color){255, 255, 255, 255});
}

bool is_point_in_rect(int x, int y, SDL_Rect rect)
{
    return (x >= rect.x && x <= rect.x + rect.w &&
            y >= rect.y && y <= rect.y + rect.h);
}

void handle_events()
{
    SDL_Event e;
    while (SDL_PollEvent(&e))
    {
        if (e.type == SDL_QUIT)
        {
            game.running = false;
        }

        switch (game.current_state)
        {
        case MENU_MAIN:
            handle_menu_events(&e);
            break;
        case MENU_PLAYER_INPUT:
            handle_player_input_events(&e);
            break;
        case GAME_ACTIVE:
            handle_game_events(&e);
            break;
        case GAME_PAUSED:
            handle_pause_events(&e);
            break;
        case HISTORY:
            handle_history_events(&e); // Nova função
            break;
        case CREDITS:
            if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE)
            {
                game.current_state = MENU_MAIN;
            }
            break;
        }
    }
}

void update()
{
    if (game.current_state == GAME_ACTIVE)
    {
        update_game();
    }
}

void render()
{
    // Limpa a tela
    SDL_SetRenderDrawColor(renderer, 0x1E, 0x1E, 0x1E, 0xFF);
    SDL_RenderClear(renderer);

    // Renderiza de acordo com o estado
    switch (game.current_state)
    {
    case MENU_MAIN:
        render_menu();
        break;
    case MENU_PLAYER_INPUT:
        render_player_input();
        break;
    case GAME_ACTIVE:
        render_game();
        break;
    case GAME_PAUSED:
        render_game(); // Renderiza o jogo por baixo
        render_pause_menu();
        break;
    case HISTORY:
        render_history();
        break;
    case CREDITS:
        draw_text("CREDITOS - EM BREVE", (SCREEN_WIDTH - 250) / 2, 100, 32, (SDL_Color){255, 255, 255, 255});
        draw_text("Pressione ESC para voltar", (SCREEN_WIDTH - 250) / 2, 200, 20, (SDL_Color){255, 255, 255, 255});
        break;
    }

    SDL_RenderPresent(renderer);
}