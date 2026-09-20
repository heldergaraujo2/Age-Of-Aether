#include "Characters/AetherMovementCameraProfile.h"
bool UAetherMovementCameraProfile::Validate(FString& OutError) const
{
 OutError.Reset();
 if(!FMath::IsFinite(WalkSpeed)||WalkSpeed<=0){OutError=TEXT("WalkSpeed must be finite and positive.");return false;}
 if(!FMath::IsFinite(SprintSpeed)||SprintSpeed<WalkSpeed){OutError=TEXT("SprintSpeed must be finite and >= WalkSpeed.");return false;}
 if(!FMath::IsFinite(JumpVelocity)||JumpVelocity<=0){OutError=TEXT("JumpVelocity must be finite and positive.");return false;}
 if(!FMath::IsFinite(RotationRate)||RotationRate<=0){OutError=TEXT("RotationRate must be finite and positive.");return false;}
 if(!FMath::IsFinite(MinCameraDistance)||!FMath::IsFinite(MaxCameraDistance)||MinCameraDistance<=0||MaxCameraDistance<MinCameraDistance){OutError=TEXT("Camera distance range is invalid.");return false;}
 if(!FMath::IsFinite(CameraZoomStep)||CameraZoomStep<=0){OutError=TEXT("CameraZoomStep must be finite and positive.");return false;}
 if(!FMath::IsFinite(CameraMinPitch)||!FMath::IsFinite(CameraMaxPitch)||CameraMaxPitch<=CameraMinPitch){OutError=TEXT("Camera pitch range is invalid.");return false;}
 return true;
}