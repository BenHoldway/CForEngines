#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Inputable.generated.h"

class UBehaviorTree;

UINTERFACE()
class UInputable : public UInterface
{
	GENERATED_BODY()
};

class UInputMappingContext;
class CFORENGINES_API IInputable
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Input_Move(FVector2D value);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Input_SprintPressed();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Input_SprintReleased();
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Input_Look(FVector2D value);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Input_JumpPressed();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Input_JumpReleased();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Input_FirePressed();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Input_FireReleased();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Input_CrouchPressed();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Input_CrouchReleased();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Input_Interact();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	UInputMappingContext* GetMappingContext();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	UBehaviorTree* GetBehaviorTree();
};

