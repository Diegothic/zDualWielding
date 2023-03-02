// Supported with union (c) 2020 Union team
// Union SOURCE file

namespace GOTHIC_ENGINE {

	// void EquipWeapon( oCItem* ) zCall( 0x0073A030 );
	HOOK Hook_oCNpc_EquipWeapon_Union PATCH(&oCNpc::EquipWeapon, &oCNpc::EquipWeapon_Union);
	void oCNpc::EquipWeapon_Union(oCItem* WeaponToEquip) {
		DualWielding DualWielder(this);
		DualWielder.RemoveDualAnimations();
		
		oCItem* LeftSwordEquipped = DualWielder.GetEquippedLeftSword();
		oCItem* RightSwordEquipped = GetSlotItem(NPC_NODE_SWORD);
		if (LeftSwordEquipped && RightSwordEquipped) {
			if (WeaponToEquip == LeftSwordEquipped) {
				DualWielder.UnequipLeftWeapon();
				EquipItem(RightSwordEquipped);
				PutInSlot(NPC_NODE_SWORD, RightSwordEquipped, 1);
				return;
			}

			DualWielder.UnequipLeftWeapon();
			DualWielder.UnequipRightWeapon();

			if (WeaponToEquip != RightSwordEquipped) {
				THISCALL(Hook_oCNpc_EquipWeapon_Union)(WeaponToEquip);
			}

			return;
		}

		if (
			!RightSwordEquipped
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

}
