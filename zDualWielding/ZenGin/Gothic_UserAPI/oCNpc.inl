// Supported with union (c) 2020 Union team

// User API for oCNpc
// Add your methods here

// oCItem* GetWeapon() zCall( 0x007377A0 );
oCItem* GetWeapon_Union();

// void EquipWeapon( oCItem* ) zCall( 0x0073A030 );
void EquipWeapon_Union(oCItem*);

// void SetWeaponMode2_novt( zSTRING const& ) zCall( 0x00738C60 );
void SetWeaponMode2_novt_Union(zSTRING const&);

// void DropUnconscious(float, oCNpc*) zCall(0x00735EB0);
void DropUnconscious_Union(float, oCNpc*);
