#include "Character/PC_FPS.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Blueprint/UserWidget.h"
#include "Character/P_FPS.h"
#include "Character/Components/Inputable.h"
#include "Character/Components/Widget_HUD.h"
#include "Kismet/KismetSystemLibrary.h"


class AP_FPS;

void APC_FPS::BeginPlay()
{
	Super::BeginPlay();
	
	if(_HUDWidgetClass)
	{
		_HUDWidget = CreateWidget<UWidget_HUD, APC_FPS*>(this, _HUDWidgetClass);
		_HUDWidget->AddToViewport();
	}

	if(AP_FPS* currentPawn = Cast<AP_FPS>(GetPawn()))
	{
		currentPawn->OnPawnDamaged.AddUniqueDynamic(this, &APC_FPS::Damaged);
		currentPawn->OnPawnStaminaChanged.AddUniqueDynamic(this, &APC_FPS::StaminaChanged);

		currentPawn->OnShowInteractPrompt.AddUniqueDynamic(this, &APC_FPS::ShowInteractPrompt);
		currentPawn->OnHideInteractPrompt.AddUniqueDynamic(this, &APC_FPS::HideInteractPrompt);
	}
}


void APC_FPS::SetupInputComponent()
{
	Super::SetupInputComponent();

	if(UEnhancedInputComponent* EIP = CastChecked<UEnhancedInputComponent>(InputComponent))
	{
		EIP->BindAction(_MoveAction, ETriggerEvent::Triggered, this, &APC_FPS::Move);
		EIP->BindAction(_MoveAction, ETriggerEvent::Completed, this, &APC_FPS::Move);
		
		EIP->BindAction(_SprintAction, ETriggerEvent::Started, this, &APC_FPS::SprintPressed);
		EIP->BindAction(_SprintAction, ETriggerEvent::Completed, this, &APC_FPS::SprintReleased);
		
		EIP->BindAction(_MoveAction, ETriggerEvent::Triggered, this, &APC_FPS::Move);
		
		EIP->BindAction(_LookAction, ETriggerEvent::Triggered, this, &APC_FPS::Look);
		
		EIP->BindAction(_InteractAction, ETriggerEvent::Started, this, &APC_FPS::Interact);
		
		EIP->BindAction(_JumpAction, ETriggerEvent::Triggered, this, &APC_FPS::JumpPressed);
		EIP->BindAction(_JumpAction, ETriggerEvent::Completed, this, &APC_FPS::JumpReleased);
		
		EIP->BindAction(_FireAction, ETriggerEvent::Triggered, this, &APC_FPS::FirePressed);
		EIP->BindAction(_FireAction, ETriggerEvent::Completed, this, &APC_FPS::FireReleased);

		EIP->BindAction(_CrouchAction, ETriggerEvent::Started, this, &APC_FPS::CrouchPressed);
		EIP->BindAction(_CrouchAction, ETriggerEvent::Completed, this, &APC_FPS::CrouchReleased);
	}
}

void APC_FPS::Move(const FInputActionValue& value)
{
	FVector2D MoveVector = value.Get<FVector2D>();

	if(value.GetMagnitude() > 0.0f) { _IsMoving = true; }
	else
	{
		_IsMoving = false;
		if(_IsSprinting) { SprintReleased(); }
	}

	if(APawn* currentPawn = GetPawn())
	{
		if(UKismetSystemLibrary::DoesImplementInterface(currentPawn, UInputable::StaticClass()))
		{
			IInputable::Execute_Input_Move(currentPawn, MoveVector);
		}
	}
}

void APC_FPS::SprintPressed()
{
	if(_IsCrouching || !_IsMoving) { return; }
	if(!_IsSprinting) { _IsSprinting = true; }
	
	if(APawn* currentPawn = GetPawn())
	{
		if(UKismetSystemLibrary::DoesImplementInterface(currentPawn, UInputable::StaticClass()))
		{
			IInputable::Execute_Input_SprintPressed(currentPawn);
		}
	}
}

