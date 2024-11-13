#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerStart.h"
#include "CustomPawnStart.generated.h"

UCLASS(Abstract)
class CFORENGINES_API ACustomPawnStart : public APlayerStart
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool _IsAI;

public:
	bool GetIsAI();
};
