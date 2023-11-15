
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BeatmapCharacter.generated.h"

class UInputMappingContext;
class UInputAction;

class AIndicator;

struct FInputActionValue;
struct FTimerHandle;

UCLASS()
class BEATMAP_API ABeatmapCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ABeatmapCharacter();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* IA_LMB;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* IA_RMB;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* IA_Space;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* IA_Begin;

	void LMB_Pressed(const FInputActionValue& Value);

	void RMB_Pressed(const FInputActionValue& Value);

	void RecordNote(FString Lane);

	TArray<TTuple<FString, FDateTime>> Output;

	FDateTime TimeAtLastNote;

	bool bAcceptingInput = false;

	UPROPERTY(EditAnywhere)
	AIndicator* LMB_Indicator;

	UPROPERTY(EditAnywhere)
	AIndicator* RMB_Indicator;

	void RecordOutput();

	void AcceptInput();

	FString FormatNoteTupleToString(TTuple<FString, FDateTime> Note, TTuple<FString, FDateTime> NextNote);

	void WriteOutputToFile(const FString& FilePath, const FString& Content);

};
