// Supported with union (c) 2020 Union team
// Union SOURCE file

namespace GOTHIC_ENGINE {

	// oCItem* GetWeapon() zCall( 0x007377A0 );
	HOOK Hook_oCNpc_GetWeapon_Union PATCH(&oCNpc::GetWeapon, &oCNpc::GetWeapon_Union);
	oCItem* oCNpc::GetWeapon_Union() {
		oCItem* Result = THISCALL(Hook_oCNpc_GetWeapon_Union)();

		zCModel*         NpcModel      = GetModel();
		zCModelNodeInst* LongswordNode = NpcModel->SearchNode(NPC_NODE_LONGSWORD);
		zCModelNodeInst* LeftHandNode  = NpcModel->SearchNode(NPC_NODE_LEFTHAND);

		if (!LongswordNode || !LeftHandNode) {
			return Result;
		}

		DualWielding DualWielder(this);
		oCItem* WeaponForDamage = DualWielder.GetWeaponForDamage();
		if (WeaponForDamage) {
			return WeaponForDamage;
		}

		return Result;
	}

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

	// void DropUnconscious(float, oCNpc*) zCall(0x00735EB0);
	HOOK Hook_oCNpc_DropUnconscious_Union PATCH(&oCNpc::DropUnconscious, &oCNpc::DropUnconscious_Union);
	void oCNpc::DropUnconscious_Union(float HitAngle, oCNpc* Instigator) {
		DualWielding DualWielder(this);

		bool32  WasInFightMode  = fmode == 0 ? False : True;
		bool32  WasDualWielding = False;
		oCItem* LeftSword       = Null;
		oCItem* RightSword      = Null;
		if (WasInFightMode) {
			oCItem* LeftSwordInHand = DualWielder.GetLeftSwordInHand();
			if (LeftSwordInHand
				&& DualWielding::IsWeaponForDualWielding(LeftSwordInHand)
			) {
				WasDualWielding = True;
				LeftSword = LeftSwordInHand;
				LeftSword->AddRef();
			}
		}
		else {
			LeftSword  = DualWielder.GetEquippedLeftSword();
			RightSword = GetSlotItem(NPC_NODE_SWORD);
			if (LeftSword && RightSword) {
				WasDualWielding = True;
				LeftSword->AddRef();
				RightSword->AddRef();
			}
		}

		THISCALL(Hook_oCNpc_DropUnconscious_Union)(HitAngle, Instigator);

		if (!WasDualWielding) {
			return;
		}

		DualWielder.DropWeapons(WasInFightMode, RightSword, LeftSword);
	}
}
