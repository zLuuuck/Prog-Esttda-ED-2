#include "menu.h"
#include "../utils/utils.h"
#include "../game/game.h"
#include "../global.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

// Botões do menu principal
Button play_button, history_button, credits_button, exit_button;
Button start_button, back_button;
Button resume_button, pause_exit_button;

// Input do jogador
char player_input[50] = "";
bool input_active = false;

void init_menu()
{
    // Menu principal - centralizado
    int center_x = SCREEN_WIDTH / 2 - 100;
    int center_y = SCREEN_HEIGHT / 2 - 100;

    play_button = (Button){{center_x, center_y, 200, 50}, "Jogar", false};
    history_button = (Button){{center_x, center_y + 70, 200, 50}, "Histórico", false};
    credits_button = (Button){{center_x, center_y + 140, 200, 50}, "Creditos", false};
    exit_button = (Button){{center_x, center_y + 210, 200, 50}, "Sair", false};

    // Menu de input do jogador
    start_button = (Button){{center_x, center_y + 150, 200, 50}, "Iniciar", false};
    back_button = (Button){{center_x, center_y + 210, 200, 50}, "Voltar", false};

    // Menu de pausa
    resume_button = (Button){{center_x, center_y, 200, 50}, "Continuar", false};
    pause_exit_button = (Button){{center_x, center_y + 70, 200, 50}, "Sair", false};
}
void handle_menu_events(SDL_Event *e)
{
    int mouse_x, mouse_y;
    SDL_GetMouseState(&mouse_x, &mouse_y);

    // Atualiza estado de hover dos botões
    play_button.hovered = is_point_in_rect(mouse_x, mouse_y, play_button.rect);
    history_button.hovered = is_point_in_rect(mouse_x, mouse_y, history_button.rect);
    credits_button.hovered = is_point_in_rect(mouse_x, mouse_y, credits_button.rect);
    exit_button.hovered = is_point_in_rect(mouse_x, mouse_y, exit_button.rect);

    if (e->type == SDL_MOUSEBUTTONDOWN)
    {
        if (play_button.hovered)
        {
            // Se já temos dados salvos, vai direto para o jogo
            if (player_data_exists() && strlen(current_player_stats.name) > 0)
            {
                game.is_new_player = false;
                game.current_state = GAME_ACTIVE;
                start_game();
            }
            else
            {
                // Novo jogador
                game.current_state = MENU_PLAYER_INPUT;
                strcpy(player_input, "");
            }
        }
        else if (history_button.hovered)
        {
            game.current_state = HISTORY;
        }
        else if (credits_button.hovered)
        {
            game.current_state = CREDITS;
        }
        else if (exit_button.hovered)
        {
            game.running = false;
        }
    }
}

void handle_player_input_events(SDL_Event *e)
{
    int mouse_x, mouse_y;
    SDL_GetMouseState(&mouse_x, &mouse_y);

    // Atualiza estado de hover dos botões
    start_button.hovered = is_point_in_rect(mouse_x, mouse_y, start_button.rect);
    back_button.hovered = is_point_in_rect(mouse_x, mouse_y, back_button.rect);

    SDL_Rect input_rect = {SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 - 50, 200, 40};
    input_active = is_point_in_rect(mouse_x, mouse_y, input_rect);

    if (e->type == SDL_MOUSEBUTTONDOWN)
    {
        if (start_button.hovered && strlen(player_input) > 0)
        {
            strcpy(game.player_name, player_input);
            strcpy(current_player_stats.name, player_input);
            game.is_new_player = true;
            game.current_state = GAME_ACTIVE;
            start_game();
        }
        else if (back_button.hovered)
        {
            game.current_state = MENU_MAIN;
        }
    }

    if (e->type == SDL_KEYDOWN && input_active)
    {
        if (e->key.keysym.sym == SDLK_BACKSPACE && strlen(player_input) > 0)
        {
            player_input[strlen(player_input) - 1] = '\0';
        }
        else if (e->key.keysym.sym == SDLK_RETURN)
        {
            if (strlen(player_input) > 0)
            {
                strcpy(game.player_name, player_input);
                strcpy(current_player_stats.name, player_input);
                game.is_new_player = true;
                game.current_state = GAME_ACTIVE;
                start_game();
            }
        }
    }

    if (e->type == SDL_TEXTINPUT && input_active && strlen(player_input) < 49)
    {
        strcat(player_input, e->text.text);
    }
}

