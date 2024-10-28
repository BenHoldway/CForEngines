#pragma once

#include "CoreMinimal.h"
#include "GameRule.h"
#include "GameRule_Systems.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CFORENGINES_API UGameRule_Systems : public UGameRule
{
	GENERATED_BODY()

public:
	UGameRule_Systems();

protected:
	virtual void BeginPlay() override;

public:
};
