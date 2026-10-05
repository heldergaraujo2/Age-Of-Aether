#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Characters/AetherCharacterTypes.h"

#include "AetherCharacter.generated.h"

class UCameraComponent;
class UInputAction;
class UInputMappingContext;
class USpringArmComponent;
class UStaticMeshComponent;
class UPaperSpriteComponent;
class UPaperSprite;
class UTexture2D;
class UAetherPlayableCharacterVisualComponent;
class UAetherMovementCameraProfile;
class UAetherEquipmentVisualComponent;
class UAetherClassEvolutionPresentationComponent;
class UAetherSkillVisualComponent;
class UAether2DCharacterVisualComponent;
class UAether2DIsometricCameraComponent;

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

    UFUNCTION(BlueprintPure, Category = "Age of Aether|2D Visual")
    UAether2DCharacterVisualComponent* Get2DVisualComponent() const { return Visual2DComponent; }

    UFUNCTION(BlueprintPure, Category = "Age of Aether|Equipment")
    UAetherEquipmentVisualComponent* GetEquipmentVisualComponent() const { return EquipmentVisualComponent; }

    UFUNCTION(BlueprintPure, Category = "Age of Aether|Class Presentation")
    UAetherClassEvolutionPresentationComponent* GetClassEvolutionPresentationComponent() const { return ClassEvolutionPresentationComponent; }

    UFUNCTION(BlueprintPure, Category = "Age of Aether|Visual")
    UCameraComponent* GetFollowCamera() const { return FollowCamera; }

    UFUNCTION(BlueprintPure, Category = "Age of Aether|2D Camera")
    UAether2DIsometricCameraComponent* Get2DIsometricCameraComponent() const { return IsometricCameraComponent; }

    UFUNCTION(BlueprintPure, Category = "Age of Aether|Skills")
    UAetherSkillVisualComponent* GetSkillVisualComponent() const { return SkillVisualComponent; }

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

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Age of Aether|2D Visual")
    TObjectPtr<UAether2DCharacterVisualComponent> Visual2DComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Age of Aether|2D Camera")
    TObjectPtr<UAether2DIsometricCameraComponent> IsometricCameraComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Age of Aether|Equipment")
    TObjectPtr<UAetherEquipmentVisualComponent> EquipmentVisualComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Age of Aether|Class Presentation")
    TObjectPtr<UAetherClassEvolutionPresentationComponent> ClassEvolutionPresentationComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Age of Aether|Skills")
    TObjectPtr<UAetherSkillVisualComponent> SkillVisualComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Age of Aether|Runtime Visual")
    TObjectPtr<UStaticMeshComponent> RuntimeBodyVisual;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Age of Aether|Runtime Visual")
    TObjectPtr<UStaticMeshComponent> RuntimeHeadVisual;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Age of Aether|Runtime Visual")
    TObjectPtr<UStaticMeshComponent> RuntimeMantleVisual;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Age of Aether|Runtime Visual")
    TObjectPtr<UStaticMeshComponent> RuntimeLeftArmVisual;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Age of Aether|Runtime Visual")
    TObjectPtr<UStaticMeshComponent> RuntimeRightArmVisual;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Age of Aether|Runtime Visual")
    TObjectPtr<UStaticMeshComponent> RuntimeLeftHandVisual;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Age of Aether|Runtime Visual")
    TObjectPtr<UStaticMeshComponent> RuntimeRightHandVisual;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Age of Aether|Runtime Visual")
    TObjectPtr<UStaticMeshComponent> RuntimeLeftLegVisual;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Age of Aether|Runtime Visual")
    TObjectPtr<UStaticMeshComponent> RuntimeRightLegVisual;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Age of Aether|Runtime Visual")
    TObjectPtr<UStaticMeshComponent> RuntimeLeftBootVisual;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Age of Aether|Runtime Visual")
    TObjectPtr<UStaticMeshComponent> RuntimeRightBootVisual;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Age of Aether|Runtime Visual")
    TObjectPtr<UStaticMeshComponent> RuntimeHairVisual;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Age of Aether|Runtime Visual")
    TObjectPtr<UStaticMeshComponent> RuntimeHatVisual;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Age of Aether|Runtime Visual")
    TObjectPtr<UStaticMeshComponent> RuntimeBeltVisual;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Age of Aether|Runtime Visual")
    TObjectPtr<UStaticMeshComponent> RuntimeStaffVisual;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Age of Aether|Runtime Visual")
    TObjectPtr<UStaticMeshComponent> RuntimeOrbVisual;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Age of Aether|Runtime 2D Art")
    TObjectPtr<UPaperSpriteComponent> Runtime2DArtVisual;

    UPROPERTY(Transient)
    TObjectPtr<UPaperSprite> Runtime2DArtSprite;

    UPROPERTY(Transient)
    TObjectPtr<UTexture2D> Runtime2DArtTexture;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Age of Aether|Movement")
    float FoundationWalkSpeed = 420.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Age of Aether|Movement")
    TObjectPtr<UAetherMovementCameraProfile> MovementCameraProfile;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Age of Aether|Camera")
    float CameraDistance = 2100.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Age of Aether|Camera")
    float CameraHeight = 120.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Age of Aether|Camera")
    float CameraTurnRate = 1.0f;

    UPROPERTY(Replicated, BlueprintReadOnly, Category = "Age of Aether|Character")
    FAetherCharacterId CharacterId;

private:
    void InitializeFoundationInput();
    void InitializeRuntimeVisual();
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
    UPROPERTY(Transient) TObjectPtr<UInputAction> BasicAttackAction;
    bool bSprinting = false;
    uint32 LocalAttackSequence = 0;
};