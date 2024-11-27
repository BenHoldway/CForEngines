#pragma once

#include "CoreMinimal.h"
#include "Turnoffable.h"
#include "GameFramework/Actor.h"
#include "LightObject.generated.h"

class URectLightComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FLightRegisteredSignature, ALightObject*, light);

UCLASS(Abstract)
class CFORENGINES_API ALightObject : public AActor, public ITurnoffable
{
	GENERATED_BODY()

public:
	ALightObject();

	static inline FLightRegisteredSignature OnRegister;

	virtual void TurnOff_Implementation() override;
	virtual void TurnOn_Implementation() override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<URectLightComponent> _Light;

	float _Intensity;
};
