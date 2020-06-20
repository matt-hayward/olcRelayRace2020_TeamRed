#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

struct gameObject {
    olc::vi2d size;
    olc::vf2d velocity;
    olc::vf2d position;
};

class olc_RelayRace : public olc::PixelGameEngine
{
public:
    olc_RelayRace()
    {
        sAppName = "RelayRace";
    }

    olc::Renderable level;
    gameObject player, camera;
    olc::vi2d tileSize, visibleTiles, worldSize; // 212 x 14

public:
    bool OnUserCreate() override
    {
        level.Load("../src/20592_rev.png");


        player.position = { 205.f, 0.0f };
        player.size = { 1, 1 };
        player.velocity = { -16.0f, 0.0f };

        camera.position = { 185, 0.0f };
        tileSize = { 16, 16 };
        worldSize = { 212, 14 };
        visibleTiles = { ScreenWidth() / tileSize.x, ScreenHeight() / tileSize.y };

        return true;
    }

    bool OnUserUpdate(float fElapsedTime) override
    {
        DrawDecal( { camera.position.x * -16, 0.0f }, level.Decal());
	
        if (player.position.x - camera.position.x < -1 * (640 / 2) / 16.0f) {
            camera.position.x -= player.velocity.x * fElapsedTime;
        }
	/*
        // move character
        if (GetKey(olc::Key::LEFT).bHeld) {
            player.position -= player.velocity * fElapsedTime;
	    
        } else if (GetKey(olc::Key::RIGHT).bHeld) {
            player.position += player.velocity * fElapsedTime;
        }
	*/
//	player.position -= player.velocity * fElapsedTime;
        FillRectDecal({ player.position.x * 16, ScreenHeight() - 56.0f}, player.size * 16.0f);
        
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
