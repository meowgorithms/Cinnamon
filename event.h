#pragma once
#include <vector>
#include <algorithm>
#include <utility>
#include <Windows.h>

namespace Cinnamon {	

	template<class ObjectClass, typename Ret, typename...Args>
	struct Action {
	public:
		typedef Ret(ObjectClass::*ObjectMemFunc)(Args...);
		
		ObjectClass& obj;

		std::vector<ObjectMemFunc> MemListeners;
		std::vector<Ret(*)(Args...)> Listeners;
		
		Action(ObjectClass& obj) : obj(obj) {}
		
		inline void operator +=(ObjectMemFunc func) {
			MemListeners.emplace_back(func);
		}

		inline void operator +=(Ret(func)(Args...)) {
			Listeners.emplace_back(func);
		}
		
		
		inline void operator ()(Args...args) {
			for (auto f : MemListeners) {
				std::invoke(f, obj, std::forward<Args>(args)...);
			}

			for (auto f : Listeners) {
				f(args...);
			}
		}
	};

}

