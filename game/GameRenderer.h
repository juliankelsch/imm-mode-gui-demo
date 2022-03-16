#ifndef PROCEDURALWORLD_GAMERENDERER_H
#define PROCEDURALWORLD_GAMERENDERER_H

#include "Game.h"

void RenderGame(u32 FramebufferWidth, u32 FramebufferHeight, game_state* GameState);
void RendererShutdown(render_state* RenderState);

#endif //PROCEDURALWORLD_GAMERENDERER_H
