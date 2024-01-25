#pragma once
#include "engine.h"
#include <memory>

namespace Cinnamon {

	class Empty : public GameObject {
	public:

		inline Empty() : GameObject() {
			displayChar = L' ';
		}

		inline void Start() override {

		}

		inline void Update() override {

		}

		inline void FixedUpdate() override {

		}
	};

}
