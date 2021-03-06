//
// Created by machavi on 5/7/21.
//

#include "CGame.h"

#include "CFileLoader.h"
#include "CFileLoaderIterator.h"
#include "CMap.h"
#include "CPlayerCreator.h"
bool CGame::LoadMap(const std::string& path, toLoad what) {
  CFileLoader loader;
  if (!loader.LoadXmlFile(path)) {
    throw std::invalid_argument("cant open map file");
    return false;
  }
  auto it = loader.GetTopNode();
  it.Child();
  it.Next();

  std::string playerPresent = it.GetContent("playerPresent");

  if (playerPresent == "no" && GAME == what) {
    throw std::invalid_argument("player not in game");
  } else if (playerPresent == "yes" && what == MAP) {
    throw std::invalid_argument("player in game");
  } else if (playerPresent != "no" || playerPresent != "yes") {
    throw std::invalid_argument("wrong content");
  }
  m_Name = it.GetContent("name");
  if (m_Name.empty()) {
    return false;
  }

  it.Next(2);

  if (!loadSpawnPoint(it)) {
    return false;
  }

  it = loader.GetNode("rooms");
  it.Child();  // move to room node
  it.Next();

  for (; it.Valid(); it.Next(2)) {
    auto prop = it.GetProperties();
    if (prop.size() != 2 || prop.front().first != "ID" || prop.back().first != "name") {
      throw std::invalid_argument("bad room properties");
      return false;
    }
    int ID = std::stoi(prop.front().second);

    if (m_Rooms.count(ID) == 1) {
      throw std::invalid_argument("rooms with duplicate IDs");
      return false;
    }

    auto room = std::make_shared<CMap>(ID, prop.back().second, *this);

    if (!room->Load(it, room)) {
      return false;
    }
    m_Rooms.insert(std::make_pair(ID, room));

#ifdef DEBUG
    std::cout << *room << std::endl;
#endif
  }
  if (m_Rooms.size() == 0) {
    throw std::invalid_argument("no rooms in map");
    return false;
  }
  if (m_Rooms.count(m_spawnRoomID) != 1) {
    throw std::invalid_argument("spawn room not present in map");
    return false;
  }
  m_Initialized = true;
  return true;
}
bool CGame::IsInitialized() const { return m_Initialized; }

bool CGame::loadSpawnPoint(CFileLoaderIt it) {
  if (it.GetName() != "spawnPoint") {
    throw std::invalid_argument("wrong node");
    return false;
  }
  it.Child();  // go to the child node
  it.Next();
  m_spawnRoomID = std::stoi(it.GetContent("roomID"));
  m_CurrRoomID = m_spawnRoomID;
  it.Next(2);
  return m_SpawnPoint.Load(it);
}
void CGame::Erase() {
  m_Rooms.clear();
  m_spawnRoomID = 1;
  m_Initialized = false;
  m_SpawnPoint = CCoordinates(0, 0);
}
void CGame::Render() {
  auto room = m_Rooms.at(m_CurrRoomID);
  m_Interface.Print(*room);
}
void CGame::Start() {
  while (m_Rooms.at(m_CurrRoomID)->ExecuteTurns()) {
  }
  EndGame();
}
bool CGame::Save() const {
  CFileLoader save;
  CFileLoaderIt it = save.NewDoc("map");
  it.CreateNewTextChildNode("name", m_Name);
  it.CreateNewChildNode("spawnPoint");
  it.CreateNewChildNode("rooms");
  it.Child();
  it.Next();
  saveSpawnPoint(it);
  it.Next();
  for (size_t i = 0; i < m_Rooms.size(); ++i) {
    it.CreateNewChildNode("room");
  }
  it.Child();
  for (auto& elem : m_Rooms) {
    elem.second->Save(it);
    it.Next();
  }
  std::string path = m_Interface.PromtWithMessage<std::string>("Please enter path, where to save the game.");
  return save.SaveFile(path);
}
bool CGame::saveSpawnPoint(CFileLoaderIt it) const {
  if (it.GetName() != "spawnPoint") {
    return false;
  }
  if (!it.CreateNewTextChildNode("roomID", std::to_string(m_spawnRoomID))) return false;
  if (!it.CreateNewChildNode("coordinates")) return false;
  if (!it.Child()) return false;
  if (!it.Next()) return false;

  return m_SpawnPoint.Save(it);
}
bool CGame::SpawnPlayer(std::shared_ptr<CEntity> player) {
  return m_Rooms.at(m_spawnRoomID)->TransferEntityToRoom(player, m_SpawnPoint);
}
void CGame::EndGame() {
  std::string message = "What dou you want to do?\n1)Save game\n2)quit";
  int res = m_Interface.Chooser(message, 2);
  switch (res) {
    case 1:
      Save();
      break;
    case 2:
      return;
      break;
    default:
      m_Interface.Message("Wrong option,choose again.");
      break;
  }
}
