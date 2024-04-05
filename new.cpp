#include <raylib.h>
#include <raymath.h>
#include <string>
#include <vector>

const int SCREENWIDTH = 800;
const int SCREENHEIGHT = 400;

const int PLAYER_SIZE = 40;

float dt = 0;

std::vector<Rectangle> world;

struct Entity {
    Vector2 velocity;
    Rectangle rec;
};

Vector2 get_move_input(Entity entity) {
    Vector2 velocity = {0, 0};
    if (IsKeyDown(KEY_LEFT)) velocity.x += -1;
    if (IsKeyDown(KEY_RIGHT)) velocity.x += 1;
    if (IsKeyDown(KEY_UP)) velocity.y += -1;
    if (!CheckCollisionRecs(entity.rec, world[0]))
        if (IsKeyDown(KEY_DOWN)) velocity.y += 1;
    return velocity;
}

Vector2 calc_velocity(Entity& target) {
    const float MAX_SPEED = 10;
    Vector2 in_v = get_move_input(target);
    in_v = Vector2Normalize(in_v);
    Vector2 desired_v = Vector2Scale(in_v, MAX_SPEED);
    Vector2 v_dif = Vector2Subtract(desired_v, target.velocity);
    Vector2 amount;
    if (!GetKeyPressed()) amount = Vector2Scale(v_dif, dt * 7);
    else amount = Vector2Scale(v_dif, dt);
    return Vector2Add(target.velocity, amount);
}

void apply_gravitation(Entity& entity) {
    const float GRAVITATIONAL_PULL = 0.1;
    Rectangle new_hitbox = entity.rec;
    new_hitbox.y += GRAVITATIONAL_PULL;
    if (CheckCollisionRecs(new_hitbox, world[0])) return;
    entity.velocity.y += GRAVITATIONAL_PULL;
}

void move_entity(Entity& entity) {
    const float MIN_VELOCITY = 0.1;
    entity.velocity = calc_velocity(entity);
    if ((Vector2Length(entity.velocity) < MIN_VELOCITY)
        && Vector2Length(entity.velocity) > -MIN_VELOCITY) {

        entity.velocity = Vector2Zero();
    }

    //apply_gravitation(entity);

    entity.rec.x += entity.velocity.x;
    entity.rec.y += entity.velocity.y;
}



int main(void)
{
    InitWindow(SCREENWIDTH, SCREENHEIGHT, "2dgame");
    SetTargetFPS(60);

    Entity player = {
        .velocity = Vector2{0, 0},
        .rec = (Rectangle){SCREENWIDTH / 2, SCREENHEIGHT / 2, PLAYER_SIZE, PLAYER_SIZE}
    };

    const int BLOCKWIDTH = 20;
    world.push_back({0, SCREENHEIGHT - BLOCKWIDTH, SCREENWIDTH, BLOCKWIDTH});

    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        dt = GetFrameTime();
        move_entity(player);
        //apply_gravitation(player);
        BeginDrawing();
            ClearBackground(BLACK);
            DrawRectangleRec(world[0], RED);
            DrawRectangleRec(player.rec, RED);
            //Vector2 in_v = get_move_input();
            //in_v = Vector2Normalize(in_v);
            std::string s = std::to_string(player.velocity.x) + " " + std::to_string(player.velocity.y);
            //std::string s = std::to_string(in_v.x) + " " + std::to_string(in_v.y);
            DrawText(s.c_str(), 10, 10, 12, YELLOW);
        EndDrawing();
    }
    CloseWindow();
}