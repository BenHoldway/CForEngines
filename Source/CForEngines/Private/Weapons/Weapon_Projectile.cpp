#include "Weapons/Weapon_Projectile.h"

#include "Weapons/Projectile.h"
#include "Components/ArrowComponent.h"

void AWeapon_Projectile::Fire()
{
	UWorld* world = GetWorld();
	if(world == nullptr || _ProjectileClass == nullptr) { return; }

	FActorSpawnParameters spawnParams;
	spawnParams.Owner = GetOwner();
	spawnParams.Instigator = GetInstigator();
	spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
	world->SpawnActor(_ProjectileClass, &_Muzzle->GetComponentTransform(), spawnParams);
	
	Super::Fire();
}
