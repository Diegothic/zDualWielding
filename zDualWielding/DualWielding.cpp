// Supported with union (c) 2020 Union team
// Union SOURCE file

namespace GOTHIC_ENGINE {
	const char* DualWielding::NPC_MDS_DUALWIELDING = "HUMANS_DUAL.MDS";
	const char* DualWielding::NPC_NODE_LEFTSWORD = "ZS_LEFTSWORD";
	const char* DualWielding::NPC_NODE_LEFTHANDSWORD = "ZS_LEFTHANDSWORD";

	oCItem* DualWielding::CombinedSword = Null;

	DualWielding::DualWielding(oCNpc* Npc) 
		: Npc(Npc) {
		if (!HasLeftWeaponSlots()) {
			CreateLeftWeaponSlots();
		}
	}

	DualWielding::~DualWielding() {
	}

	bool32 DualWielding::HasLeftWeaponSlots() const {
		TNpcSlot*        LeftWeaponInvSlot = Npc->GetInvSlot(NPC_NODE_LEFTSWORD);
		zCModel*         NpcModel          = Npc->GetModel();
		zCModelNodeInst* LeftSwordNode     = NpcModel->SearchNode(NPC_NODE_LEFTSWORD);
		zCModelNodeInst* LeftHandSwordNode = NpcModel->SearchNode(NPC_NODE_LEFTHANDSWORD);
		return LeftWeaponInvSlot && LeftSwordNode && LeftHandSwordNode;
	}

	void DualWielding::CreateLeftWeaponSlots() const {
		TNpcSlot*        LeftWeaponInvSlot = Npc->GetInvSlot(NPC_NODE_LEFTSWORD);
		zCModel*         NpcModel          = Npc->GetModel();
		zCModelNodeInst* LongswordNode     = NpcModel->SearchNode(NPC_NODE_LONGSWORD);
		zCModelNodeInst* LeftHandNode      = NpcModel->SearchNode(NPC_NODE_LEFTHAND);
		zCModelNodeInst* LeftSwordNode     = NpcModel->SearchNode(NPC_NODE_LEFTSWORD);
		zCModelNodeInst* LeftHandSwordNode = NpcModel->SearchNode(NPC_NODE_LEFTHANDSWORD);

		if (!LeftWeaponInvSlot) {
			Npc->CreateInvSlot(NPC_NODE_LEFTSWORD);
			Npc->UpdateSlots();
		}

		if (!LeftSwordNode) {
			zMAT4 NodeTrafo = LongswordNode->trafo;
			NodeTrafo.PostRotateX(180.0f);
			NodeTrafo.PostRotateY(-60.0f);
			NodeTrafo.PostRotateZ(10.0f);
			NodeTrafo.Translate(zVEC3(0.0f, 35.0f, -5.0f));

			CreateNode(LongswordNode, NPC_NODE_LEFTSWORD, NodeTrafo);
		}

		if (!LeftHandSwordNode) {
			zMAT4 NodeTrafo = LeftHandNode->trafo;
			NodeTrafo.PostRotateX(180.0f);
			NodeTrafo.Translate(zVEC3(0.0f, -5.0f, 0.0f));

			CreateNode(LeftHandNode, NPC_NODE_LEFTHANDSWORD, NodeTrafo);
		}
	}

	void DualWielding::CreateNode(
		zCModelNodeInst* TemplateNode, 
		const zSTRING&   NodeName, 
		zMAT4            NodeTrafo
	) const {
		zCModel*         NpcModel    = Npc->GetModel();
		zCModelNode*     NewNode     = new zCModelNode();
		zCModelNodeInst* NewNodeInst = new zCModelNodeInst();

		NewNode->nodeName = NodeName;
		NewNode->parentNode = TemplateNode->protoNode->parentNode;
		NewNode->lastInstNode = Null;
		NewNode->trafo = NodeTrafo;
		NewNodeInst->InitByModelProtoNode(NewNode);
		NewNodeInst->parentNode = TemplateNode->parentNode;
		NewNodeInst->trafo = NodeTrafo;
		NewNode->lastInstNode = NewNodeInst;

		NpcModel->nodeList.Insert(NewNodeInst);
	}

