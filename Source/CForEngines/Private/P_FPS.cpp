#include "P_FPS.h"

#include "HealthComponent.h"
#include "Weapon_Base.h"
#include "Camera/CameraComponent.h"


// Sets default values
AP_FPS::AP_FPS()
{
	_Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	_Camera->SetupAttachment(RootComponent);
	_Health = CreateDefaultSubobject<UHealthComponent>(TEXT("Health"));

	_WeaponAttachPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Weapon Attack Point"));
	_WeaponAttachPoint->SetupAttachment(_Camera);
}

void AP_FPS::BeginPlay()
{
	Super::BeginPlay();

	_Health->OnDead.AddUniqueDynamic(this, &AP_FPS::Handle_HealthDead);
	_Health->OnDamaged.AddUniqueDynamic(this, &AP_FPS::Handle_HealthDamaged);

	if(_DefaultWeapon)
	{
		FActorSpawnParameters spawnParams;
		spawnParams.Owner = this;
		spawnParams.Instigator = this;
		_WeaponRef = GetWorld()->SpawnActor<AWeapon_Base>(_DefaultWeapon, _WeaponAttachPoint->GetComponentTransform(), spawnParams);
		_WeaponRef->AttachToComponent(_WeaponAttachPoint, FAttachmentTransformRules::SnapToTargetIncludingScale);
	}
}



void AP_FPS::Input_Move_Implementation(FVector2D value)
{
	AddMovementInput(FVector::VectorPlaneProject(_Camera->GetForwardVector(), FVector::UpVector).GetSafeNormal(), value.Y);
	AddMovementInput(_Camera->GetRightVector(), value.X);
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


void AP_FPS::Handle_HealthDead(AController* causer)
{
	
}

void AP_FPS::Handle_HealthDamaged(float currentHealth, float maxHealth, float changedHealth)
{
	
}

