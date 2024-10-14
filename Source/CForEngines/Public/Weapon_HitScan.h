#pragma once

#include "CoreMinimal.h"
#include "Weapon_Base.h"
#include "Weapon_HitScan.generated.h"

UCLASS(Abstract)
class CFORENGINES_API AWeapon_HitScan : public AWeapon_Base
{
	GENERATED_BODY()

public:
	virtual void Fire() override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float _Damage;
};
