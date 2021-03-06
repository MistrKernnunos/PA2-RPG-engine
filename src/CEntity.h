//
// Created by machavi on 5/7/21.
//
#pragma once
#include <ostream>
#include <string>
#include <utility>
#include <vector>

#include "CControler.h"
#include "CCoordinates.h"
#include "CFileLoaderIterator.h"
#include "CGame.h"
#include "CInventory.h"
class CPlayerCreator;
class CMap;

enum state { DEAD, ALIVE };

class CEntity {
 public:
  /**
   * Constructor
   * @param name name of the entity
   */
  CEntity() = default;
  virtual ~CEntity() = default;
  /**
   * executes one turn
   */
  virtual bool Turn() = 0;

  /**
   * Facilitates attack action
   * @param toAttack which entity to attack
   * @return true if the attack was successful, false if not (not enough action
   * points, target too far, etc.)
   */
  virtual bool Attack(CEntity& toAttack, const CWeapon& weapon) = 0;

  /**
   * handles action if the entity is attacked
   * @param attackPower attack damage of the incoming attack
   * @return the damage that has the entity received
   */
  virtual int Attacked(const int attackDamage) = 0;

  /**
   * loads stats and inventory from file
   * @param it iterator to entity node
   * @return true if successfully loaded otherwise null
   */
  bool Load(CFileLoaderIt it);

  /**
   * saves entity to iterator
   * @param it where to save the entity
   * @return true if successful false if not
   */
  bool Save(CFileLoaderIt it);

  /**
   *moves entity by x and y
   * @param x
   * @param y
   * @return
   */
  virtual bool Move(const int x, const int y) = 0;

  /**
   * changes position of entity
   * @param coordinates where to put the entity;
   */
  void ChangePostion(CCoordinates coordinates);

  bool IsLootable();

  void AttachController(std::shared_ptr<CControler> controler);

  std::vector<std::shared_ptr<CEntity>> GetEntitiesInRange(int range) const;
  std::vector<std::shared_ptr<CEntity>> GetLootableEntities() const;

  const std::string& GetName() const;
  int GetMaxHealth() const;
  int GetHealth() const;
  int GetDefencePower() const;
  int GetAttackPower() const;
  int GetActionPoints() const;
  int GetXp() const;
  int GetLevel() const;
  const CCoordinates& GetCoordinates() const;
  int GetMovementCost() const;
  int GetCurrActionPoints() const;
  int GetAttackCost() const;
  int GetDefenseConst() const;
  state GetState() const;
  int GetMovement() const;
  bool InsertIntoMap(std::weak_ptr<CMap> room);
  void SetHealth(int mHealth);
  //  CInventory& GetInventory();

  const CInventory& GetInventory() const;
  CInventory& GetEditableInventory();

  void PrintToBuffer(std::vector<std::vector<std::string>>& outputBuffer);

  friend std::ostream& operator<<(std::ostream& os, const CEntity& entity);

  friend CPlayerCreator;

 private:
  bool loadProperties(CFileLoaderIt iterator);
  bool saveProperties(CFileLoaderIt iterator);
  virtual bool addEntityId(CFileLoaderIt it) = 0;

 protected:
  // information about the entity
  std::string m_Name;
  int m_MaxHealth = 0;
  int m_NextLevelCoef = 5;
  int m_Movement = 0;
  int m_MovementCost = 0;
  int m_AttackCost = 0;
  int m_DefenseCost = 0;
  int m_ActionPoints = 0;

  // current stats of the entity
  int m_Health = 0;
  int m_DefencePower = 0;
  int m_AttackPower = 0;
  int m_CurrActionPoints = 0;
  int m_XP = 0;
  int m_Level = 1;
  bool m_Lootable = false;

  state m_State = ALIVE;
  std::shared_ptr<CControler> m_Controller;

  std::weak_ptr<CMap> m_Room;
  CCoordinates m_Coordinates;
  CInventory m_Inventory;
  std::string m_Apperance = "\33[105m*\33[0m";
};
