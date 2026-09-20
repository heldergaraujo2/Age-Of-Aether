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
class UAetherMovementCameraProfile;
class UAetherEquipmentVisualComponent;
class UAetherClassEvolutionPresentationComponent;

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

    UFUNCTION(BlueprintPure, Category = "Age of Aether|Equipment")
    UAetherEquipmentVisualComponent* GetEquipmentVisualComponent() const { return EquipmentVisualComponent; }

    UFUNCTION(BlueprintPure, Category = "Age of Aether|Class Presentation")
    UAetherClassEvolutionPresentationComponent* GetClassEvolutionPresentationComponent() const { return ClassEvolutionPresentationComponent; }

    UFUNCTION(BlueprintPure, Category = "Age of Aether|Visual")
    UCameraComponent* GetFollowCamera() const { return FollowCamera; }

    UFUNCTION(BlueprintPure, Category = "Age of Aether|Movement")
    UAetherMovementCameraProfile* GetMovementCameraProfile() const { return MovementCameraProfile; }

    void InitializeCharacterIdentity(const FAetherCharacterId& InCharacterId);

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Age of Aether|Visual")
    TObjectPtr<USpringArmComponent> CameraBoom;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Age of Aether|Visual")
    TObjectPtr<UCameraComponent> FollowCamera;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Age of Aether|Visual")
    TObjectPtr<UAetherPlayableCharacterVisualComponent> VisualComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Age of Aether|Equipment")
    TObjectPtr<UAetherEquipmentVisualComponent> EquipmentVisualComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Age of Aether|Class Presentation")
    TObjectPtr<UAetherClassEvolutionPresentationComponent> ClassEvolutionPresentationComponent;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Age of Aether|Movement")
    float FoundationWalkSpeed = 420.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Age of Aether|Movement")
    TObjectPtr<UAetherMovementCameraProfile> MovementCameraProfile;

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
    void SprintStarted(const struct FInputActionValue& Value);
    void SprintStopped(const struct FInputActionValue& Value);
    void CameraZoom(const struct FInputActionValue& Value);
    void BasicAttackPressed(const struct FInputActionValue& Value);
    void ExecuteBasicAttack();
    UFUNCTION(Server, Reliable)
    void ServerSetSprinting(bool bNewSprinting);

    UFUNCTION(Server, Reliable)
    void ServerRequestBasicAttack(const FAetherCharacterId& TargetCharacterId);

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
    UPROPERTY(Transient) TObjectPtr<UInputAction> SprintAction;
    UPROPERTY(Transient) TObjectPtr<UInputAction> CameraZoomAction;
    bool bSprinting = false;
    uint32 LocalAttackSequence = 0;
};
