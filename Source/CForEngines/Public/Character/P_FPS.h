#pragma once

#include "CoreMinimal.h"
#include "Components/Inputable.h"
#include "GameFramework/Character.h"
#include "P_FPS.generated.h"

class USpotLightComponent;
class USphereComponent;
class UMovementComponent;
class AWeapon_Base;
class UHealthComponent;
class UStaminaComponent;
class UCharacterMovementComponent;
class UCameraComponent;
class UCapsuleComponent;
class UBehaviorTree;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPawnDeadSignature, AController*, causer);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FPawnDamagedSignature, float, newHealth, float, maxHealth, float, changeInHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FPawnStaminaChangedSignature, float, currentStamina, float, maxStamina, float, changedStamina);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FShowInteractPromptSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FHideInteractPromptSignature);


UCLASS()
class CFORENGINES_API AP_FPS : public ACharacter, public IInputable 
{
	GENERATED_BODY()

public:
	AP_FPS();

	virtual void Input_StartedMove_Implementation(FVector2D value) override;
	virtual void Input_Move_Implementation(FVector2D value) override;
	virtual void Input_MoveCancelled_Implementation(FVector2D value) override;
	virtual void Input_Look_Implementation(FVector2D value) override;
	virtual void Input_JumpPressed_Implementation() override;
	virtual void Input_JumpReleased_Implementation() override;
	virtual void Input_FirePressed_Implementation() override;
	virtual void Input_FireReleased_Implementation() override;
	virtual void Input_SprintPressed_Implementation() override;
	virtual void Input_SprintReleased_Implementation() override;
	virtual void Input_CrouchPressed_Implementation() override;
	virtual void Input_CrouchReleased_Implementation() override;
	virtual void Input_Interact_Implementation() override;
	virtual void Input_FlashlightToggle_Implementation(bool isFlashlightOn) override;

	virtual UInputMappingContext* GetMappingContext_Implementation() override;
	virtual void DisableInput(APlayerController* PlayerController) override;

	virtual UBehaviorTree* GetBehaviorTree_Implementation() override;
	
	void OverrideSkeletonMesh(USkeletalMesh* mesh);
	void OverrideFlashlightColour(FLinearColor colour);

	UPROPERTY(BlueprintAssignable)
	FPawnDeadSignature OnPawnDead;
	FPawnDamagedSignature OnPawnDamaged;
	FPawnStaminaChangedSignature OnPawnStaminaChanged;
	FShowInteractPromptSignature OnShowInteractPrompt;
	FHideInteractPromptSignature OnHideInteractPrompt;
	

protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<UAudioComponent> _AudioComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<UCameraComponent> _Camera;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<USpotLightComponent> _Flashlight;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<UCapsuleComponent> _InteractionCollider2;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<UArrowComponent> _Arrow;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<UHealthComponent> _Health;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<UStaminaComponent> _Stamina2;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<USceneComponent> _WeaponAttachPoint;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AWeapon_Base> _DefaultWeapon;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<AWeapon_Base> _WeaponRef;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UInputMappingContext> _MappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UBehaviorTree> _BehaviorTree;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UCharacterMovementComponent> _MovementComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float _InteractionRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float _SprintMoveSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float _NormalMoveSpeed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FVector _CrouchEyeOffset;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float _CrouchSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	AActor* _Interactable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USoundBase> _FootstepSounds;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float _FootstepTimeStep;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USoundBase> _FlashlightToggleSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USoundBase> _DeadSound;
	
	FTimerHandle PlayFootstepSoundTimer;
	UPROPERTY()
	TObjectPtr<UAudioComponent> _FootstepAudioComponent;
	UPROPERTY()
	TObjectPtr<UAudioComponent> _FlashlightAudioComponent;
	UPROPERTY()
	TObjectPtr<UAudioComponent> _DeadAudioComponent;
	
	FTimerHandle _CrouchTimer;

	float _FlashlightNormalIntensity;

	void BeginPlay() override;

	void StartPlayingFootstepSounds();
	UFUNCTION()
	void PlayFootstepSound();
	void StopPlayingFootstepSounds();

private:
	UFUNCTION()
	void Handle_HealthDead(AController* causer);
	UFUNCTION()
	void Handle_HealthDamaged(float currentHealth, float maxHealth, float changedHealth);

	UFUNCTION()
	void Handle_InteractionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void Handle_InteractionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	UFUNCTION()
	void Handle_StoppedSprinting();
	UFUNCTION()
	void Handle_ChangeStamina(float currentStamina, float maxStamina, float changedStamina);

	AActor* GetClosest(TArray<FHitResult> hit);
};
