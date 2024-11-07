#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SystemDisplayer.generated.h"

class USystemWidget;
class UWidgetComponent;
class UBoxComponent;
class UStaticMeshComponent;

UCLASS(Abstract)
class CFORENGINES_API ASystemDisplayer : public AActor
{
	GENERATED_BODY()

public:
	ASystemDisplayer();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<UBoxComponent> _Collider;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<UStaticMeshComponent> _Mesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr<UWidgetComponent> _SystemWidgetComponent;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<USystemWidget> _SystemWidgetClass;
	TObjectPtr<USystemWidget> _SystemWidget;

public:
};
