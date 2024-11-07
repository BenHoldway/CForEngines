#include "Character/P_FPS.h"

#include "Camera/CameraComponent.h"
#include "Character/Components/HealthComponent.h"
#include "Character/Components/Interactable.h"
#include "Character/Components/StaminaComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Systems/System_Controller.h"
#include "Weapons/Weapon_Base.h"


AP_FPS::AP_FPS()
{
	_Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	_Camera->SetupAttachment(RootComponent);
	
	_InteractionCollider2 = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Interaction Collider"));
	_InteractionCollider2->SetupAttachment(_Camera);
	_InteractionCollider2->OnComponentBeginOverlap.AddUniqueDynamic(this, &AP_FPS::Handle_InteractionBeginOverlap);
	_InteractionCollider2->OnComponentEndOverlap.AddUniqueDynamic(this, &AP_FPS::Handle_InteractionEndOverlap);
	
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

	_InteractionCollider2->SetCapsuleHalfHeight(_InteractionRange);
	_InteractionCollider2->SetRelativeLocation(FVector(_InteractionRange, 0, 0));
	//_InteractionCollider2->SetRelativeRotation(FRotator(0, 90, 0));
	
	
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
	if(_Interactable == nullptr) { return; }

	if(UKismetSystemLibrary::DoesImplementInterface(_Interactable, UInteractable::StaticClass()))
	{
		IInteractable::Execute_Interact(_Interactable);
	}
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

AActor* AP_FPS::GetClosest(TArray<FHitResult> hit)
{
	float distance = 100000000;
	FHitResult closest(ForceInit);
	
	for(int hitActor = 0; hitActor < hit.Num(); hitActor++)
	{
		if(GetDistanceTo(hit[hitActor].GetActor()) < distance) { closest = hit[hitActor]; }
	}

	return closest.GetActor();
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

void AP_FPS::Handle_InteractionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr))
	{
		UWorld* const world = GetWorld();
		if(world == nullptr) { return; }

		//Raycast when shooting
		FHitResult hit(ForceInit);
		FVector start = _Camera->GetComponentLocation();
		FVector end = start + (_Camera->GetForwardVector() * _InteractionRange * 2);
		TArray<AActor*> ActorsToIgnore;
		ActorsToIgnore.Add(this);

		UKismetSystemLibrary::SphereTraceSingle(world, start, end, _InteractionCollider2->GetScaledCapsuleRadius(), UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel2),
			false, ActorsToIgnore, EDrawDebugTrace::ForDuration, hit, true, FLinearColor::Red, FLinearColor::Green, 5.0f);
		
		if(hit.GetActor() == OtherActor)
		{
			_Interactable = OtherActor;

			UE_LOG(LogTemp, Display, TEXT("Can Interact with %s"), *hit.GetActor()->GetName());
			OnShowInteractPrompt.Broadcast();
		}
	}
}

void AP_FPS::Handle_InteractionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr) && (_Interactable == OtherActor))
	{
		if(_Interactable != nullptr)
		{
			_Interactable = nullptr;
			OnHideInteractPrompt.Broadcast();
		}
	}
}
