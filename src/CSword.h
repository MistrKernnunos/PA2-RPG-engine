//
// Created by machavi on 6/4/21.
//

#pragma once
#include "CEntity.h"
#include "CWeapon.h"

class CSword : public CWeapon {
 public:
  CSword() = default;
  bool Defense(std::shared_ptr<CEntity> user) override;
  bool Effect(std::shared_ptr<CEntity> toEffect, std::shared_ptr<CEntity> user) override;
  bool Attack(std::shared_ptr<CEntity> toAttack, std::shared_ptr<CEntity> user) override;
  static std::unique_ptr<CWeapon> Create();
};