void APC_FPS::SprintReleased()
{
	if(_IsSprinting) { _IsSprinting = false; }
	
	if(APawn* currentPawn = GetPawn())
	{
		if(UKismetSystemLibrary::DoesImplementInterface(currentPawn, UInputable::StaticClass()))
		{
			IInputable::Execute_Input_SprintReleased(currentPawn);
		}
	}
}

void APC_FPS::Look(const FInputActionValue& value)
{
	FVector2D LookVector = value.Get<FVector2D>();

	if(APawn* currentPawn = GetPawn())
	{
		if(UKismetSystemLibrary::DoesImplementInterface(currentPawn, UInputable::StaticClass()))
		{
			IInputable::Execute_Input_Look(currentPawn, LookVector);
		}
	}
}

void APC_FPS::JumpPressed()
{
	if(APawn* currentPawn = GetPawn())
	{
		if(UKismetSystemLibrary::DoesImplementInterface(currentPawn, UInputable::StaticClass()))
		{
			IInputable::Execute_Input_JumpPressed(currentPawn);
		}
	}
}

void APC_FPS::JumpReleased()
{
	if(APawn* currentPawn = GetPawn())
	{
		if(UKismetSystemLibrary::DoesImplementInterface(currentPawn, UInputable::StaticClass()))
		{
			IInputable::Execute_Input_JumpReleased(currentPawn);
		}
	}
}

void APC_FPS::FirePressed()
{
	if(APawn* currentPawn = GetPawn())
	{
		if(UKismetSystemLibrary::DoesImplementInterface(currentPawn, UInputable::StaticClass()))
		{
			IInputable::Execute_Input_FirePressed(currentPawn);
		}
	}
}

void APC_FPS::FireReleased()
{
	if(APawn* currentPawn = GetPawn())
	{
		if(UKismetSystemLibrary::DoesImplementInterface(currentPawn, UInputable::StaticClass()))
		{
			IInputable::Execute_Input_FireReleased(currentPawn);
		}
	}
}

void APC_FPS::CrouchPressed()
{
	if(APawn* currentPawn = GetPawn())
	{
		if(UKismetSystemLibrary::DoesImplementInterface(currentPawn, UInputable::StaticClass()))
		{
			_IsCrouching = true;
			IInputable::Execute_Input_CrouchPressed(currentPawn);
		}
	}
}

void APC_FPS::CrouchReleased()
{
	if(APawn* currentPawn = GetPawn())
	{
		if(UKismetSystemLibrary::DoesImplementInterface(currentPawn, UInputable::StaticClass()))
		{
			_IsCrouching = false;
			IInputable::Execute_Input_CrouchReleased(currentPawn);
		}
	}
}

void APC_FPS::Interact()
{
	if(APawn* currentPawn = GetPawn())
	{
		if(UKismetSystemLibrary::DoesImplementInterface(currentPawn, UInputable::StaticClass()))
		{
			_IsCrouching = false;
			IInputable::Execute_Input_Interact(currentPawn);
		}
	}
}

void APC_FPS::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if(UEnhancedInputLocalPlayerSubsystem* subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		if(UKismetSystemLibrary::DoesImplementInterface(InPawn, UInputable::StaticClass()))
		{
			subsystem->AddMappingContext(IInputable::Execute_GetMappingContext(InPawn), 0);
		}
	}
}

void APC_FPS::AddPoints_Implementation(int points)
{
	_Score += points;
	_HUDWidget->UpdateScore(_Score);
}

void APC_FPS::Damaged(float currentHealth, float maxHealth, float changedHealth)
{
	_HUDWidget->UpdateHealth(currentHealth / maxHealth);
}

void APC_FPS::StaminaChanged(float currentStamina, float maxStamina, float changedStamina)
{
	_HUDWidget->StartStaminaChange(currentStamina / maxStamina);
}

void APC_FPS::ShowInteractPrompt()
{
	_HUDWidget->ShowInteractPrompt();
}

void APC_FPS::HideInteractPrompt()
{
	_HUDWidget->HideInteractPrompt();
}
