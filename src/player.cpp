#include "player.hpp"
#include "iostream"

Player::Player(float x, float y)
    : Velocity({0, 0}), Position({x, y}), JumpSpeed(-14), MaxJumpSpeed(-16) {};

Player::~Player() {
  UnloadTexture(Player::Sprite);
  printf("unloaded texture: Player\n");
}

void Player::SetSprite(const char *spritePath) {
  Player::Sprite = LoadTexture(spritePath);

  std::cout << "Sprite Dimentions: " << Player::Sprite.width << ", "
            << Player::Sprite.height << std::endl;
  Player::Collision = {Player::Position.x, Player::Position.y,
                       (float)Player::Sprite.width,
                       (float)Player::Sprite.height};
  std::cout << "Collision Dimentions: " << Player::Collision.width << ", "
            << Player::Collision.height << std::endl;
}

void Player::UnloadTextures() { UnloadTexture(Player::Sprite); }

void Player::Update() {
  Player::Position.x += Player::Velocity.x;
  Player::Position.y += Player::Velocity.y;

  Player::Collision.x = Player::Position.x;
  Player::Collision.y = Player::Position.y;
}

void Player::ApplyForce(const Vector2 force) {
  Player::Velocity.x += force.x;
  Player::Velocity.y += force.y;
}