	void DualWielding::LoadWeaponState() const {
		if (!HasLeftWeaponSlots()) {
			return;
		}

		if (Npc->IsSlotFree(NPC_NODE_LEFTSWORD)) {
			return;
		}

		RemoveDualAnimations();

		zCModel*         NpcModel      = Npc->GetModel();
		zCModelNodeInst* SwordNode     = NpcModel->SearchNode(NPC_NODE_SWORD);
		zCModelNodeInst* LongswordNode = NpcModel->SearchNode(NPC_NODE_LONGSWORD);
		zCModelNodeInst* LeftSwordNode = NpcModel->SearchNode(NPC_NODE_LEFTSWORD);

		if (Npc->fmode == 0) {
			oCItem* LeftSwordEquipped  = Npc->GetSlotItem(NPC_NODE_LEFTSWORD);
			oCItem* RightSwordEquipped = Npc->GetSlotItem(NPC_NODE_SWORD);
			Npc->PutInSlot(NPC_NODE_LEFTSWORD, LeftSwordEquipped, 1);

			NpcModel->SetNodeVisual(SwordNode, Null, 0);
			NpcModel->SetNodeVisual(LongswordNode, RightSwordEquipped->visual, 0);
			NpcModel->SetNodeVisual(LeftSwordNode, LeftSwordEquipped->visual, 0);
		} else {
			NpcModel->SetNodeVisual(SwordNode, Null, 0);
		}

		ApplyDualAnimations();
	}

	void DualWielding::EquipDualWeapons(
		oCItem* RightHandWeapon,
		oCItem* LeftHandWeapon
	) const {
		zCModel*         NpcModel      = Npc->GetModel();
		zCModelNodeInst* LongswordNode = NpcModel->SearchNode(NPC_NODE_LONGSWORD);
		zCModelNodeInst* SwordNode     = NpcModel->SearchNode(NPC_NODE_SWORD);
		zCModelNodeInst* LeftSwordNode = NpcModel->SearchNode(NPC_NODE_LEFTSWORD);

		Npc->EquipItem(LeftHandWeapon);
		Npc->EquipItem(RightHandWeapon);
		Npc->PutInSlot(NPC_NODE_SWORD, RightHandWeapon, 1);
		Npc->PutInSlot(NPC_NODE_LEFTSWORD, LeftHandWeapon, 1);

		NpcModel->SetNodeVisual(SwordNode, Null, 0);
		NpcModel->SetNodeVisual(LongswordNode, RightHandWeapon->visual, 0);
		NpcModel->SetNodeVisual(LeftSwordNode, LeftHandWeapon->visual, 0);
	}

	void DualWielding::UnequipLeftWeapon() const {
		zCModel*         NpcModel          = Npc->GetModel();
		zCModelNodeInst* LeftSwordNode     = NpcModel->SearchNode(NPC_NODE_LEFTSWORD);
		zCModelNodeInst* LongswordNode     = NpcModel->SearchNode(NPC_NODE_LONGSWORD);
		oCItem*          LeftSwordEquipped = Npc->GetSlotItem(NPC_NODE_LEFTSWORD);

		Npc->RemoveFromSlot(NPC_NODE_LEFTSWORD, 1, 1);
		Npc->UnequipItem(LeftSwordEquipped);
		Npc->GetModel()->SetNodeVisual(LeftSwordNode, Null, 0);
		Npc->GetModel()->SetNodeVisual(LongswordNode, Null, 0);
	}

	void DualWielding::UnequipRightWeapon() const {
		zCModel*         NpcModel           = Npc->GetModel();
		zCModelNodeInst* LongswordNode      = NpcModel->SearchNode(NPC_NODE_LONGSWORD);
		oCItem*          RightSwordEquipped = Npc->GetSlotItem(NPC_NODE_SWORD);

		Npc->RemoveFromSlot(NPC_NODE_SWORD, 1, 1);
		Npc->UnequipItem(RightSwordEquipped);
		Npc->GetModel()->SetNodeVisual(LongswordNode, Null, 0);
	}

	oCItem* DualWielding::GetEquippedLeftSword() const {
		return Npc->GetSlotItem(NPC_NODE_LEFTSWORD);
	}

	oCItem* DualWielding::GetLeftSwordInHand() const {
		return Npc->GetSlotItem(NPC_NODE_LEFTHAND);
	}

