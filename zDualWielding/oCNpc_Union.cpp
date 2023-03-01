// Supported with union (c) 2020 Union team
// Union SOURCE file

namespace GOTHIC_ENGINE {

	// void EquipWeapon( oCItem* ) zCall( 0x0073A030 );
	HOOK Hook_oCNpc_EquipWeapon_Union PATCH(&oCNpc::EquipWeapon, &oCNpc::EquipWeapon_Union);
	void oCNpc::EquipWeapon_Union(oCItem* WeaponToEquip) {
		DualWielding*    DW                 = DW->GetInstance();
		zCModel*         NpcModel           = GetModel();
		zCModelNodeInst* LongswordNode      = NpcModel->SearchNode(NPC_NODE_LONGSWORD);
		oCItem*          LeftSwordEquipped  = DW->GetEquippedLeftSword(this);
		oCItem*          RightSwordEquipped = GetSlotItem(NPC_NODE_SWORD);

		if (!DW->HasLeftWeaponSlots(this)) {
			DW->CreateLeftWeaponSlots(this);
		}

		DW->RemoveDualAnimations(this);
		
		if (LeftSwordEquipped && RightSwordEquipped) {
			if (WeaponToEquip == LeftSwordEquipped) {
				DW->UnequipLeftWeapon(this);
				EquipItem(RightSwordEquipped);
				PutInSlot(NPC_NODE_SWORD, RightSwordEquipped, 1);
				return;
			}

			DW->UnequipLeftWeapon(this);
			RemoveFromSlot(NPC_NODE_SWORD, 1, 1);
			UnequipItem(RightSwordEquipped);
			NpcModel->SetNodeVisual(LongswordNode, Null, 0);

			if (WeaponToEquip != RightSwordEquipped) {
				THISCALL(Hook_oCNpc_EquipWeapon_Union)(WeaponToEquip);
			}

			return;
		}

		if (
			!RightSwordEquipped
			|| RightSwordEquipped == WeaponToEquip
			|| !DW->IsWeaponForDualWielding(RightSwordEquipped)
			|| !DW->IsWeaponForDualWielding(WeaponToEquip)
			) {
			THISCALL(Hook_oCNpc_EquipWeapon_Union)(WeaponToEquip);
			return;
		}

		DW->EquipDualWeapons(this, RightSwordEquipped, WeaponToEquip);
		DW->ApplyDualAnimations(this);
	}

}
