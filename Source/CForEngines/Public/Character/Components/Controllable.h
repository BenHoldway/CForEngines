#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Controllable.generated.h"

UINTERFACE()
class UControllable : public UInterface
{
	GENERATED_BODY()
};

class CFORENGINES_API IControllable
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void AddPoints(int points);
};
