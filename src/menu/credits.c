#include "credits.h"
#include "../utils/utils.h"
#include "../global.h"
#include <stdio.h>

// Botão para voltar ao menu
Button back_button_credits;

void render_credits()
{
    int center_x = SCREEN_WIDTH / 2;
    int y = 20;

    SDL_Color white = {255, 255, 255, 255};
    SDL_Color yellow = {255, 255, 0, 255};
    SDL_Color blue = {100, 150, 255, 255};
    SDL_Color gray = {200, 200, 200, 255};

    // Título - Agora centralizado automaticamente
    draw_centered_text("CREDITOS", y, 36, yellow);
    y += 70;

    // Linha decorativa - Já está centralizada
    SDL_SetRenderDrawColor(renderer, 100, 100, 255, 255);
    SDL_Rect line = {center_x - 150, y, 300, 2};
    SDL_RenderFillRect(renderer, &line);
    y += 30;

    // Seção de criadores - Centralizada
    draw_centered_text("DESENVOLVEDORES", y, 28, blue);
    y += 50;

    // Nomes - Mantive alinhados à esquerda para melhor legibilidade, mas você pode centralizar se quiser
    draw_text("Lucas Toterol Rodrigues", center_x - 130, y, 24, white); // Ajuste o offset se necessário
    y += 35;
    draw_text("Leonardo de Paula Teixeira", center_x - 145, y, 24, white);
    y += 60;

    // Linha decorativa - Já está centralizada
    SDL_SetRenderDrawColor(renderer, 100, 100, 255, 255);
    line.y = y;
    SDL_RenderFillRect(renderer, &line);
    y += 30;

    // Seção de orientação - Centralizada
    draw_centered_text("ORIENTACAO", y, 28, blue);
    y += 50;

    // Textos - Mantive alinhados à esquerda
    draw_text("Professor: Diogenes Cago Furlan", center_x - 200, y, 24, white);
    y += 35;
    draw_text("Disciplina: Programacao estruturada", center_x - 220, y, 24, white);
    y += 60;

    // Linha decorativa - Já está centralizada
    SDL_SetRenderDrawColor(renderer, 100, 100, 255, 255);
    line.y = y;
    SDL_RenderFillRect(renderer, &line);
    y += 30;

    // Agradecimento - Centralizada
    draw_centered_text("AGRADECIMENTOS", y, 28, blue);
    y += 50;

    draw_centered_text("Obrigado por jogar!", y, 24, yellow);
    y += 40;
    draw_centered_text("Esperamos que tenha se divertido!", y, 20, gray);
    y += 60;

    // Botão de voltar - Já está centralizado
    back_button_credits.rect = (SDL_Rect){center_x - 100, y, 200, 50};
    back_button_credits.text = "Voltar ao Menu";
    back_button_credits.hovered = false;
    draw_button(&back_button_credits);
}

void handle_credits_events(SDL_Event *e)
{
    // Código permanece o mesmo
    int mouse_x, mouse_y;
    SDL_GetMouseState(&mouse_x, &mouse_y);

    back_button_credits.hovered = is_point_in_rect(mouse_x, mouse_y, back_button_credits.rect);

    if (e->type == SDL_MOUSEBUTTONDOWN)
    {
        if (back_button_credits.hovered)
        {
            game.current_state = MENU_MAIN;
        }
    }

    if (e->type == SDL_KEYDOWN && e->key.keysym.sym == SDLK_ESCAPE)
    {
        game.current_state = MENU_MAIN;
    }
}