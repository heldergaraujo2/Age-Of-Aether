#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Characters/AetherCharacterTypes.h"

#include "AetherCharacter.generated.h"

class UCameraComponent;
class UInputAction;
class UInputMappingContext;
class USpringArmComponent;
class UAetherPlayableCharacterVisualComponent;

UCLASS()
class AGEOFAETHER_API AAetherCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    AAetherCharacter();

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
    virtual void PossessedBy(AController* NewController) override;
    virtual void UnPossessed() override;
    virtual void BeginPlay() override;
    virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

    UFUNCTION(BlueprintPure, Category = "Age of Aether|Character")
    FAetherCharacterId GetCharacterId() const;

    UFUNCTION(BlueprintPure, Category = "Age of Aether|Visual")
    USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

    UFUNCTION(BlueprintPure, Category = "Age of Aether|Visual")
    UAetherPlayableCharacterVisualComponent* GetVisualComponent() const { return VisualComponent; }

    UFUNCTION(BlueprintPure, Category = "Age of Aether|Visual")
    UCameraComponent* GetFollowCamera() const { return FollowCamera; }

    void InitializeCharacterIdentity(const FAetherCharacterId& InCharacterId);

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Age of Aether|Visual")
    TObjectPtr<USpringArmComponent> CameraBoom;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Age of Aether|Visual")
    TObjectPtr<UCameraComponent> FollowCamera;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Age of Aether|Visual")
    TObjectPtr<UAetherPlayableCharacterVisualComponent> VisualComponent;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Age of Aether|Movement")
    float FoundationWalkSpeed = 420.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Age of Aether|Camera")
    float CameraDistance = 450.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Age of Aether|Camera")
    float CameraHeight = 120.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Age of Aether|Camera")
    float CameraTurnRate = 1.0f;

    UPROPERTY(Replicated, BlueprintReadOnly, Category = "Age of Aether|Character")
    FAetherCharacterId CharacterId;

private:
    void InitializeFoundationInput();
    void MoveForward(const struct FInputActionValue& Value);
    void MoveRight(const struct FInputActionValue& Value);
    void LookYaw(const struct FInputActionValue& Value);
    void LookPitch(const struct FInputActionValue& Value);
    void JumpPressed(const struct FInputActionValue& Value);

    UPROPERTY(Transient)
    TObjectPtr<UInputMappingContext> RuntimeInputContext;

    UPROPERTY(Transient)
    TObjectPtr<UInputAction> MoveForwardAction;

    UPROPERTY(Transient)
    TObjectPtr<UInputAction> MoveRightAction;

    UPROPERTY(Transient)
    TObjectPtr<UInputAction> LookYawAction;

    UPROPERTY(Transient)
    TObjectPtr<UInputAction> LookPitchAction;

    UPROPERTY(Transient)
    TObjectPtr<UInputAction> JumpAction;
};
