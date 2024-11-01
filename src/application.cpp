#include "application.hpp"
#include "pipe.hpp"
#include "player.hpp"

#include "format"
#include "raylib.h"
#include "resource_dir.h" // utility header for SearchAndSetResourceDir
#include <iostream>

namespace FlappyBird {

enum STATE {
  MENU,
  PLAY,
  GAME_OVER

};

Application::Application(const ApplicationSpecification &spec)
    : m_Specification(spec) {}
} // namespace FlappyBird

FlappyBird::Application::~Application() {}
void FlappyBird::Application::Run() {
  {

    const Vector2 GRAVITY = {0, 1};

    SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);

    // Create the window and OpenGL context
    InitWindow(m_Specification.Width, m_Specification.Height,
               m_Specification.Title
                   .c_str()); // Utility function from resource_dir.h to find
                              // the resources folder and set it as the current
                              // working directory so we can load from it
    SearchAndSetResourceDir("resources");

    // Load a texture from the resources directory

    Player player(m_Specification.Width / 2.0, m_Specification.Height / 2.0);

    player.SetSprite(
        "../resources/flappy-bird-assets/sprites/yellowbird-midflap.png");

    int pipesAmmount = 2;
    Pipe pipes[pipesAmmount];

    for (int i = 0; i < pipesAmmount; i++) {
      pipes[i].Position.x = m_Specification.Width + (200 * i);
      pipes[i].RandomizeHeight(m_Specification.Height);
      pipes[i].SetTextures(
          "../resources/flappy-bird-assets/sprites/pipe-green.png");
    }

    Texture2D ground =
        LoadTexture("../resources/flappy-bird-assets/sprites/base.png");
    Texture2D background = LoadTexture(
        "../resources/flappy-bird-assets/sprites/background-day.png");
    float background_x = 0;
    float ground_x = 0;

    Texture2D GameOver =
        LoadTexture("../resources/flappy-bird-assets/sprites/gameover.png");

    SetTargetFPS(60);

    bool Collision = false;
    STATE gameState = MENU;
    int Score = 0;
    Texture2D numbers[10];
    bool CanScore = true;
    for (int i = 0; i < 10; i++) {
      numbers[i] = LoadTexture(
          std::format("../resources/flappy-bird-assets/sprites/{}.png", i)
              .c_str());
    }
    while (!WindowShouldClose()) {

      switch (gameState) {
      case MENU:
        if (IsKeyPressed(KEY_SPACE)) {
          gameState = PLAY;

          Score = 0;
          CanScore = true;
          player.Position.x = m_Specification.Width / 2.0;
          player.Position.y = m_Specification.Height / 2.0;
          player.Velocity = {0, 0};
          for (int i = 0; i < pipesAmmount; i++) {
            pipes[i].Position.x = m_Specification.Width + (200 * i);
            pipes[i].RandomizeHeight(m_Specification.Height);
            pipes[i].SetTextures(
                "../resources/flappy-bird-assets/sprites/pipe-green.png");
          }
          Collision = false;
        }

        for (int i = 0; i < pipesAmmount; i++) {
          pipes[i].Update();
          if (pipes[i].Position.x + pipes[i].TopCollider.width <= 0) {
            pipes[i].Position.x = m_Specification.Width + 150;
            pipes[i].RandomizeHeight(m_Specification.Height);
          }
        }
        background_x -= -pipes[0].Speed - 1;
        ground_x += pipes[0].Speed;

        if (background_x < 0 - m_Specification.Width) {
          background_x = 0;
        }
        if (ground_x < 0 - m_Specification.Width) {
          ground_x = 0;
        }
        BeginDrawing();

        // Setup the backbuffer for drawing (clear color and depth buffers)
        ClearBackground(BLUE);

        DrawTexture(background, background_x, 0, WHITE);
        DrawTexture(background, background_x + m_Specification.Width, 0, WHITE);

        for (int i = 0; i < pipesAmmount; i++) {
          pipes[i].Draw();
        }

        DrawTexture(ground, ground_x, m_Specification.Height - 70, WHITE);

        DrawTexture(ground, ground_x + m_Specification.Width,
                    m_Specification.Height - 70, WHITE);

        EndDrawing();

        break;

      case PLAY:
        if (IsKeyPressed(KEY_SPACE) && !Collision) {
          player.Velocity.y += player.JumpSpeed;
          if (player.Velocity.y <= player.MaxJumpSpeed)
            player.Velocity.y = player.MaxJumpSpeed;
        }

        player.ApplyForce(GRAVITY);
        //  drawing
        player.Update();

        for (int i = 0; i < pipesAmmount; i++) {
          pipes[i].Update();
          if (CheckCollisionRecs(player.Collision, pipes[i].TopCollider) ||
              CheckCollisionRecs(player.Collision, pipes[i].BottomCollider)) {
            std::cout << "Collide!!!" << std::endl;
            Collision = true;
          }
          if (CheckCollisionRecs(player.Collision, pipes[i].ScoreCollider) &&
              CanScore) {
            Score++;
            CanScore = false;
          }

          if (pipes[i].Position.x + pipes[i].TopCollider.width <= 0) {
            pipes[i].Position.x = m_Specification.Width + 150;
            pipes[i].RandomizeHeight(m_Specification.Height);
            CanScore = true;
          }
        }
        background_x -= -pipes[0].Speed - 1;
        if (background_x < 0 - m_Specification.Width) {
          background_x = 0;
        }
        if (player.Position.y >= m_Specification.Height - 90) {
          player.Position.y = m_Specification.Height - 90;
          player.Position.x += pipes[0].Speed;
          Collision = true;
        }
        BeginDrawing();

        // Setup the backbuffer for drawing (clear color and depth buffers)
        ClearBackground(BLUE);

        DrawTexture(background, background_x, 0, WHITE);
        DrawTexture(background, background_x + m_Specification.Width, 0, WHITE);

        for (int i = 0; i < pipesAmmount; i++) {
          pipes[i].Draw();
        }

        DrawTexture(ground, background_x, m_Specification.Height - 70, WHITE);

        DrawTexture(ground, background_x + m_Specification.Width,
                    m_Specification.Height - 70, WHITE);
        // draw some text using the default font
        DrawTexture(player.Sprite, player.Position.x, player.Position.y, WHITE);
        DrawTexture(numbers[Score % 10], 5, 5, WHITE);

        if (Collision)
          gameState = GAME_OVER;

        EndDrawing();

        break;

      case GAME_OVER:

        if (IsKeyPressed(KEY_SPACE)) {
          gameState = PLAY;

          Score = 0;
          CanScore = true;
          player.Position.x = m_Specification.Width / 2.0;
          player.Position.y = m_Specification.Height / 2.0;
          player.Velocity = {0, 0};
          for (int i = 0; i < pipesAmmount; i++) {
            pipes[i].Position.x = m_Specification.Width + (200 * i);
            pipes[i].RandomizeHeight(m_Specification.Height);
            pipes[i].SetTextures(
                "../resources/flappy-bird-assets/sprites/pipe-green.png");
          }
          Collision = false;
        }

        player.Update();
        player.ApplyForce(GRAVITY);

        for (int i = 0; i < pipesAmmount; i++) {
          pipes[i].Update();
          if (pipes[i].Position.x + pipes[i].TopCollider.width <= 0) {

            pipes[i].Position.x = m_Specification.Width + 150;
            pipes[i].RandomizeHeight(m_Specification.Height);
          }
        }
        background_x -= -pipes[0].Speed - 1;
        if (background_x < 0 - m_Specification.Width) {
          background_x = 0;
        }
        if (player.Position.y >= m_Specification.Height - 90) {
          player.Position.y = m_Specification.Height - 90;
          player.Position.x += pipes[0].Speed;
        }
        BeginDrawing();

        // Setup the backbuffer for drawing (clear color and depth buffers)
        ClearBackground(BLUE);

        DrawTexture(background, background_x, 0, WHITE);
        DrawTexture(background, background_x + m_Specification.Width, 0, WHITE);

        for (int i = 0; i < pipesAmmount; i++) {
          pipes[i].Draw();
          DrawRectangleLines(
              pipes[i].Position.x + pipes[i].TopSprite.width / 2.0,
              pipes[i].Position.y - 400 + pipes[i].TopSprite.height, 5, 80,
              RED);
        }

        DrawTexture(ground, background_x, m_Specification.Height - 70, WHITE);

        DrawTexture(ground, background_x + m_Specification.Width,
                    m_Specification.Height - 70, WHITE);
        // draw some text using the default font
        DrawTexture(player.Sprite, player.Position.x, player.Position.y, WHITE);
        DrawTexture(GameOver, m_Specification.Width / 2 - GameOver.width / 2,
                    m_Specification.Height / 2, WHITE);

        EndDrawing();
        break;
      }

      // end the frame and get ready for the next one  (display frame, poll
      // input, etc...)
    }

    player.UnloadTextures();
    for (int i = 0; i < pipesAmmount; i++) {
      pipes[i].UnloadTextures();
    }

    // destory the window and cleanup the OpenGL context
    CloseWindow();
  }
}
