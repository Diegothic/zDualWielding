// Supported with union (c) 2020 Union team
// Union SOURCE file

namespace GOTHIC_ENGINE {

	// void EquipWeapon( oCItem* ) zCall( 0x0073A030 );
	HOOK Hook_oCNpc_EquipWeapon_Union PATCH(&oCNpc::EquipWeapon, &oCNpc::EquipWeapon_Union);
	void oCNpc::EquipWeapon_Union(oCItem* WeaponToEquip) {
		DualWielding DualWielder(this);
		DualWielder.RemoveDualAnimations();
		
		oCItem* LeftSwordEquipped  = DualWielder.GetEquippedLeftSword();
		oCItem* RightSwordEquipped = GetSlotItem(NPC_NODE_SWORD);
		if (LeftSwordEquipped && RightSwordEquipped) {
			if (WeaponToEquip == LeftSwordEquipped) {
				DualWielder.UnequipLeftWeapon();

				EquipItem(RightSwordEquipped);
				PutInSlot(NPC_NODE_SWORD, RightSwordEquipped, 1);
				return;
			}

			DualWielder.UnequipRightWeapon();
			DualWielder.UnequipLeftWeapon();

			if (WeaponToEquip != RightSwordEquipped) {
				THISCALL(Hook_oCNpc_EquipWeapon_Union)(WeaponToEquip);
			}

			return;
		}

		if (!RightSwordEquipped
			|| RightSwordEquipped == WeaponToEquip
			|| !DualWielder.IsWeaponForDualWielding(RightSwordEquipped)
			|| !DualWielder.IsWeaponForDualWielding(WeaponToEquip)
			) {
			THISCALL(Hook_oCNpc_EquipWeapon_Union)(WeaponToEquip);
			return;
		}

		DualWielder.EquipDualWeapons(RightSwordEquipped, WeaponToEquip);
		DualWielder.ApplyDualAnimations();
	}

	// void SetWeaponMode2_novt( zSTRING const& ) zCall( 0x00738C60 );
	HOOK Hook_oCNpc_SetWeaponMode2_novt_Union PATCH(0x00738C60, &oCNpc::SetWeaponMode2_novt_Union);
	void oCNpc::SetWeaponMode2_novt_Union(zSTRING const& NewWeaponMode) {
		DualWielding DualWielder(this);

		int FromFightMode = fmode;
		THISCALL(Hook_oCNpc_SetWeaponMode2_novt_Union)(NewWeaponMode);

		DualWielder.ChangeWeaponMode(NewWeaponMode, FromFightMode);
	}
}
