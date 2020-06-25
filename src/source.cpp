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

#define RGB(R, G, B) (((R) * 256 + G) * 256 + B)

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
	olc::Renderable mario;
	olc::Renderable marioLeft;


public:
    void load_map() {
        olc::Sprite *map = new olc::Sprite("../../src/testmap.png");

        for(int i = 0; i < map -> width ; i+=16) {
            for(int j = 0; j < map -> height ; j+=16) {
                olc::Pixel pixel = map -> GetPixel(i + 8 ,j + 8);

                std::string tag = "";
                switch (RGB(pixel.r, pixel.g, pixel.b))
                {
					//map->height - j < 2 ? "floor" :
                  case RGB(0, 0, 255): tag =  "block"; break;
                  case RGB(75, 255, 128): tag = "coin"; break;
                  default: break;
                }

                if (tag != "")
                {
                    colliders.push_back(new Collider{
                        tag,
                        olc::vf2d((i / 16.0f), (float)(map->height / 16) - (j / 16.0f)),
                        olc::vf2d(1.0f, 1.0f), nullptr
                    });
                }
            }
        }

        delete map;

    }
    bool OnUserCreate() override
    {
        level.Load("../../src/20592_rev.png");
        g = -32;

        player.position = { 33.0f, 9.5f };
        player.size = { 1, 1 };
        player.velocity = { -0.0f, 0.0f };

        camera.position = { 212 - 40, 0.0f };
        tileSize = { 16, 16 };
        worldSize = { 212, 14 };
        visibleTiles = { ScreenWidth() / tileSize.x, ScreenHeight() / tileSize.y };


        playerCollider = { "player", {0.0f, 0.0f}, {1.0f, 1.0f}, &player};

		mario.Load("../../src/mario.png");
		marioLeft.Load("../../src/marioLeft.png");

        /*colliders =
        {
          new Collider {std::string("coin"), olc::vf2d(200.0f, 3.0f), olc::vf2d(1.0f, 1.0f), nullptr},
        };*/

        load_map();

        return true;
    }

    bool check_collisions()
    {
      for (auto c : colliders)
      {
        if (check_collision(playerCollider, *c))
        {
			std::cout << "COLLIDER FUNCTION\n";
          if (c->tag == "coin")
          {
            score += 100;
            c->tag = "_destroyed_";
          }
          else if (c->tag == "block")
          {
            // If going to collide - don't
            return true;
          }
        }
      }

      return false;
    }

    int score = 0;
    bool OnUserUpdate(float fElapsedTime) override
    {
        DrawDecal( { camera.position.x * -16, 0.0f }, level.Decal());
    //temp clamp before collision

		//olc::vf2d nextPosition = player.position;

		std::cout << "Camera: " << camera.position.x << std::endl;
		
        if (player.position.x < 20.0f) {
            camera.position.x += player.position.x - 20;
			player.position.x = 20.0f;

        }
        if (player.position.x > 39.0f) {
            player.position.x = 39.0f;
        }

        olc::vf2d previousPosition = player.position;

        // move character
        if (GetKey(olc::Key::LEFT).bHeld) {
            player.velocity.x -=  fElapsedTime * 12.0;

        } else if (GetKey(olc::Key::RIGHT).bHeld) {
            player.velocity.x += fElapsedTime * 12.0;
        }

        player.velocity.x += -1.5f * player.velocity.x * fElapsedTime;
        player.position += { player.velocity.x * fElapsedTime, 0.0f };
        playerCollider.position = player.position + camera.position;
        if (check_collisions())
        {
          // left or right collision (depends X speed)
          player.position = previousPosition;
          player.velocity.x = 0;
        }

        previousPosition = player.position;
        if (GetKey(olc::Key::SPACE).bPressed && player.velocity.y == 0.00) {
            player.velocity.y = 20;
        }
        player.velocity.y += g * fElapsedTime;
        player.position += { 0.0f, player.velocity.y * fElapsedTime };
        playerCollider.position = player.position + camera.position;
        if (check_collisions())
        {
          // top or bottom collision (depends Y speed)
			std::cout << "COLLISION Y\n";
          player.position = previousPosition;
          player.velocity.y = 0;
        }

		// check death from falling
		if (player.position.y < 0) {
			camera.position = { 212 - 40, 0.0f };
			player.position = { 33.0f, 9.5f };
			player.velocity = {0.0f, 0.0f};
		}

		if (camera.position.x < -7) {
			DrawStringDecal({ 10.0f, 10.0f }, "YOU WIN!!!");
		}

        // std::cout << playerCollider.position.x << ' ' << playerCollider.position.y << std::endl;

        // if (player.position.y < 2.5)
        // {
        //     player.velocity.y = 0.0;
        //     player.position.y = 2.5;
        // }

		DrawDecal({ player.position.x * 16, 16 * (worldSize.y - player.position.y) }, player.velocity.x >= 0 ? mario.Decal() : marioLeft.Decal());
        //FillRectDecal({ player.position.x * 16, 16 * (worldSize.y - player.position.y)}, player.size * 16.0f);

        /*for (auto c : colliders)
        {
          if (c->tag == "_destroyed_") continue;
          FillRectDecal({ (c->position.x - camera.position.x) * 16, 16 * (worldSize.y - c->position.y)}, c->size * 16.0f, olc::RED);
        }*/

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