	void DualWielding::ChangeWeaponMode(
		zSTRING const& NewWeaponMode, 
		int FromFightMode
	) const {
		oCItem* LeftSwordEquipped = Npc->GetSlotItem(NPC_NODE_LEFTSWORD);
		oCItem* LeftSwordInHand   = Npc->GetSlotItem(NPC_NODE_LEFTHAND);

		if (NewWeaponMode.IsEmpty() 
			&& FromFightMode == NPC_WEAPON_1HS 
			&& LeftSwordInHand
			&& IsWeaponForDualWielding(LeftSwordInHand)
			) {
			SheathSwords();
			return;
		}
		
		if (NewWeaponMode.Compare("1H") 
			&& LeftSwordEquipped 
			&& !LeftSwordInHand
			){
			DrawSwords();
			return;
		};
	}

	void DualWielding::DrawSwords() const {
		oCItem*          LeftSwordEquipped = Npc->GetSlotItem(NPC_NODE_LEFTSWORD);
		zCModel*         NpcModel          = Npc->GetModel();
		zCModelNodeInst* SwordNode         = NpcModel->SearchNode(NPC_NODE_SWORD);
		zCModelNodeInst* LongswordNode     = NpcModel->SearchNode(NPC_NODE_LONGSWORD);
		zCModelNodeInst* LeftHandNode      = NpcModel->SearchNode(NPC_NODE_LEFTHAND);
		zCModelNodeInst* LeftSwordNode     = NpcModel->SearchNode(NPC_NODE_LEFTSWORD);
		zCModelNodeInst* LeftHandSwordNode = NpcModel->SearchNode(NPC_NODE_LEFTHANDSWORD);

		Npc->RemoveFromSlot(NPC_NODE_LEFTHAND, 1, 1);
		Npc->PutInSlot(NPC_NODE_LEFTHAND, LeftSwordEquipped, 1);

		NpcModel->SetNodeVisual(SwordNode, Null, 0);
		NpcModel->SetNodeVisual(LeftSwordNode, Null, 0);
		NpcModel->SetNodeVisual(LongswordNode, Null, 0);
		NpcModel->SetNodeVisual(LeftHandNode, Null, 0);
		NpcModel->SetNodeVisual(LeftHandSwordNode, LeftSwordEquipped->visual, 0);
	}

	void DualWielding::SheathSwords() const {
		oCItem*          RightSwordEquipped = Npc->GetSlotItem(NPC_NODE_SWORD);
		oCItem*          LeftSwordInHand    = Npc->GetSlotItem(NPC_NODE_LEFTHAND);
		zCModel*         NpcModel           = Npc->GetModel();
		zCModelNodeInst* SwordNode          = NpcModel->SearchNode(NPC_NODE_SWORD);
		zCModelNodeInst* LongswordNode      = NpcModel->SearchNode(NPC_NODE_LONGSWORD);
		zCModelNodeInst* RightHandNode      = NpcModel->SearchNode(NPC_NODE_RIGHTHAND);
		zCModelNodeInst* LeftHandNode       = NpcModel->SearchNode(NPC_NODE_LEFTHAND);
		zCModelNodeInst* LeftSwordNode      = NpcModel->SearchNode(NPC_NODE_LEFTSWORD);
		zCModelNodeInst* LeftHandSwordNode  = NpcModel->SearchNode(NPC_NODE_LEFTHANDSWORD);

		Npc->RemoveFromSlot(NPC_NODE_LEFTHAND, 1, 1);
		Npc->PutInSlot(NPC_NODE_LEFTHAND, Null, 1);

		NpcModel->SetNodeVisual(LeftSwordNode, LeftSwordInHand->visual, 0);
		NpcModel->SetNodeVisual(LongswordNode, RightSwordEquipped->visual, 0);
		NpcModel->SetNodeVisual(SwordNode, Null, 0);
		NpcModel->SetNodeVisual(LeftHandNode, Null, 0);
		NpcModel->SetNodeVisual(RightHandNode, Null, 0);
		NpcModel->SetNodeVisual(LeftHandSwordNode, Null, 0);
	}

