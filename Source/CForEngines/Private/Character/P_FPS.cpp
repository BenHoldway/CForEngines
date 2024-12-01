#include "Character/P_FPS.h"

#include "Camera/CameraComponent.h"
#include "Character/Components/HealthComponent.h"
#include "Character/Components/Interactable.h"
#include "Character/Components/StaminaComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/AudioComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Weapons/Weapon_Base.h"
#include "Components/SpotLightComponent.h"
#include "Kismet/GameplayStatics.h"


AP_FPS::AP_FPS()
{
	_AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("Sound"));
	_AudioComponent->SetupAttachment(RootComponent);
	
	_Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	_Camera->SetupAttachment(RootComponent);	

	_Arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Forwards"));
	_Arrow->SetupAttachment(RootComponent);
	
	_InteractionCollider2 = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Interaction Collider"));
	_InteractionCollider2->SetupAttachment(_Camera);
	_InteractionCollider2->OnComponentBeginOverlap.AddUniqueDynamic(this, &AP_FPS::Handle_InteractionBeginOverlap);
	_InteractionCollider2->OnComponentEndOverlap.AddUniqueDynamic(this, &AP_FPS::Handle_InteractionEndOverlap);

	_Flashlight = CreateDefaultSubobject<USpotLightComponent>(TEXT("Flashlight"));
	_Flashlight->SetupAttachment(_Camera);
	
	_Health = CreateDefaultSubobject<UHealthComponent>(TEXT("Health"));
	_Stamina2 = CreateDefaultSubobject<UStaminaComponent>(TEXT("Stamina"));

	_WeaponAttachPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Weapon Attack Point"));
	_WeaponAttachPoint->SetupAttachment(_Camera);

	_InteractionRange = 50.0f;
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
	
	
	_MovementComponent = GetCharacterMovement();
	_MovementComponent->MaxWalkSpeed = _NormalMoveSpeed;

	_FlashlightNormalIntensity = _Flashlight->Intensity;
	
	_FlashlightAudioComponent = UGameplayStatics::CreateSound2D(this, _FlashlightToggleSound);
	_DeadAudioComponent = UGameplayStatics::CreateSound2D(this, _DeadSound);
}

void AP_FPS::StartPlayingFootstepSounds()
{
	PlayFootstepSound();
	GetWorld()->GetTimerManager().SetTimer(PlayFootstepSoundTimer, this, &AP_FPS::PlayFootstepSound, _FootstepTimeStep, true);
}

void AP_FPS::PlayFootstepSound()
{
	_FootstepAudioComponent = UGameplayStatics::CreateSound2D(this, _FootstepSounds);
	_FootstepAudioComponent->Play();
}

void AP_FPS::StopPlayingFootstepSounds()
{
	if(_FootstepAudioComponent) { _FootstepAudioComponent->DestroyComponent(); }
	GetWorld()->GetTimerManager().ClearTimer(PlayFootstepSoundTimer);
}

void AP_FPS::Input_StartedMove_Implementation(FVector2D value)
{
	StartPlayingFootstepSounds();
}

void AP_FPS::Input_Move_Implementation(FVector2D value)
{
	AddMovementInput(FVector::VectorPlaneProject(_Camera->GetForwardVector(), FVector::UpVector).GetSafeNormal(), value.Y);
	AddMovementInput(_Camera->GetRightVector(), value.X);
}

void AP_FPS::Input_MoveCancelled_Implementation(FVector2D value)
{
	StopPlayingFootstepSounds();
}

void AP_FPS::Input_SprintPressed_Implementation()
{
	_Stamina2->StartSprinting();

	_MovementComponent->MaxWalkSpeed = _SprintMoveSpeed;
	GetWorld()->GetTimerManager().ClearTimer(PlayFootstepSoundTimer);
	GetWorld()->GetTimerManager().SetTimer(PlayFootstepSoundTimer, this, &AP_FPS::PlayFootstepSound, _FootstepTimeStep / 2, true);
}

void AP_FPS::Input_SprintReleased_Implementation()
{
	_Stamina2->StopSprinting(0.1f);
	GetWorld()->GetTimerManager().ClearTimer(PlayFootstepSoundTimer);
	GetWorld()->GetTimerManager().SetTimer(PlayFootstepSoundTimer, this, &AP_FPS::PlayFootstepSound, _FootstepTimeStep, true);
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

void AP_FPS::Input_FlashlightToggle_Implementation(bool isFlashlightOn)
{
	_Flashlight->SetIntensity(isFlashlightOn ? _FlashlightNormalIntensity : 0.0f);
	if(_FlashlightAudioComponent)
	{
		_FlashlightAudioComponent->Play();
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

}

void AP_FPS::Input_FireReleased_Implementation()
{

}

UInputMappingContext* AP_FPS::GetMappingContext_Implementation()
{
	return _MappingContext;
}

void AP_FPS::DisableInput(APlayerController* PlayerController)
{
	Super::DisableInput(PlayerController);
}

UBehaviorTree* AP_FPS::GetBehaviorTree_Implementation()
{
	return _BehaviorTree;
}

void AP_FPS::OverrideSkeletonMesh(USkeletalMesh* mesh)
{
	GetMesh()->SetSkeletalMesh(mesh); 
}

void AP_FPS::OverrideFlashlightColour(FLinearColor colour)
{
	_Flashlight->SetLightColor(colour);
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
	if(_DeadAudioComponent) { _DeadAudioComponent->Play(); }
	OnPawnDead.Broadcast(causer);
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
			false, ActorsToIgnore, EDrawDebugTrace::ForDuration, hit, true, FLinearColor::Transparent, FLinearColor::Transparent, 0.0f);
		
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
