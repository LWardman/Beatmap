
#include "Actors/Indicator.h"

AIndicator::AIndicator()
{
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));

}

void AIndicator::BeginPlay()
{
	Super::BeginPlay();

	if (Mesh && MatPlain)
	{
		Mesh->SetMaterial(0, MatPlain);
	}
}

void AIndicator::Indicate()
{
	if (Mesh && MatIndicate)
	{
		Mesh->SetMaterial(0, MatIndicate);

		GetWorld()->GetTimerManager().SetTimer(
			IndicatorTimerHandle,
			this,
			&AIndicator::EndIndicate,
			IndicationLength,
			false
			);
	}
}

void AIndicator::EndIndicate()
{
	if (Mesh && MatPlain)
	{
		Mesh->SetMaterial(0, MatPlain);
	}
}
