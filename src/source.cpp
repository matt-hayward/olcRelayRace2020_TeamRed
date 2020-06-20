#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

class olc_RelayRace : public olc::PixelGameEngine
{
public:
    olc_RelayRace()
    {
        sAppName = "RelayRace";
    }

public:
    bool OnUserCreate() override
    {
        return true;
    }

    bool OnUserUpdate(float fElapsedTime) override
    {
        Clear(olc::BLACK);
        for(int y = 0; y < ScreenHeight(); y++)
            for(int x = 0; x < ScreenWidth(); x++)
                Draw(x, y, olc::Pixel(rand() % 255, rand() % 255, rand() % 255));
        
        return !GetKey(olc::Key::ESCAPE).bPressed;
    }

private:

};

int main()
{
    olc_RelayRace app;
    if(app.Construct(320,180,4, 4, false, false))
    {
        app.Start();
    }
    else
    {
        std::cout << "Failed to start PGE" << std::endl;
    }
    
    return 0;
}