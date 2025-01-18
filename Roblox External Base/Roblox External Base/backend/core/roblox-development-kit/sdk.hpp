#include <cmath>
#include <string>
#include <vector>

#ifndef SDK_HPP
#define SDK_HPP

class Vectors
{
public:
    struct Vector2 final {
        float x, y;
    };

    struct Vector3 final {
        float x, y, z;
    };

    struct Vector4 final {
        float x, y, z, w;
    };
};

struct Matrix4 final {
    float data[16];
};

class Visualengine
{
public:
    uint64_t Address;
    Vectors::Vector2 GetViewPortSize();
    Matrix4 GetViewMatrix();
    Matrix4 SetViewMatrix(Matrix4 Point);
}; static Visualengine* visual_engine = new Visualengine;

class GameInstance
{
public:
    uint64_t Address;

    // --------------------------------------------------------------------- //
    // GameInstance functions - required functions to read basic game data   //
    // --------------------------------------------------------------------- //

    std::string ReadString(uint64_t Address);
    std::string InstanceName();
    std::string ClassName();
    std::vector<GameInstance> Children();
    GameInstance FindFirstChildOfClass(std::string ChildName);
    GameInstance FindFirstChild(std::string ChildName);

    bool IsA(std::string& ClassName);

    // --------------------------------------------------------------------- //
    // Entity functions - functions specific to game entities (like players) //
    // --------------------------------------------------------------------- //

    GameInstance GetLocalPlayer();
    GameInstance GetCharacter();
    Vectors::Vector3 Position();
}; static GameInstance* REntity = new GameInstance;

Vectors::Vector2 w2s(Vectors::Vector3 RelativeLocation);

#endif // SDK_HPP
