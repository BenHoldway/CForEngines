#pragma once

#include "CoreMinimal.h"
#include "GameRule.h"
#include "GameRule_Target.generated.h"

class UTarget;
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FGameRuleTargetRequestTargetsSignature);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CFORENGINES_API UGameRule_Target : public UGameRule
{
	GENERATED_BODY()

public:
	UGameRule_Target();

	static inline FGameRuleTargetRequestTargetsSignature OnRequestTargets;

	virtual void Init() override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TObjectPtr<UTarget>> _Targets;

	int _AmountRemaining;

private:
	UFUNCTION()
	void Handle_TargetRegistered(UTarget* target);
	UFUNCTION()
	void Handle_TargetDestroyed(UTarget* target, AController* causer, int targetValue);
};
