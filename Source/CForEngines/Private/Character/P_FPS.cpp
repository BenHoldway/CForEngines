#include "Character/P_FPS.h"

#include "Camera/CameraComponent.h"
#include "Character/Components/HealthComponent.h"
#include "Character/Components/StaminaComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/InputSettings.h"
#include "Weapons/Weapon_Base.h"

AP_FPS::AP_FPS()
{
	_Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	_Camera->SetupAttachment(RootComponent);
	_Health = CreateDefaultSubobject<UHealthComponent>(TEXT("Health"));
	_Stamina2 = CreateDefaultSubobject<UStaminaComponent>(TEXT("Stamina"));

	_WeaponAttachPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Weapon Attack Point"));
	_WeaponAttachPoint->SetupAttachment(_Camera);
}

void AP_FPS::BeginPlay()
{
	Super::BeginPlay();

	_Health->OnDead.AddUniqueDynamic(this, &AP_FPS::Handle_HealthDead);
	_Health->OnDamaged.AddUniqueDynamic(this, &AP_FPS::Handle_HealthDamaged);
	_Stamina2->OnStoppedSprinting.AddUniqueDynamic(this, &AP_FPS::Handle_StoppedSprinting);
	_Stamina2->OnStaminaValueChanged.AddUniqueDynamic(this, &AP_FPS::Handle_ChangeStamina);

	if(_DefaultWeapon)
	{
		FActorSpawnParameters spawnParams;
		spawnParams.Owner = this;
		spawnParams.Instigator = this;
		_WeaponRef = GetWorld()->SpawnActor<AWeapon_Base>(_DefaultWeapon, _WeaponAttachPoint->GetComponentTransform(), spawnParams);
		_WeaponRef->AttachToComponent(_WeaponAttachPoint, FAttachmentTransformRules::SnapToTargetIncludingScale);
	}

	
	_MovementComponent = GetCharacterMovement();
	_MovementComponent->MaxWalkSpeed = _NormalMoveSpeed;
}

/*void AP_FPS::OnStartCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust)
{
	if(HalfHeightAdjust == 0.0f || !_Camera) { return; }

	float startEyeHeight = BaseEyeHeight;
	Super::OnStartCrouch(HalfHeightAdjust, ScaledHalfHeightAdjust);

	_CrouchEyeOffset.Z += (startEyeHeight - BaseEyeHeight) + HalfHeightAdjust;
	_Camera->SetRelativeLocation(FVector(0, 0, BaseEyeHeight));
}

void AP_FPS::OnEndCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust)
{
	if(HalfHeightAdjust == 0.0f || !_Camera) { return; }

	float startEyeHeight = BaseEyeHeight;
	Super::OnEndCrouch(HalfHeightAdjust, ScaledHalfHeightAdjust);

	_CrouchEyeOffset.Z += (startEyeHeight - BaseEyeHeight) - HalfHeightAdjust;
	_Camera->SetRelativeLocation(FVector(0, 0, BaseEyeHeight));
	GetWorld()->GetTimerManager().SetTimer(_CrouchTimer, this, &AP_FPS::LerpCrouchCamera, 0.01f);
}

void AP_FPS::CalcCamera(float DeltaTime, struct FMinimalViewInfo& OutResult)
{
	if(!_Camera) { return; }

	_Camera->GetCameraView(DeltaTime, OutResult);
	OutResult.Location += _CrouchEyeOffset;
}

void AP_FPS::LerpCrouchCamera()
{
	//_CrouchEyeOffset = (1.0f )
}*/


void AP_FPS::Input_Move_Implementation(FVector2D value)
{
	AddMovementInput(FVector::VectorPlaneProject(_Camera->GetForwardVector(), FVector::UpVector).GetSafeNormal(), value.Y);
	AddMovementInput(_Camera->GetRightVector(), value.X);
}

void AP_FPS::Input_SprintPressed_Implementation()
{
	_Stamina2->StartSprinting();

	_MovementComponent->MaxWalkSpeed = _SprintMoveSpeed;
}

void AP_FPS::Input_SprintReleased_Implementation()
{
	_Stamina2->StopSprinting(0.1f);
}

void AP_FPS::Input_CrouchPressed_Implementation()
{
	ACharacter::Crouch();
}

void AP_FPS::Input_CrouchReleased_Implementation()
{
	ACharacter::UnCrouch();
}

void AP_FPS::Input_Interact_Implementation()
{
	IInputable::Input_Interact_Implementation();
}

void AP_FPS::Input_Look_Implementation(FVector2D value)
{
	AddActorWorldRotation(FRotator(0.0f, value.X, 0.0f));
	_Camera->AddLocalRotation(FRotator(value.Y, 0.0f, 0.0f));
}

void AP_FPS::Input_JumpPressed_Implementation()
{
	ACharacter::Jump();
}

void AP_FPS::Input_JumpReleased_Implementation()
{
	ACharacter::StopJumping();
}

void AP_FPS::Input_FirePressed_Implementation()
{
	if(_WeaponRef)
	{
		_WeaponRef->StartFire();
	}
}

void AP_FPS::Input_FireReleased_Implementation()
{
	if(_WeaponRef)
	{
		_WeaponRef->StopFire();
	}
}

UInputMappingContext* AP_FPS::GetMappingContext_Implementation()
{
	return _MappingContext;
}

UBehaviorTree* AP_FPS::GetBehaviorTree_Implementation()
{
	return _BehaviorTree;
}


void AP_FPS::Handle_HealthDead(AController* causer)
{
	
}

void AP_FPS::Handle_HealthDamaged(float currentHealth, float maxHealth, float changedHealth)
{
	OnPawnDamaged.Broadcast(currentHealth, maxHealth, changedHealth);
}

void AP_FPS::Handle_StoppedSprinting()
{
	GetCharacterMovement()->MaxWalkSpeed = _NormalMoveSpeed;
}

void AP_FPS::Handle_ChangeStamina(float currentStamina, float maxStamina, float changedStamina)
{
	OnPawnStaminaChanged.Broadcast(currentStamina, maxStamina, changedStamina);
}

