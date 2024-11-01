#include "pipe.hpp"

Pipe::Pipe() : Speed(-2), Position({0, 0}), Velocity({Speed, 0}) {}

Pipe::~Pipe() {
  UnloadTexture(Pipe::TopSprite);
  UnloadTexture(Pipe::BottomSprite);
}

void Pipe::Update() {
  Pipe::Position.x += Pipe::Velocity.x;
  Pipe::Position.y += Pipe::Velocity.y;

  Pipe::TopCollider.x = Pipe::Position.x;
  Pipe::TopCollider.y = Pipe::Position.y - 400;

  Pipe::ScoreCollider.x = Pipe::Position.x + Pipe::TopSprite.width / 2.0,
  Pipe::ScoreCollider.y = Pipe::Position.y - 400 + Pipe::TopSprite.height;

  Pipe::BottomCollider.x = Pipe::Position.x;
  Pipe::BottomCollider.y = Pipe::Position.y;
}

void Pipe::Draw() {
  Rectangle sourceTop = {0, 0, (float)Pipe::TopSprite.width,
                         (float)Pipe::TopSprite.height};

  Rectangle destTop = {Pipe::Position.x + 26, Pipe::Position.y - 400,
                       (float)Pipe::TopSprite.width,
                       (float)Pipe::TopSprite.height * 1.4f};

  Vector2 originTop = {(float)Pipe::TopSprite.width / 2,
                       (float)Pipe::TopSprite.height};

  Rectangle sourceBottom = {0, 0, (float)Pipe::BottomSprite.width,
                            (float)Pipe::BottomSprite.height};

  Rectangle destBottom = {Pipe::Position.x, Pipe::Position.y,
                          (float)Pipe::BottomSprite.width,
                          (float)Pipe::BottomSprite.height * 1.4f};

  Vector2 originBottom = {0, 0};

  DrawTexturePro(Pipe::BottomSprite, sourceBottom, destBottom, originBottom, 0,
                 WHITE);

  DrawTexturePro(Pipe::TopSprite, sourceTop, destTop, originTop, 180, WHITE);
}

void Pipe::SetTextures(const char *spritePath) {
  Pipe::TopSprite = LoadTexture(spritePath);
  Pipe::BottomSprite = LoadTexture(spritePath);
  SetTextureFilter(Pipe::TopSprite, TEXTURE_FILTER_POINT);
  SetTextureFilter(Pipe::BottomSprite, TEXTURE_FILTER_POINT);

  Pipe::TopCollider = {Pipe::Position.x, Pipe::Position.y,
                       (float)Pipe::TopSprite.width,
                       (float)Pipe::TopSprite.height};

  Pipe::BottomCollider = {Pipe::Position.x, Pipe::Position.y,
                          (float)Pipe::BottomSprite.width,
                          (float)Pipe::BottomSprite.height};

  Pipe::ScoreCollider = {Pipe::Position.x, Pipe::Position.y, 5, 80};
}

void Pipe::UnloadTextures() {
  UnloadTexture(Pipe::TopSprite);
  UnloadTexture(Pipe::BottomSprite);
}

void Pipe::RandomizeHeight(float windowHeigth) {
  float randomHeight = GetRandomValue(100, windowHeigth - 100);

  Pipe::Position.y = randomHeight;
}
