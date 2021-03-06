//
// Created by machavi on 6/1/21.
//
#include "CEntityLoader.h"

#include "CChest.h"
#include "CPlayer.h"
#include "CPlayerController.h"
#include "CZombie.h"
#include "CZombieController.h"
CEntityLoader::CEntityLoader() {
  m_ControlerFactory.Register("zombie", CZombieController::Create);
  m_EntityFactory.Register("zombie", CZombie::Create);
  m_HasController.insert("zombie");
  m_ControlerFactory.Register("player", CPlayerController::Create);
  m_EntityFactory.Register("player", CPlayer::Create);
  m_HasController.insert("player");
  m_EntityFactory.Register("chest", CChest::Create);
}

std::map<CCoordinates, std::shared_ptr<CEntity>> CEntityLoader::LoadEntities(CFileLoaderIt iterator) {
  std::map<CCoordinates, std::shared_ptr<CEntity>> entities;
  if (iterator.GetName() != "entities") {
    throw std::invalid_argument("wrong entity node");
    return entities;
  }

  iterator.Child();  // go to entity node
  iterator.Next();
  for (; iterator.Valid(); iterator.Next(), iterator.Next()) {
    auto prop = iterator.GetProperties();
    if (prop.empty() || prop.size() != 1 || prop.front().first != "entityID") {
      throw std::invalid_argument("wrong entity node ");
      return entities;
    }
    auto entity = m_EntityFactory.createObject(prop.front().second);
    if (!entity) {
      return entities;
    }
    entity->Load(iterator);
    if (m_HasController.count(prop.front().second) == 1) {
      entity->AttachController(m_ControlerFactory.createObject(prop.front().second));
    }
    entities.insert(std::make_pair(entity->GetCoordinates(), entity));
  }
  return entities;
}
std::shared_ptr<CEntity> CEntityLoader::CreateEntity(const std::string& id) {
  auto entity = m_EntityFactory.createObject(id);
  if (m_HasController.count(id) == 1) {
    entity->AttachController(m_ControlerFactory.createObject(id));
  }
  return entity;
}
