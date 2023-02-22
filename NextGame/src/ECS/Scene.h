#pragma once

#include <bitset>
#include <vector>
#include <iostream>

constexpr int MAX_COMPONENTS = 32;
constexpr int MAX_ENTITIES = 100;
using EntityId = unsigned long long;
using EntityIndex = unsigned int;
using EntityVersion = unsigned int;
using ComponentMask = std::bitset<MAX_COMPONENTS>;

extern int sComponentCounter;

template <class T>
int GetId()
{
	static int sComponentId = sComponentCounter++;
	return sComponentId;
}

inline EntityId CreateEntityId(const EntityIndex index, const EntityVersion version)
{
	// Shift the index up 32, and put the version in the bottom
	return (static_cast<EntityId>(index) << 32) | static_cast<EntityId>(version);
}

inline EntityIndex GetEntityIndex(const EntityId id)
{
	// Shift down 32 so we lose the version and get our index
	return id >> 32;
}

inline EntityVersion GetEntityVersion(const EntityId id)
{
	// Cast to a 32 bit int to get our version number (loosing the top 32 bits)
	return static_cast<EntityVersion>(id);
}

inline bool IsEntityValid(const EntityId id)
{
	// Check if the index is our invalid index
	return (id >> 32) != static_cast<EntityIndex>(-1);
}


struct ComponentPool
{
	explicit ComponentPool(const size_t elementsize)
	{
		// We'll allocate enough memory to hold MAX_ENTITIES, each with element size
		elementSize = elementsize;
		pData = new char[elementSize * MAX_ENTITIES];
	}

	~ComponentPool()
	{
		delete[] pData;
	}

	void* Get(const size_t index) const
	{
		// looking up the component at the desired index
		return pData + index * elementSize;
	}

	char* pData{nullptr};
	size_t elementSize{0};
};


struct Scene
{
	// All the information we need about each entity
	struct EntityDesc
	{
		EntityId Id;
		ComponentMask Mask;
	};

	std::vector<EntityDesc> Entities;
	// store all components in a serial cache friendly format
	std::vector<ComponentPool*> ComponentPools;
	std::vector<EntityIndex> FreeEntities;

	EntityId NewEntity()
	{
		if (!FreeEntities.empty())
		{
			const EntityIndex newIndex = FreeEntities.back();
			FreeEntities.pop_back();
			const EntityId newId = CreateEntityId(newIndex, GetEntityVersion(Entities[newIndex].Id));
			Entities[newIndex].Id = newId;
			return Entities[newIndex].Id;
		}
		Entities.push_back({CreateEntityId(static_cast<EntityIndex>(Entities.size()), 0), ComponentMask()});
		return Entities.back().Id;
	}

	/*
	 * AddComponent an component to an entity
	 */
	template <typename T>
	T* AddComponent(const EntityId id)
	{
		const int componentId = GetId<T>();

		if (ComponentPools.size() <= componentId) // Not enough component pool
		{
			ComponentPools.resize(componentId + 1, nullptr);
		}
		if (ComponentPools[componentId] == nullptr) // New component, make a new pool
		{
			ComponentPools[componentId] = new ComponentPool(sizeof(T));
		}

		// Looks up the component in the pool, and initializes it with placement new
		T* pComponent = new(ComponentPools[componentId]->Get(GetEntityIndex(id))) T();

		// Set the bit for this component to true and return the created component
		Entities[GetEntityIndex(id)].Mask.set(componentId);
		return pComponent;
	}


	/**
	 * RemoveEntity an entity from the entity list
	 */
	template <typename T>
	void RemoveEntity(const EntityId id)
	{
		// ensures you're not accessing an entity that has been deleted
		if (Entities[GetEntityIndex(id)].Id != id)
			return;

		const int componentId = GetId<T>();
		Entities[GetEntityIndex(id)].Mask.reset(componentId);
	}


	/**
	 * Destroy an entity and add
	 */
	void DestroyEntity(const EntityId id)
	{
		const EntityId newId = CreateEntityId(static_cast<EntityIndex>(-1), GetEntityVersion(id) + 1);
		Entities[GetEntityIndex(id)].Id = newId;
		Entities[GetEntityIndex(id)].Mask.reset();
		FreeEntities.push_back(GetEntityIndex(id));
	}

	template <typename T>
	T* Get(const EntityId id)
	{
		const int componentId = GetId<T>();
		if (!Entities[GetEntityIndex(id)].Mask.test(componentId))
			return nullptr;

		T* pComponent = static_cast<T*>(ComponentPools[componentId]->Get(GetEntityIndex(id)));
		return pComponent;
	}
};

struct SceneIterator
{
	SceneIterator(Scene& scene, int componentIds[], const int size) : PScene(&scene)
	{
		if (size == 0)
		{
			All = true;
		}
		else
		{
			for (int i = 0; i < size; i++)
				ComponentMask.set(componentIds[i]);
		}
	}

	struct Iterator
	{
		Iterator(Scene* pScene, const EntityIndex index, const ComponentMask mask, const bool all)
			: Index(index), PScene(pScene), Mask(mask), All(all)
		{
		}

		EntityId operator*() const
		{
			return PScene->Entities[Index].Id;
		}

		bool operator==(const Iterator& other) const
		{
			return Index == other.Index || Index == PScene->Entities.size();
		}

		bool operator!=(const Iterator& other) const
		{
			return Index != other.Index && Index != PScene->Entities.size();
		}

		bool ValidIndex() const
		{
			return
				// It's a valid entity ID
				IsEntityValid(PScene->Entities[Index].Id) &&
				// It has the correct component mask
				(All || Mask == (Mask & PScene->Entities[Index].Mask));
		}

		Iterator& operator++()
		{
			do
			{
				Index++;
			}
			while (Index < PScene->Entities.size() && !ValidIndex());
			return *this;
		}

		EntityIndex Index;
		Scene* PScene;
		ComponentMask Mask;
		bool All{false};
	};

	Iterator begin() const
	{
		EntityIndex firstIndex = 0;
		while (firstIndex < PScene->Entities.size() &&
			(ComponentMask != (ComponentMask & PScene->Entities[firstIndex].Mask)
				|| !IsEntityValid(PScene->Entities[firstIndex].Id)))
		{
			firstIndex++;
		}
		return {PScene, firstIndex, ComponentMask, All};
	}


	Iterator end() const
	{
		return {PScene, static_cast<EntityIndex>(PScene->Entities.size()), ComponentMask, All};
	}


	Scene* PScene{nullptr};
	ComponentMask ComponentMask;
	bool All{false};
};