	oCItem* DualWielding::GetWeaponForDamage()
	{
		oCItem* LeftSwordEquipped  = GetEquippedLeftSword();
		oCItem* RightSwordEquipped = Npc->GetSlotItem(NPC_NODE_RIGHTHAND);

		if (!RightSwordEquipped
			|| !LeftSwordEquipped
			|| !IsWeaponForDualWielding(RightSwordEquipped)
			) {
			return Null;
		}

		zCModel*          NpcModel      = Npc->GetModel();
		zCModelNodeInst*  RightHandNode = NpcModel->SearchNode(NPC_NODE_RIGHTHAND);
		zCModelNodeInst*  LeftHandNode  = NpcModel->SearchNode(NPC_NODE_LEFTHAND);
		zCModelNodeInst** HitLimbs      = Npc->GetAnictrl()->hitlimb;

		bool32 DamageFromRightWeapon = False;
		bool32 DamageFromLeftWeapon  = False;
		for (int i = 0; i < ANI_HITLIMB_MAX; i++) {
			if (HitLimbs[i] == RightHandNode) {
				DamageFromRightWeapon = True;
			}

			if (HitLimbs[i] == LeftHandNode) {
				DamageFromLeftWeapon = True;
			}
		}

		if (DamageFromRightWeapon && DamageFromLeftWeapon) {
			if (CombinedSword) {
				CombinedSword->Release();
				CombinedSword = Null;
			}

			CombinedSword = RightSwordEquipped->CreateCopy()->CastTo<oCItem>();
			CombinedSword->AddRef();
			CombinedSword->damageTotal += LeftSwordEquipped->damageTotal;
			CombinedSword->flags &= LeftSwordEquipped->flags;
			for (int i = 0; i < oEDamageIndex_MAX; i++) {
				CombinedSword->damage[i] += LeftSwordEquipped->damage[i];
			}

			return CombinedSword;
		}

		if (DamageFromRightWeapon) {
			return RightSwordEquipped;
		}

		if (DamageFromLeftWeapon) {
			return LeftSwordEquipped;
		}

		return Null;
	}

	void DualWielding::DropWeapons(bool32 WasInFightMode, oCItem* RightSword, oCItem* LeftSword) {
		zCModel*         NpcModel          = Npc->GetModel();
		zCModelNodeInst* SwordNode         = NpcModel->SearchNode(NPC_NODE_SWORD);
		zCModelNodeInst* LongswordNode     = NpcModel->SearchNode(NPC_NODE_LONGSWORD);
		zCModelNodeInst* LeftHandNode      = NpcModel->SearchNode(NPC_NODE_LEFTHAND);
		zCModelNodeInst* LeftSwordNode     = NpcModel->SearchNode(NPC_NODE_LEFTSWORD);
		zCModelNodeInst* LeftHandSwordNode = NpcModel->SearchNode(NPC_NODE_LEFTHANDSWORD);

		if (WasInFightMode) {
			UnequipRightWeapon();
			UnequipLeftWeapon();

			NpcModel->SetNodeVisual(LeftHandSwordNode, Null, 0);
			NpcModel->SetNodeVisual(LeftHandNode, Null, 0);

			if (LeftSword) {
				ogame->GetGameWorld()->AddVob(LeftSword);
				ogame->GetGameWorld()->EnableVob(LeftSword, Null);
				zMAT4 Trafo = NpcModel->GetTrafoNodeToModel(LeftHandNode);
				LeftSword->SetPositionWorld(Npc->GetPositionWorld() + Trafo.GetTranslation());
				LeftSword->physicsEnabled = True;
				LeftSword->SetSleeping(False);
				LeftSword->Release();
			}
		}
		else {
			UnequipRightWeapon();
			UnequipLeftWeapon();

			Npc->EquipItem(RightSword);
			Npc->PutInSlot(NPC_NODE_SWORD, RightSword, 1);
			EquipDualWeapons(RightSword, LeftSword);
			ApplyDualAnimations();

			LeftSword->Release();
			RightSword->Release();
		}
	}

	void DualWielding::ApplyDualAnimations() const {
		Npc->ApplyOverlay(NPC_MDS_DUALWIELDING);
	}

	void DualWielding::RemoveDualAnimations() const {
		Npc->RemoveOverlay(NPC_MDS_DUALWIELDING);
	}

	bool32 DualWielding::IsWeaponForDualWielding(oCItem* Weapon) {
		return Weapon->HasFlag(ITM_FLAG_SWD) || Weapon->HasFlag(ITM_FLAG_AXE);
	}
}
