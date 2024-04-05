#include <cmath>
#include <iostream>
#include <raylib.h>
#include <raymath.h>
#include <vector>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 400;
const float GRAVITY = 800;
const float FRICTION = 10;

struct {
  Vector2 pos = {SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2};
  Vector2 velocity = Vector2Zero();
  bool is_on_floor = false;

  const Vector2 SIZE = {40, 40};
  const float SPEED = 300;
} player;

std::vector<Rectangle> world;

void setup(void) {
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "2dgame");
  // SetTargetFPS(690);
  world.push_back({0, SCREEN_HEIGHT - 30, SCREEN_WIDTH, 30});
  world.push_back({100, 200, 30, 800});
  world.push_back({400, 100, 100, 30});
}

// better collision FULL STEP / COARSE STEP
void player_move_and_collide(void) {
  const float delta = GetFrameTime();
  Vector2 amount = Vector2Scale(player.velocity, delta);

  float old_x = player.pos.x;
  player.pos.x += amount.x;
  Rectangle body = {player.pos.x, player.pos.y, player.SIZE.x, player.SIZE.y};
  for (size_t i = 0; i < world.size(); i++)
    if (CheckCollisionRecs(body, world[i])) {
      player.pos.x = old_x;
      break;
    }

  bool y_dir = signbit(amount.y);
  if (y_dir)
    player.is_on_floor = false;

  float old_y = player.pos.y;
  player.pos.y += amount.y;
  body = {player.pos.x, player.pos.y, player.SIZE.x, player.SIZE.y};
  for (size_t i = 0; i < world.size(); i++)
    if (CheckCollisionRecs(body, world[i])) {
      if (!y_dir)
        player.is_on_floor = true;
      player.pos.y = old_y;
      break;
    }
}

void render(void) {
  BeginDrawing();
  ClearBackground(BLACK);
  for (size_t i = 0; i < world.size(); i++)
    DrawRectangleRec(world[i], GRAY);
  DrawRectangleV(player.pos, player.SIZE, RED);
  std::cout << player.is_on_floor << std::endl;
  DrawFPS(10, 10);
  EndDrawing();
}

void update(void) {
  // gravity
  if (!player.is_on_floor) {
    player.velocity.y += GRAVITY * GetFrameTime();
    if (player.velocity.y > 1000)
      player.velocity.y = 1000;
  }
  // vertical
  if (player.is_on_floor && IsKeyPressed(KEY_W))
    player.velocity.y = -player.SPEED * 2;
  // horizontal
  int horizontal = IsKeyDown(KEY_D) - IsKeyDown(KEY_A);
  if (horizontal)
    player.velocity.x = horizontal * player.SPEED;
  else {
    float friction = player.is_on_floor ? 15 : 0.5;
    player.velocity.x = Lerp(player.velocity.x, 0, friction * GetFrameTime());
  }

  player_move_and_collide();

  render();
}

void cleanup(void) { CloseWindow(); }

int main(void) {
  setup();
  while (!WindowShouldClose())
    update();
  cleanup();
}