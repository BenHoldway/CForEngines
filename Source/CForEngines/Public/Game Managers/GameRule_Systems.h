#pragma once

#include "CoreMinimal.h"
#include "GameRule.h"
#include "GameRule_Systems.generated.h"

class ASystem_Controller;
class USystem;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FGameRuleSystemRequestTargetsSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGameRuleSystemDepletedSignature, USystem*, system);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CFORENGINES_API UGameRule_Systems : public UGameRule
{
	GENERATED_BODY()

public:
	UGameRule_Systems();

	UPROPERTY(BlueprintAssignable)
	FGameRuleSystemDepletedSignature OnSystemDepleted;
	
	static inline FGameRuleSystemRequestTargetsSignature OnRequestSystems;

	virtual void Init() override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<TObjectPtr<ASystem_Controller>> _Systems;

	UFUNCTION()
	void Handle_SystemRegistered(ASystem_Controller* systemController);
	UFUNCTION()
	void Handle_SystemDepleted(ASystem_Controller* systemController, USystem* system);

public:
};
