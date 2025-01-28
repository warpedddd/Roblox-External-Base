#include "sdk.hpp"
#include "../driver-communication/comms.hpp"
#include "../../dependencies/offsets.hpp"
#include <../../utilities/globals/globals.hpp>
#include <d3d9types.h>
#include <iostream>

// Visual Engine Class
Vectors::Vector2 Visualengine::GetViewPortSize() {
    return pMem->Rpm<Vectors::Vector2>(this->Address + VIEWPORT_SIZE);
}
Matrix4 Visualengine::GetViewMatrix() {
    return pMem->Rpm<Matrix4>(this->Address + VIEW_MATRIX);
}

// w2s type shit
Vectors::Vector2 w2s(Vectors::Vector3 RelativeLocation) // you dont need to use visual engine, you could also use camera
{
    Matrix4 ViewProjMatrix = Globals::Roblox::VisualEngine.GetViewMatrix(); 
    Vectors::Vector2 ViewPortSize = Globals::Roblox::VisualEngine.GetViewPortSize();

    Vectors::Vector4 transformed(
        RelativeLocation.x * ViewProjMatrix.data[0] + RelativeLocation.y * ViewProjMatrix.data[4] + RelativeLocation.z * ViewProjMatrix.data[8] + ViewProjMatrix.data[12],
        RelativeLocation.x * ViewProjMatrix.data[1] + RelativeLocation.y * ViewProjMatrix.data[5] + RelativeLocation.z * ViewProjMatrix.data[9] + ViewProjMatrix.data[13],
        RelativeLocation.x * ViewProjMatrix.data[2] + RelativeLocation.y * ViewProjMatrix.data[6] + RelativeLocation.z * ViewProjMatrix.data[10] + ViewProjMatrix.data[14],
        RelativeLocation.x * ViewProjMatrix.data[3] + RelativeLocation.y * ViewProjMatrix.data[7] + RelativeLocation.z * ViewProjMatrix.data[11] + ViewProjMatrix.data[15]
    );

    if (transformed.w < 0.1f)
        return Vectors::Vector2(-1, -1);

    transformed.x /= transformed.w;
    transformed.y /= transformed.w;

    float screenX = (ViewPortSize.x / 2.0f) + (transformed.x * ViewPortSize.x / 2.0f);
    float screenY = (ViewPortSize.y / 2.0f) - (transformed.y * ViewPortSize.y / 2.0f);

    return Vectors::Vector2(screenX, screenY);
}

// GameInstance Class
std::string GameInstance::ReadString(uint64_t Address) // should be in driver class ngl
{
    uint64_t StringCount = pMem->Rpm<uint64_t>(Address + STRING_LENGTH);

    if (StringCount > 15000 || StringCount <= 0)
        return "";

    if (StringCount > 15) 
        Address = pMem->Rpm<uint64_t>(Address);

    std::string buffer;
    buffer.resize(StringCount);

    for (size_t i = 0; i < StringCount; ++i) 
        buffer[i] = pMem->Rpm<char>(Address + i);  

    return buffer;
}
std::vector<GameInstance> GameInstance::Children() 
{
    if (!this->Address)
        return ChildrenList;

    std::vector<GameInstance> vec;
    uintptr_t ChildrenListStart = pMem->Rpm<uintptr_t>(this->Address + CHILDREN);
    uintptr_t ChildrenListEnd = pMem->Rpm<uintptr_t>(ChildrenListStart + STRING_LENGTH);

    for (auto Children = pMem->Rpm<uintptr_t>(ChildrenListStart); Children != ChildrenListEnd; Children += sizeof(uintptr_t)) 
        vec.emplace_back( pMem->Rpm<GameInstance>(Child) ); // emplace back instead of push back because we want to directly store the children

    return vec;
}
std::string GameInstance::InstanceName()
{
    return ReadString(pMem->Rpm<std::uint64_t>(this->Address + INSTANCE_NAME));
}
std::string GameInstance::ClassName()
{
    std::uint64_t instanceClass = pMem->Rpm<std::uint64_t>(this->Address + CLASS_NAME);
    return ReadString(pMem->Rpm<std::uint64_t>(instanceClass + STRING_LENGTH));
}
GameInstance GameInstance::FindFirstChildOfClass(std::string ChildName)
{
    if (!this->Address) 
        return *this;

    for (RBX::GameInstance Child : this->Children()) 
    {
        if (Child.IsA(ChildName)) 
            return Child;
    }

    return *this;
}
GameInstance GameInstance::FindFirstChild(std::string ChildName)
{
    if (!this->Address) 
        return *this;
    
    for (RBX::GameInstance Child : this->Children()) 
    {
        if (Child.Name() == ChildName) 
            return Child;
    }

    return *this;
}
bool GameInstance::IsA(std::string& ClassName) 
{ 
    if (!this->Address) 
        return false;

    return this->ClassName() == ClassName;
}

GameInstance GameInstance::GetLocalPlayer()
{
    if (!this->Address) 
        return *this;

    return pMem->Rpm<GameInstance>(this->Address + LOCAL_PLAYER);
}
GameInstance GameInstance::GetCharacter() // I don't know the actual word for this but in lua it is Character
{
    if (this->Address == NULL) 
        return *this;

    return pMem->Rpm<GameInstance>(this->Address + CHARACTER);
}

// REntity Class
Vectors::Vector3 GameInstance::Position()
{
    if (!this->Address) 
        return {};

    auto Primitive = pMem->Rpm<std::uint64_t>(this->Address + PRIMITIVE);
    return pMem->Rpm<Vectors::Vector3>(Primitive + POSITION);
}

