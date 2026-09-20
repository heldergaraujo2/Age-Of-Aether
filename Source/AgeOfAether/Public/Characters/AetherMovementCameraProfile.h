#pragma once
#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "AetherMovementCameraProfile.generated.h"
UCLASS(BlueprintType)
class AGEOFAETHER_API UAetherMovementCameraProfile : public UDataAsset
{
 GENERATED_BODY()
public:
 UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Movement",meta=(ClampMin="0.0")) float WalkSpeed=420.0f;
 UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Movement",meta=(ClampMin="0.0")) float SprintSpeed=650.0f;
 UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Movement",meta=(ClampMin="0.0")) float JumpVelocity=560.0f;
 UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Movement",meta=(ClampMin="0.0")) float RotationRate=720.0f;
 UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Camera",meta=(ClampMin="100.0")) float MinCameraDistance=250.0f;
 UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Camera",meta=(ClampMin="100.0")) float MaxCameraDistance=650.0f;
 UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Camera") float CameraZoomStep=50.0f;
 UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Camera") float CameraMinPitch=-75.0f;
 UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Camera") float CameraMaxPitch=35.0f;
 UFUNCTION(BlueprintCallable,Category="Movement") bool Validate(FString& OutError) const;
};