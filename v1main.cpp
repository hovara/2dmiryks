#include <raylib.h>
#include <iostream>
#include <vector>

struct Entity {
    Rectangle rec;
    Color color;
    bool is_moving;
};

std::vector<Rectangle> world;

bool operator==(const Rectangle& lhs, const Rectangle& rhs) {
    if (lhs.x == rhs.x && lhs.y == rhs.y && lhs.width == rhs.width &&
        lhs.height == rhs.height) return true;
    else return false;
}

const size_t WORLD_SIZE = 10;

const int SPEED = 100;
const int JUMP_SPEED = 10;

const int TERMINAL_VELOCITY = 5;
const int BASE_VELOCITY = 0.5;
float velocity = 0.8;

void move_entity(Entity& entity, const int key, float amount) {
    const Rectangle old_rec = entity.rec;
    switch(key) {
    case KEY_LEFT: entity.rec.x -= SPEED * amount; break;
    case KEY_RIGHT: entity.rec.x += SPEED * amount; break;
    case KEY_UP: entity.rec.y -= JUMP_SPEED * SPEED * amount; break;
    case KEY_DOWN: entity.rec.y += SPEED * amount; break;
    default: break;
    }

    for (size_t i = 0; i < world.size(); i++) {
        if (CheckCollisionRecs(entity.rec, world[i])) {
            entity.rec = old_rec;
            entity.is_moving = false;
        }
    }
}

void apply_gravitation(Entity& entity) {
    velocity += 0.8;
    move_entity(entity, KEY_DOWN, GetFrameTime() * velocity);
    if (entity.is_moving == false) velocity = BASE_VELOCITY;
}

void move_player(Entity& player, const std::vector<Rectangle> world) {
    if (IsKeyDown(KEY_LEFT)) move_entity(player, KEY_LEFT, GetFrameTime());
    if (IsKeyDown(KEY_RIGHT)) move_entity(player, KEY_RIGHT, GetFrameTime());
    if (IsKeyDown(KEY_UP) && !player.is_moving) move_entity(player, KEY_UP, GetFrameTime());
    if (IsKeyDown(KEY_DOWN)) move_entity(player, KEY_DOWN, GetFrameTime());
    apply_gravitation(player);
}

void draw_world(const std::vector<Rectangle> world) {
    for (size_t i = 0; i < world.size(); i++) {
        DrawRectangleRec(world[i], Color{120, 120, 120, 255});
    }
}

void place_block() {
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        Vector2 mpos = {0, 0};
        mpos = GetMousePosition();
        Rectangle new_rec = {mpos.x, mpos.y, 10, 10};
        world.push_back(new_rec);
    }
}

int main(void)
{
    const int SCREENWIDTH = 800;
    const int SCREENHEIGHT = 400;

    const int PLAYER_SIZE = 20;
    Entity player = {
        Rectangle{SCREENWIDTH / 2, SCREENHEIGHT / 2, PLAYER_SIZE, PLAYER_SIZE},
        Color{255, 0, 0, 255},
        .is_moving = true
    };

    world.push_back({0, 350, 800, 40});

    InitWindow(SCREENWIDTH, SCREENHEIGHT, "2dgame");
    SetTargetFPS(60);
    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        move_player(player, world);
        place_block();
        BeginDrawing();
            ClearBackground(BLACK);
            draw_world(world);
            DrawRectangleRec(player.rec, player.color);
        EndDrawing();
    }
    CloseWindow();
    return 0;
}