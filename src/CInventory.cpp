//
// Created by machavi on 5/9/21.
//
#include "CInventory.h"

#include "CItemLoader.h"
#include "CWeaponLoader.h"
bool CInventory::insert(CInventory& from, size_t index, invType type) {
  if (type == ITEM) {
    auto item = from.GetItem(index);
    if (item) {
      m_Inventory.resize(m_Inventory.size() + 1);
      m_Inventory.back().swap(item);
      return true;
    } else {
      return false;
    }
  } else {
    auto item = from.GetWeapon(index);
    if (item) {
      m_WeaponInventory.resize(m_WeaponInventory.size() + 1);
      m_WeaponInventory.back().swap(item);
      return true;
    } else {
      return false;
    }
  }
  return false;
}
const std::vector<std::unique_ptr<CItem>>& CInventory::GetItemInventory() const { return m_Inventory; }
bool CInventory::drop(size_t index, invType type) {
  if (type == ITEM) {
    if (index < m_Inventory.size()) {
      auto it = m_Inventory.begin() + index;
      m_Inventory.erase(it);
    }
  } else {
    if (index < m_WeaponInventory.size()) {
      auto it = m_WeaponInventory.begin() + index;
      m_WeaponInventory.erase(it);
    }
  }
  return false;
}

bool CInventory::Load(CFileLoaderIt it) {
  if (it.GetName() != "inventory") {
    throw std::invalid_argument("wrong inventory node");
    return false;
  }
  it.Child();
  it.Next();
  if (it.GetName() != "size") {
    throw std::invalid_argument("wrong inventory node");
    return false;
  }
  m_Size = std::stoi(it.GetContent());

  if (!it.Next(2)) return false;
  CItemLoader itemLoader;
  m_Inventory = itemLoader.LoadItems(it);

  if (!it.Next(2)) return false;
  CWeaponLoader weaponLoader;
  m_WeaponInventory = weaponLoader.LoadWeapons(it);
  size_t size = 0;
  for (auto& elem : m_Inventory) {
    size += elem->GetSize();
  }

  for (auto& elem : m_WeaponInventory) {
    size += elem->GetSize();
  }
  if (size > m_Size) {
    throw std::invalid_argument("items and weapons dot fit to inventory");
    return false;
  }
  m_CurrSize = size;

  return true;
}
const std::vector<std::unique_ptr<CWeapon>>& CInventory::GetWeaponInventory() const { return m_WeaponInventory; }
std::unique_ptr<CItem> CInventory::GetItem(size_t index) {
  std::unique_ptr<CItem> ptr;
  if (index < m_Inventory.size()) {
    ptr.swap(m_Inventory.at(index));
    this->drop(index, ITEM);
    return ptr;
  }
  return ptr;
}
std::unique_ptr<CWeapon> CInventory::GetWeapon(size_t index) {
  std::unique_ptr<CWeapon> ptr;
  if (index < m_WeaponInventory.size()) {
    ptr.swap(m_WeaponInventory.at(index));
    this->drop(index, WEAPON);
    return ptr;
  }
  return ptr;
}