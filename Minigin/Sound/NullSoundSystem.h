#pragma once
#include "SoundSystem.h"

namespace dae
{
	class NullSoundSystem final : public SoundSystem
	{
	public:

		NullSoundSystem();
		~NullSoundSystem() override;

		// Copy and move
		NullSoundSystem(const NullSoundSystem& other) = delete;
		NullSoundSystem(NullSoundSystem&& other) = delete;
		NullSoundSystem& operator=(const NullSoundSystem& other) = delete;
		NullSoundSystem& operator=(NullSoundSystem&& other) = delete;


		void Init(const std::string&) override {};
		virtual void Quit() override {};

		virtual void NotifySound(SoundData) {};


	};
}
