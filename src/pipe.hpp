#pragma once

#include "raylib.h"

class Pipe {
public:
  float Speed;
  Vector2 Position;
  Vector2 Velocity;
  Texture2D TopSprite;
  Rectangle TopCollider;
  Texture2D BottomSprite;
  Rectangle BottomCollider;
  Rectangle ScoreCollider;

  Pipe();
  ~Pipe();

  void SetTextures(const char *spritePath);

  void Draw();

  void Update();

  void UnloadTextures();
  void RandomizeHeight(float windowHeight);
};
