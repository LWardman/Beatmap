
#include "Characters/BeatmapCharacter.h"

#include "Components/InputComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Kismet/GameplayStatics.h"
#include "Misc/Timespan.h"

#include "Actors/Indicator.h"

ABeatmapCharacter::ABeatmapCharacter()
{
 	PrimaryActorTick.bCanEverTick = true;
}

void ABeatmapCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (const APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	TArray<AActor*> Indicators;
	UGameplayStatics::GetAllActorsOfClass(this, AIndicator::StaticClass(), Indicators);

	if (Indicators.Num() == 2)
	{
		LMB_Indicator = Cast<AIndicator>(Indicators[1]);
		RMB_Indicator = Cast<AIndicator>(Indicators[0]);
	}
}

void ABeatmapCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABeatmapCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) 
	{
		EnhancedInputComponent->BindAction(IA_LMB, ETriggerEvent::Started, this, &ABeatmapCharacter::LMB_Pressed);

		EnhancedInputComponent->BindAction(IA_RMB, ETriggerEvent::Started, this, &ABeatmapCharacter::RMB_Pressed);

		EnhancedInputComponent->BindAction(IA_Space, ETriggerEvent::Started, this, &ABeatmapCharacter::RecordOutput);

		EnhancedInputComponent->BindAction(IA_Begin, ETriggerEvent::Started, this, &ABeatmapCharacter::AcceptInput);
	}
}

void ABeatmapCharacter::LMB_Pressed(const FInputActionValue& Value)
{
	if (!bAcceptingInput) return;
	
	RecordNote("L");

	if (LMB_Indicator)
	{
		LMB_Indicator->Indicate();
	}
}

void ABeatmapCharacter::RMB_Pressed(const FInputActionValue& Value)
{
	if (!bAcceptingInput) return;
	
	RecordNote("R");

	if (RMB_Indicator)
	{
		RMB_Indicator->Indicate();
	}
}

void ABeatmapCharacter::RecordNote(FString Lane)
{
	Output.Add(TTuple<FString, FDateTime>(Lane, FDateTime::Now()));
}

void ABeatmapCharacter::RecordOutput()
{
	const FString FilePath = FPaths::ProjectDir() / TEXT("Output.txt");

	FString TotalString;

	for (int idx = 0; idx < Output.Num(); idx++)
	{
		if (idx == Output.Num() - 1)
		{
			TotalString += FormatNoteTupleToString(Output[idx], Output[idx]);
		}
		else
		{
			TotalString += FormatNoteTupleToString(Output[idx], Output[idx + 1]);
		}
	}

	TotalString = TotalString.LeftChop(2);
	
	WriteOutputToFile(FilePath, TotalString);
}

void ABeatmapCharacter::AcceptInput()
{
	bAcceptingInput = true;
}

FString ABeatmapCharacter::FormatNoteTupleToString(TTuple<FString, FDateTime> Note, TTuple<FString, FDateTime> NextNote)
{
	const FString Lane = Note.Get<0>();

	const FDateTime Date1 = Note.Get<1>();
	const FDateTime Date2 = NextNote.Get<1>();

	const FTimespan TimeDifference = Date2 - Date1;

	const double TimeInSeconds = TimeDifference.GetTotalSeconds();
	
	const FString TimerAsString = FString::SanitizeFloat(TimeInSeconds);
    
	const FString Content = TEXT("MakeDanceNote(\"" + Lane + "\", " + TimerAsString + "),\n");

	return Content;
}

void ABeatmapCharacter::WriteOutputToFile(const FString& FilePath, const FString& Content)
{
	FFileHelper::SaveStringToFile(Content, *FilePath);
}
