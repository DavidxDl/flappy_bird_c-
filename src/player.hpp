#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "raylib.h"
#include "stdio.h"

class Player {
public:
  Vector2 Position;
  Vector2 Velocity;
  float MaxJumpSpeed;
  float JumpSpeed;
  Rectangle Collision;
  Texture2D Sprite;

  Player(float x, float y);
  ~Player();

  void SetSprite(const char *spritePath);

  void UnloadTextures();

  void Update();

  void ApplyForce(const Vector2 force);
};

#endif
