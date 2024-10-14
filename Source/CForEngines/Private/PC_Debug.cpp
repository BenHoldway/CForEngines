#include "PC_Debug.h"
#include "EnhancedInputComponent.h"

void APC_Debug::SetupInputComponent()
{
	Super::SetupInputComponent();

	if(UEnhancedInputComponent* EIP = CastChecked<UEnhancedInputComponent>(InputComponent))
	{
		EIP -> BindAction(_DebugAction, ETriggerEvent::Triggered, this, &APC_Debug::Debug);
	}
}

void APC_Debug::Debug(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Display, TEXT("Debugging is working!"));
}
