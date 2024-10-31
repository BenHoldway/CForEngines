#pragma once

#include "CoreMinimal.h"
#include "GameRule.h"
#include "GameRule_Clock.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CFORENGINES_API UGameRule_Clock : public UGameRule
{
	GENERATED_BODY()

public:
	UGameRule_Clock();

	virtual void Init() override;

protected:
UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float _Time;
	
	FTimerManager* _TimerManager;
	FTimerHandle _ClockTimer;

	void Handle_ClockCompleted();

public:
};
