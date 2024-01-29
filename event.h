#pragma once
#include <vector>
#include <algorithm>
#include <utility>
#include <Windows.h>

namespace Cinnamon {
	template<typename Ret, typename...Args>
	class Event {
	public:
		std::vector<Ret(*)(Args...)> Listeners;
		std::vector<std::function<Ret(Args...)>> MemListeners;

		template<class ObjectType>
		inline void operator +=(Ret(ObjectType::*func)(Args...)) {
			Listeners.emplace_back(func);
		}
		
		template<class ObjectType>
		inline void AddMemFunc(ObjectType& obj, Ret(ObjectType::*func)(Args...)) {
			std::function<Ret(Args...)> f = std::bind(func, &obj);
			MemListeners.emplace_back(f);
		}
		
		inline void operator +=(Ret(*func)(Args...)) {
			Listeners.emplace_back(func);
		}

		inline void operator -=(Ret(func)(Args...)) {
			Listeners.erase(std::remove(Listeners.begin(), Listeners.end(), func), Listeners.end());
		}

		template<class ObjectType>
		inline void RemoveMemFunc(ObjectType& obj, Ret(ObjectType::* func)(Args...)) {
			std::function<Ret(Args...)> f = std::bind(func, &obj);
			MemListeners.erase(std::remove(MemListeners.begin(), MemListeners.end(), f), MemListeners.end());
		}

		inline void operator ()(Args...args) {
			for (auto f : MemListeners) {
				f(args...);
			}

			for (auto f : Listeners) {
				f(args...);
			}
		}
	};



	/*
	template<typename Ret, typename...Args>
	struct Action<ObjectClass, Ret, Args...> {
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
		
		inline void operator -=(Ret(func)(Args...)) {
			Listeners.erase(std::remove(Listeners.begin(), Listeners.end(), func), Listeners.end());
		}
		
		inline void operator -=(ObjectMemFunc func) {
			Listeners.erase(std::remove(Listeners.begin(), Listeners.end(), func), Listeners.end());
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
*/
}

