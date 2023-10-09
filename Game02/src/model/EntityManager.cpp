#include "EntityManager.h"

EntityManager::EntityManager()
{
}

void EntityManager::removeDeadEntities(EntityVec &vec)
{
	//		 remove all dead entities from the input vector.
	//		 this is called by the update() function
	EntityVec::iterator erased =
			std::remove_if(vec.begin(), vec.end(), [](std::shared_ptr<Entity> e)
										 { return !e->isActive(); });
	vec.erase(erased, vec.end());
}

void EntityManager::update()
{
	// add entities from m_entitiesToAdd to
	//		 - vector of all entities
	//		 - vector inside the map, with the tag as a key
	for (auto e : m_entitiesToAdd)
	{
		m_entities.push_back(e);
		m_entityMap[e->tag()].push_back(e);
	}

	// clear the entities in waiting room because they have been added already
	m_entitiesToAdd.clear();

	// remove dead entities from the vector of all entities
	removeDeadEntities(m_entities);

	 // remove dead entities from each vector in the entity map
	 // C++17 way of iterating through [key,value] pairs in a map
	 for (auto &[tag, entityVec] : m_entityMap)
	 {
	 	removeDeadEntities(entityVec);
	 }
	// for (auto i = m_entityMap.begin(); i != m_entityMap.end(); i++)
	// {
	// 	removeDeadEntities(entityVec);
	// }
	//for (EntityMap::iterator it = m_entityMap.begin(); it != m_entityMap.end(); ++it)
	//{
	//	EntityVec &entityVec = it->second;

	//	// Call the removeDeadEntities function on entityVec
	//	removeDeadEntities(entityVec);
	//}
}

std::shared_ptr<Entity> EntityManager::addEntity(const std::string &tag)
{
	auto entity = std::shared_ptr<Entity>(new Entity(m_totalEntities++, tag));
	m_entitiesToAdd.push_back(entity);

	return entity;
}

const EntityVec &EntityManager::getEntities()
{
	return m_entities;
}

const EntityVec &EntityManager::getEntities(const std::string &tag)
{
	return m_entityMap[tag];
}
