#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>

// clear window and draw full screen background
static void loadScreen(sf::RenderWindow &screen, std::string path)
{
    screen.clear(sf::Color::Black);
    sf::Texture texture(path, false, sf::IntRect({0,0},{1920,1080}));
    sf::Sprite sprite(texture);
    screen.draw(sprite);
}

// draw a tile at specific location
static void drawTile(sf::RenderWindow &screen, std::string path, int w, int h, float x, float y)
{
    sf::Texture texture(path, false, sf::IntRect({0,0}, {w,h}));
    sf::Sprite sprite(texture);
    sprite.setPosition({x,y});
    screen.draw(sprite);
}

// Count mines
template <std::size_t H, std::size_t W>
int countMines(const bool (&grid)[H][W], int rows, int columns)
{
    int checkHorizontal;
    int checkVertical;
    int count = 0;
    for (int horizontal = -1; horizontal <= 1; horizontal++)
    {
        for (int vertical = -1; vertical <= 1; vertical++)
        {
            if (horizontal == 0 && vertical == 0)
                continue;
            else
            {
                checkHorizontal = rows + horizontal;
                checkVertical = columns + vertical;
            }
            // makes sure the array stays in bounds
            if ((checkHorizontal >= 0 && checkHorizontal < static_cast<int>(H)) &&  (checkVertical   >= 0 && checkVertical   < static_cast<int>(W)))
            {
                // increases the count whenever a mine is in one of the adjacent spaces
                if (grid[checkHorizontal][checkVertical])
                    count++;
            }
        }
    }
    return count;
}

// flood and scoring
template <std::size_t H, std::size_t W>
int floodScore(const bool (&grid)[H][W], bool (&selected)[H][W], bool (&scored)[H][W], int rows, int columns)
{
    int score = 0;
    int checkHorizontal;
    int checkVertical;
    for (int horizontal = -1; horizontal <= 1; horizontal++)
    {
        for (int vertical = -1; vertical <= 1; vertical++)
        {
            if (horizontal == 0 && vertical == 0)
            {
                continue;
            }
            else
            {
                checkHorizontal = rows + horizontal;
                checkVertical = columns + vertical;
            }
            // makes sure the array stays in bounds
            if ((checkHorizontal >= 0 && checkHorizontal < static_cast<int>(H)) &&
                (checkVertical   >= 0 && checkVertical   < static_cast<int>(W)))
            {
                // if the square is not a mine and has not already been scored, then that square is selected and then scored
                if (!grid[checkHorizontal][checkVertical] && !scored[checkHorizontal][checkVertical])
                {
                    selected[checkHorizontal][checkVertical] = true;
                    scored[checkHorizontal][checkVertical] = true;
                    score += 100;
                }
            }
        }
    }
    return score;
}

// flood for demolition
template <std::size_t H, std::size_t W>
void floodDemolition(const bool (&grid)[H][W], bool (&selected)[H][W], int rows, int columns)
{
    int checkHorizontal;
    int checkVertical;
    for (int horizontal = -1; horizontal <= 1; horizontal++)
    {
        for (int vertical = -1; vertical <= 1; vertical++)
        {
            if (horizontal == 0 && vertical == 0)
            {
                continue;
            }
            else
            {
                checkHorizontal = rows + horizontal;
                checkVertical = columns + vertical;
            }
            // makes sure the array stays in bounds
            if ((checkHorizontal >= 0 && checkHorizontal < static_cast<int>(H)) && (checkVertical   >= 0 && checkVertical   < static_cast<int>(W)))
            {
                // if the square is not a mine then select it
                if (!grid[checkHorizontal][checkVertical])
                {
                    selected[checkHorizontal][checkVertical] = true;
                }
            }
        }
    }
}

// abstract class for visual effect
class Effect {
public:
    virtual ~Effect() = default; // allow deleting through base pointer
    virtual bool update(float) = 0; // advance one frame; return true when finished
    virtual void draw(sf::RenderWindow& window) const = 0; // render the effect
};

class Effects {
public:
    ~Effects()
    {
        clearAll();
    }

    // construct and register via template
    template <class T, class... Args>
    T& spawn(Args... args)
    { // create an effect with given arguments
        T* created = new T(args...);  // allocate effect
        effectsList.push_back(created);  // store base-class pointer
        return *created; // return reference to new effect
    }

    //
    void spawn(Effect* effectPtr)
    {
        effectsList.push_back(effectPtr);  // store it
    }

    // advance and remove finished
    void update(float frameTimeSec)
    {
        int i = 0;
        while (i < (int)effectsList.size())
        { // loop over list
            bool done = effectsList[i]->update(frameTimeSec); // step this effect
            if (done)
            {
                delete effectsList[i]; // free memory
                effectsList.erase(effectsList.begin() + i); // remove from list
            }
            else
            {
                ++i; // move to next
            }
        }
    }

