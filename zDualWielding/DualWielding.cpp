// Supported with union (c) 2020 Union team
// Union SOURCE file

namespace GOTHIC_ENGINE {
	const char* DualWielding::NPC_MDS_DUALWIELDING = "HUMANS_DUAL.MDS";
	const char* DualWielding::NPC_NODE_LEFTSWORD = "ZS_LEFTSWORD";
	const char* DualWielding::NPC_NODE_LEFTHANDSWORD = "ZS_LEFTHANDSWORD";

	DualWielding::DualWielding(oCNpc* Npc) : Npc(Npc) {
		if (!HasLeftWeaponSlots()) {
			CreateLeftWeaponSlots();
		}
	}

	DualWielding::~DualWielding() { }

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
			NodeTrafo.Translate(zVEC3(0.0f, 35.0f, 1.5f));

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
		oCItem*          LeftSwordEquipped = Npc->GetSlotItem(NPC_NODE_LEFTSWORD);

		Npc->RemoveFromSlot(NPC_NODE_LEFTSWORD, 1, 1);
		Npc->UnequipItem(LeftSwordEquipped);
		Npc->GetModel()->SetNodeVisual(LeftSwordNode, Null, 0);
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
