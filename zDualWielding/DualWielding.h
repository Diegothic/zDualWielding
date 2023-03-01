// Supported with union (c) 2020 Union team
// Union HEADER file

namespace GOTHIC_ENGINE {
	class DualWielding {
	public:
		static const char* NPC_MDS_DUALWIELDING;

		static const char* NPC_NODE_LEFTSWORD;
		static const char* NPC_NODE_LEFTHANDSWORD;

	private:
		DualWielding() {};
		~DualWielding();
		
	public:
		static DualWielding* GetInstance();

		bool32 HasLeftWeaponSlots(oCNpc* Npc) const;
		void CreateLeftWeaponSlots(oCNpc* Npc) const;
		void CreateNode(
			oCNpc*           Npc, 
			zCModelNodeInst* TemplateNode, 
			const zSTRING&   NodeName, 
			zMAT4            NodeTrafo
		) const;

		void EquipDualWeapons(
			oCNpc*  Npc, 
			oCItem* RightHandWeapon, 
			oCItem* LeftHandWeapon
		) const;
		void UnequipLeftWeapon(oCNpc* Npc) const;
		oCItem* GetEquippedLeftSword(oCNpc* Npc) const;
		bool32 IsWeaponForDualWielding(oCItem* Weapon) const;

		void ApplyDualAnimations(oCNpc* Npc) const;
		void RemoveDualAnimations(oCNpc* Npc) const;

	private:
		static DualWielding* Instance;
	};
}
