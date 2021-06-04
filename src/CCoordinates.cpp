//
// Created by machavi on 5/29/21.
//
#include "CCoordinates.h"

#include "cmath"

int CCoordinates::X() const { return m_X; }
int CCoordinates::Y() const { return m_Y; }

bool CCoordinates::Load(CFileLoaderIt iterator) {
  iterator.Child();  // move to X node
  iterator.Next();

  if (!(iterator.GetName() == "X")) {
    throw std::invalid_argument("bad coordinates node name");
    return false;
  }
  m_X = std::stoi(iterator.GetContent());
  iterator.Next();
  iterator.Next();

  if (!(iterator.GetName() == "Y")) {
    throw std::invalid_argument("bad coordinates node name");
    return false;
  }
  m_Y = std::stoi(iterator.GetContent());
  return true;
}

CCoordinates CCoordinates::LoadCoordinates(CFileLoaderIt iterator) {
  CCoordinates coordinates;
  coordinates.Load(iterator);
  return coordinates;
}
std::ostream& operator<<(std::ostream& os, const CCoordinates& cordinates) {
  os << "m_X: " << cordinates.m_X << " m_Y: " << cordinates.m_Y;
  return os;
}
int CCoordinates::Distance(const CCoordinates& other) {
  int x = X() - other.X();
  int y = Y() - other.Y();
  return std::hypot(x, y);
}
bool CCoordinates::operator<(const CCoordinates& rhs) const {
  if (m_X < rhs.m_X) return true;
  if (rhs.m_X < m_X) return false;
  return m_Y < rhs.m_Y;
}
bool CCoordinates::operator>(const CCoordinates& rhs) const { return rhs < *this; }
bool CCoordinates::operator<=(const CCoordinates& rhs) const { return !(rhs < *this); }
bool CCoordinates::operator>=(const CCoordinates& rhs) const { return !(*this < rhs); }
bool CCoordinates::operator==(const CCoordinates& rhs) const { return m_X == rhs.m_X && m_Y == rhs.m_Y; }
bool CCoordinates::operator!=(const CCoordinates& rhs) const { return !(rhs == *this); }