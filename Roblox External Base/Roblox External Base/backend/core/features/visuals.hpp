#include "../../../utilities/globals/globals.hpp"

#ifndef VISUALS_HPP
#define VISUALS_HPP

inline void BOX(Vectors::Vector2 top, Vectors::Vector2 bottom, float width, ImColor color)
{
	float height = bottom.y - top.y; 
	ImGui::GetBackgroundDrawList()->AddRect(ImVec2(top.x - width / 2, top.y), ImVec2(top.x + width / 2, top.y + height), color, 0.0f, 0, 1.5f);
}

inline void ESP() // bla bla bla this is rushed ahhaha bot lol (plz paste this)
{
	if (Globals::Features::ESPEnabled)
	{
		// don't ask me about this... (ima swiss cheese yo ahh)
		for (auto& hothispanicmom : Globals::Roblox::Players.Children()) // not doing a player cache dont ask me :broken_heart:
		{
			if (hothispanicmom.Address == Globals::Roblox::LocalPlayer.Address)
				continue;

			GameInstance Character = hothispanicmom.GetCharacter();
			if (!Character.Address)
				continue; // no character found then skip (I think its called model instance or something, I don't remember

			GameInstance Humanoid = Character.FindFirstChild("Humanoid");
			if (!Humanoid.Address) // we always gotta check if a dudes balls is there other wise it wouldn't be a man right?
				continue;

			GameInstance Head = Character.FindFirstChild("Head");
			GameInstance Hrp = Character.FindFirstChild("HumanoidRootPart");

			Vectors::Vector2 Head2D = w2s(Head.Position()); // my balls feelin nutty so I'm doing this lil check (btw if Head is not defined make sure to use _TRANSMIT_FILE_BUFFERS) hehe
			Vectors::Vector2 Hrp2D = w2s(Hrp.Position());
			if (Head2D.x == -1 || Head2D.y == -1)
				continue;

			float boxWidth = (Hrp2D.y - Head2D.y) * 2.0f;
			BOX(Head2D, Hrp2D, boxWidth, ImColor(255, 0, 0, 255));
		}
	}
}

#endif // VISUALS_HPP