void handle_pause_events(SDL_Event *e)
{
    int mouse_x, mouse_y;
    SDL_GetMouseState(&mouse_x, &mouse_y);

    resume_button.hovered = is_point_in_rect(mouse_x, mouse_y, resume_button.rect);
    pause_exit_button.hovered = is_point_in_rect(mouse_x, mouse_y, pause_exit_button.rect);

    if (e->type == SDL_MOUSEBUTTONDOWN)
    {
        if (resume_button.hovered)
        {
            game.current_state = GAME_ACTIVE;
        }
        else if (pause_exit_button.hovered)
        {
            game.current_state = MENU_MAIN;
            reset_current_game();
        }
    }

    if (e->type == SDL_KEYDOWN && e->key.keysym.sym == SDLK_p)
    {
        game.current_state = GAME_ACTIVE;
    }
}

void render_menu()
{
    // Desenha título centralizado
    int title_width = 300;
    int title_x = (SCREEN_WIDTH - title_width) / 2;
    draw_text("TETRIS DOS GURI", title_x, 100, 48, (SDL_Color){255, 255, 255, 255});

    // Saudação personalizada
    if (strlen(game.player_name) > 0)
    {
        char hello[100];
        snprintf(hello, sizeof(hello), "Ola, %s!", game.player_name);
        int hello_width = strlen(hello) * 12;
        int hello_x = (SCREEN_WIDTH - hello_width) / 2;
        draw_text(hello, hello_x, 170, 24, (SDL_Color){200, 200, 200, 255});
    }
    else
    {
        draw_text("Ola, Jogador!", (SCREEN_WIDTH - 150) / 2, 170, 24, (SDL_Color){200, 200, 200, 255});
    }

    // Desenha botões
    draw_button(&play_button);
    draw_button(&history_button);
    draw_button(&credits_button);
    draw_button(&exit_button);
}

void render_player_input()
{
    // Desenha título
    draw_text("DIGITE SEU NOME", (SCREEN_WIDTH - 200) / 2, 100, 32, (SDL_Color){255, 255, 255, 255});

    // Desenha input field
    SDL_Rect input_rect = {SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 - 50, 200, 40};
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawRect(renderer, &input_rect);

    if (input_active)
    {
        SDL_SetRenderDrawColor(renderer, 100, 100, 255, 255);
        SDL_RenderDrawRect(renderer, &input_rect);
    }

    // Desenha texto do input
    if (strlen(player_input) > 0)
    {
        int text_width = strlen(player_input) * 12;
        int text_x = (SCREEN_WIDTH - text_width) / 2;
        draw_text(player_input, text_x, SCREEN_HEIGHT / 2 - 45, 24, (SDL_Color){255, 255, 255, 255});
    }
    else
    {
        draw_text("Digite seu nome...", SCREEN_WIDTH / 2 - 90, SCREEN_HEIGHT / 2 - 45, 24, (SDL_Color){128, 128, 128, 255});
    }

    // Desenha botões
    draw_button(&start_button);
    draw_button(&back_button);
}

void render_pause_menu()
{
    // Overlay escuro
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 200);
    SDL_Rect overlay = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
    SDL_RenderFillRect(renderer, &overlay);

    // Título
    draw_text("JOGO PAUSADO", (SCREEN_WIDTH - 200) / 2, 100, 32, (SDL_Color){255, 255, 255, 255});

    // Botões
    draw_button(&resume_button);
    draw_button(&pause_exit_button);

    // Aviso
    draw_text("Caso decida sair, seu progresso nao sera salvo.", (SCREEN_WIDTH - 400) / 2, 400, 16, (SDL_Color){255, 100, 100, 255});
}

void cleanup_menu()
{
    // Limpeza específica do menu, se necessário
}