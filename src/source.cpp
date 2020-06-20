#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

#include <string>
#include <vector>


struct gameObject {
    olc::vi2d size;
    olc::vf2d velocity;
    olc::vf2d position;
};

struct Collider
{
    std::string tag;

    olc::vf2d position;
    olc::vf2d size;

    void *refereceObject;
};

enum Side
{
  SIDE_NONE,

  SIDE_TOP,
  SIDE_LEFT,
  SIDE_BOTTOM,
  SIDE_RIGHT,
};

bool check_collision(Collider const &left, Collider const &right)
{
    return true
      && left.position.x + left.size.x >= right.position.x
      && left.position.x < right.position.x + right.size.x
      && left.position.y + left.size.y >= right.position.y
      && left.position.y < right.position.y + right.size.y;
}

class olc_RelayRace : public olc::PixelGameEngine
{
public:
    olc_RelayRace()
    {
        sAppName = "RelayRace";
    }

    float g;//gravity acceleration
    olc::Renderable level;
    gameObject player, camera;
    olc::vi2d tileSize, visibleTiles, worldSize; // 212 x 14

    Collider playerCollider;
    std::vector<Collider*> colliders;


public:
    bool OnUserCreate() override
    {
        level.Load("../src/20592_rev.png");
        g = -32;

        player.position = { 33.0f, 9.5f };
        player.size = { 1, 1 };
        player.velocity = { -0.0f, 0.0f };

        camera.position = { 212 - 40, 0.0f };
        tileSize = { 16, 16 };
        worldSize = { 212, 14 };
        visibleTiles = { ScreenWidth() / tileSize.x, ScreenHeight() / tileSize.y };


        playerCollider = { "player", {0.0f, 0.0f}, {1.0f, 1.0f}, &player};
        colliders = {
          new Collider {std::string("something"), olc::vf2d(200.0f, 2.7f), olc::vf2d(1.0f, 1.0f), nullptr},
        };

        return true;
    }

    bool OnUserUpdate(float fElapsedTime) override
    {
        DrawDecal( { camera.position.x * -16, 0.0f }, level.Decal());
    //temp clamp before collision



        if (player.position.x < 20.0f) {
            camera.position.x += player.position.x - 20;
            player.position.x = 20.0f;

        }
        if (player.position.x > 39.0f) {
            player.position.x = 39.0f;
        }

        // move character
        if (GetKey(olc::Key::LEFT).bHeld) {
            player.velocity.x -=  fElapsedTime * 12.0;

        } else if (GetKey(olc::Key::RIGHT).bHeld) {
            player.velocity.x += fElapsedTime * 12.0;
        }
        if (GetKey(olc::Key::SPACE).bPressed && player.velocity.y == 0.00) {
            player.velocity.y = 20;
        }

        player.position += player.velocity * fElapsedTime;
        player.velocity.y += g * fElapsedTime;
        player.velocity.x += -1.5f * player.velocity.x * fElapsedTime;

        playerCollider.position = player.position + camera.position;
        std::cout << playerCollider.position.x << ' ' << playerCollider.position.y << std::endl;

        if (player.position.y < 2.5)
        {
            player.velocity.y = 0.0;
            player.position.y = 2.5;
        }

        FillRectDecal({ player.position.x * 16, 16 * (worldSize.y - player.position.y)}, player.size * 16.0f);

        for (auto c : colliders)
        {
          if (check_collision(playerCollider, *c))
            std::cout << "collision with " << c->tag << std::endl;

          FillRectDecal({ (c->position.x - camera.position.x) * 16, 16 * (worldSize.y - c->position.y)}, c->size * 16.0f, olc::RED);
        }

        return !GetKey(olc::Key::ESCAPE).bPressed;
    }

private:

};

int main()
{
    olc_RelayRace app;
    if(app.Construct(640,224,2, 2, false, false))
    {
        app.Start();
    }
    else
    {
        std::cout << "Failed to start PGE" << std::endl;
    }

    return 0;
}
