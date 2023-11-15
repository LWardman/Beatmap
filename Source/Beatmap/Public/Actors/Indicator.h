
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Indicator.generated.h"

UCLASS()
class BEATMAP_API AIndicator : public AActor
{
	GENERATED_BODY()
	
public:	
	AIndicator();

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere)
	UMaterial* MatPlain;

	UPROPERTY(EditAnywhere)
	UMaterial* MatIndicate;

	void Indicate();

	void EndIndicate();

	FTimerHandle IndicatorTimerHandle;

	UPROPERTY(EditAnywhere)
	float IndicationLength = 0.05;

};
