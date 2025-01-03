#pragma once

#include "CoreMinimal.h"
#include "Character/Components/Interactable.h"
#include "GameFramework/Actor.h"
#include "System_Controller.generated.h"

enum ESystemType : int;
class UBoxComponent;
class USystem;
class ASystem_Controller;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSystemRegisteredSignature, ASystem_Controller*, system);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FSystemValueChangedSignature, ESystemType, systemType, float, max, float, current);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FSystemDepletedSignature, ASystem_Controller*, systemController,
	ESystemType, systemType);

UCLASS(Abstract)
class CFORENGINES_API ASystem_Controller : public AActor, public IInteractable
{
	GENERATED_BODY()

public:
	ASystem_Controller();

	UPROPERTY(BlueprintAssignable)
	FSystemDepletedSignature OnDepleted;
	FSystemValueChangedSignature OnSystemValueChanged;
	
	static inline FSystemRegisteredSignature OnSystemRegister;

protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UBoxComponent> _Collider2;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> _Mesh;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<USystem> _System;
	
	virtual void BeginPlay() override;

	virtual void Interact_Implementation() override;

public:
	UFUNCTION()
	void Handle_SystemValueChanged(float max, float current);
	UFUNCTION()
	void Handle_SystemRegenerated();
	UFUNCTION()
	void Handle_SystemDepleted();
};
