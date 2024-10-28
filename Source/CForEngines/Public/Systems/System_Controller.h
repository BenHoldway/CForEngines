#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "System_Controller.generated.h"

class USphereComponent;
class USystem;

UCLASS(Abstract)
class CFORENGINES_API ASystem_Controller : public AActor
{
	GENERATED_BODY()

public:
	ASystem_Controller();

protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<USphereComponent> _Collider;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> _Mesh;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<USystem> _System;
	
	virtual void BeginPlay() override;

public:
	UFUNCTION()
	void Handle_SystemValueChanged(float max, float current);
	UFUNCTION()
	void Handle_SystemRegenerated();
	UFUNCTION()
	void Handle_SystemDepleted();
};
