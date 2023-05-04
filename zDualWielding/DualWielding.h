// Supported with union (c) 2020 Union team
// Union HEADER file

namespace GOTHIC_ENGINE {
	class DualWielding {
	public:
		static const char* NPC_MDS_DUALWIELDING;

		static const char* NPC_NODE_LEFTSWORD;
		static const char* NPC_NODE_LEFTHANDSWORD;

	public:
		DualWielding(oCNpc* Npc);
		~DualWielding();
		
	public:
		bool32 HasLeftWeaponSlots() const;
		void CreateLeftWeaponSlots() const;
		void CreateNode(
			zCModelNodeInst* TemplateNode, 
			const zSTRING&   NodeName, 
			zMAT4            NodeTrafo
		) const;
		void LoadWeaponState() const;

		void EquipDualWeapons(
			oCItem* RightHandWeapon, 
			oCItem* LeftHandWeapon
		) const;
		void UnequipLeftWeapon() const;
		void UnequipRightWeapon() const;
		oCItem* GetEquippedLeftSword() const;

		void ChangeWeaponMode(
			zSTRING const& NewWeaponMode, 
			int FromFightMode
		) const;
		void DrawSwords() const;
		void SheathSwords() const;
		oCItem* GetWeaponForDamage();

		void ApplyDualAnimations() const;
		void RemoveDualAnimations() const;

	public:
		static bool32 IsWeaponForDualWielding(oCItem* Weapon);

	private:
		oCNpc* Npc;
		oCItem* CombinedSword;
	};
}
