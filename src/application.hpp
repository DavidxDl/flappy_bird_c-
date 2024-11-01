#pragma once

#include <iostream>

namespace FlappyBird {

struct ApplicationSpecification {
  int Width = 280;
  int Height = 570;
  std::string Title = "Flappy Bird";
};

class Application {
public:
  Application(
      const ApplicationSpecification &spec = ApplicationSpecification());

  ~Application();

  void Run();

private:
  ApplicationSpecification m_Specification;
};
} // namespace FlappyBird