    // draw all active effects
    void draw(sf::RenderWindow& window) const
    {
        for (int i = 0; i < (int)effectsList.size(); ++i)
            effectsList[i]->draw(window);
    }

    void clearAll()
    {
        for (int i = 0; i < (int)effectsList.size(); ++i)
            delete effectsList[i];
        effectsList.clear();
    }

private:
    std::vector<Effect*> effectsList; // stores pointers to effects
};

// circle that expands from a center point
class RingWaveEffect : public Effect {
public:
    RingWaveEffect(sf::Vector2f center, float startRadiusIn, float endRadiusIn, float lifetimeSecondsIn, sf::Color outlineColor)
        : startRadius(startRadiusIn), // starting size
          endRadius(endRadiusIn),  // final size
          framesLived(0) // frames used so far
    {
        // convert seconds to frames using 60 fps
        const int fps = 60;
        if (lifetimeSecondsIn <= 0.f)
            totalFrames = 1;
        else
        {
            int f = (int)(lifetimeSecondsIn * fps + 0.5f);
            totalFrames = (f < 1 ? 1 : f);
        }

        ringShape.setFillColor(sf::Color::Transparent); // no fill, only outline
        ringShape.setOutlineThickness(6.f);  // outline thickness
        ringShape.setOutlineColor(outlineColor); // outline color
        ringShape.setPosition(center); // fixed center position
    }

    bool update(float) override
    {
        // progress in [0..1] based on frames
        float progress = (totalFrames > 0) ? (float)framesLived / (float)totalFrames : 1.f;
        if (progress < 0.f)
            progress = 0.f; // force low
        if (progress > 1.f)
            progress = 1.f;  // force high

        float radius = startRadius + (endRadius - startRadius) * progress; // change size
        ringShape.setRadius(radius); // apply radius
        ringShape.setOrigin(sf::Vector2f{ radius, radius }); // keep centered

        if (framesLived < totalFrames)
            ++framesLived; // advance one frame
        return framesLived >= totalFrames; // done when we reach total frames
    }

    void draw(sf::RenderWindow& window) const override
    {
        window.draw(ringShape); // show circle outline
    }

private:
    float startRadius{0.f}; // start size
    float endRadius{0.f};  // end size
    int   framesLived{0};  // frames elapsed
    int   totalFrames{1}; // total frames to live
    sf::CircleShape ringShape;  // SFML circle shape
};

// solid color rectangle that covers view
class ScreenFlashEffect : public Effect
{
public:
    ScreenFlashEffect(sf::Color fillColorIn, float lifetimeSecondsIn) // set color and time
        : fillColor(fillColorIn), framesLived(0)
    {
        // convert seconds to frames using 60 fps
        const int fps = 60;
        if (lifetimeSecondsIn <= 0.f)
            totalFrames = 1;
        else
        {
            int f = (int)(lifetimeSecondsIn * fps + 0.5f);
            totalFrames = (f < 1 ? 1 : f);
        }
    }

    bool update(float) override
    {
        if (framesLived < totalFrames)
            ++framesLived; // advance one frame
        return framesLived >= totalFrames; // finished when out of frames
    }

    void draw(sf::RenderWindow& window) const override
    {
        sf::RectangleShape rect; // simple rectangle
        sf::Vector2f viewSize   = window.getView().getSize(); // current view size
        sf::Vector2f viewCenter = window.getView().getCenter(); // current view center
        rect.setSize(viewSize); // fill the view
        rect.setOrigin(sf::Vector2f{ viewSize.x * 0.5f, viewSize.y * 0.5f }); // center origin
        rect.setPosition(viewCenter);  // place at center
        rect.setFillColor(fillColor); // solid color
        window.draw(rect); // draw it
    }

private:
    sf::Color fillColor; // color to fill the screen
    int framesLived{0};  // frames elapsed
    int totalFrames{1};  // total frames to live
};

class ExplosionSoundEffect : public Effect
{
public:
    ExplosionSoundEffect(const std::string& file, float vol = 100.f)
        : buffer(), sound(buffer)
    {
        if (buffer.loadFromFile(file))
            { // load audio data
            sound.setVolume(vol);
            sound.play(); // play once
        }
    }

    bool update(float) override
    {
        return sound.getStatus() == sf::SoundSource::Status::Stopped; // done when sound stops
    }

    void draw(sf::RenderWindow&) const override { }

private:
    sf::SoundBuffer buffer; // holds audio samples
    sf::Sound       sound; // plays the buffer
};
