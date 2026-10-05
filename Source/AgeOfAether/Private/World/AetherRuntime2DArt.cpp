#include "World/AetherRuntime2DArt.h"

#include "Engine/Texture2D.h"
#include "PaperSprite.h"
#include "SpriteEditorOnlyTypes.h"

namespace
{
    struct FArtCanvas
    {
        int32 Width = 0;
        int32 Height = 0;
        TArray<FColor> Pixels;

        FArtCanvas(int32 InWidth, int32 InHeight)
            : Width(InWidth)
            , Height(InHeight)
            , Pixels()
        {
            Pixels.Init(FColor(0, 0, 0, 0), Width * Height);
        }

        void Set(int32 X, int32 Y, const FColor& Color)
        {
            if (X >= 0 && X < Width && Y >= 0 && Y < Height)
            {
                Pixels[Y * Width + X] = Color;
            }
        }

        void Rect(int32 MinX, int32 MinY, int32 MaxX, int32 MaxY, const FColor& Color)
        {
            for (int32 Y = MinY; Y <= MaxY; ++Y)
            {
                for (int32 X = MinX; X <= MaxX; ++X)
                {
                    Set(X, Y, Color);
                }
            }
        }

        void Ellipse(int32 CX, int32 CY, int32 RX, int32 RY, const FColor& Color)
        {
            if (RX <= 0 || RY <= 0) return;
            const float InvX = 1.0f / FMath::Square(static_cast<float>(RX));
            const float InvY = 1.0f / FMath::Square(static_cast<float>(RY));
            for (int32 Y = CY - RY; Y <= CY + RY; ++Y)
            {
                for (int32 X = CX - RX; X <= CX + RX; ++X)
                {
                    const float DX = static_cast<float>(X - CX);
                    const float DY = static_cast<float>(Y - CY);
                    if (DX * DX * InvX + DY * DY * InvY <= 1.0f)
                    {
                        Set(X, Y, Color);
                    }
                }
            }
        }

        void Line(int32 X0, int32 Y0, int32 X1, int32 Y1, int32 Radius, const FColor& Color)
        {
            const int32 DX = FMath::Abs(X1 - X0);
            const int32 DY = FMath::Abs(Y1 - Y0);
            const int32 SX = X0 < X1 ? 1 : -1;
            const int32 SY = Y0 < Y1 ? 1 : -1;
            int32 Err = DX - DY;
            for (;;)
            {
                Ellipse(X0, Y0, Radius, Radius, Color);
                if (X0 == X1 && Y0 == Y1) break;
                const int32 E2 = 2 * Err;
                if (E2 > -DY) { Err -= DY; X0 += SX; }
                if (E2 < DX) { Err += DX; Y0 += SY; }
            }
        }

        void Triangle(FIntPoint A, FIntPoint B, FIntPoint C, const FColor& Color)
        {
            const int32 MinX = FMath::Min3(A.X, B.X, C.X);
            const int32 MaxX = FMath::Max3(A.X, B.X, C.X);
            const int32 MinY = FMath::Min3(A.Y, B.Y, C.Y);
            const int32 MaxY = FMath::Max3(A.Y, B.Y, C.Y);
            const auto Edge = [](const FIntPoint& P, const FIntPoint& Q, const FIntPoint& R)
            {
                return (R.X - P.X) * (Q.Y - P.Y) - (R.Y - P.Y) * (Q.X - P.X);
            };
            const int32 Area = Edge(A, B, C);
            if (Area == 0) return;
            for (int32 Y = MinY; Y <= MaxY; ++Y)
            {
                for (int32 X = MinX; X <= MaxX; ++X)
                {
                    const FIntPoint P(X, Y);
                    const int32 W0 = Edge(B, C, P);
                    const int32 W1 = Edge(C, A, P);
                    const int32 W2 = Edge(A, B, P);
                    if ((W0 >= 0 && W1 >= 0 && W2 >= 0) || (W0 <= 0 && W1 <= 0 && W2 <= 0))
                    {
                        Set(X, Y, Color);
                    }
                }
            }
        }
    };

    UPaperSprite* BuildSprite(UObject* Outer, const TCHAR* Name, const FArtCanvas& Canvas, float PixelsPerUnit = 2.0f)
    {
        TArray64<uint8> Raw;
        Raw.SetNumUninitialized(Canvas.Pixels.Num() * sizeof(FColor));
        FMemory::Memcpy(Raw.GetData(), Canvas.Pixels.GetData(), Raw.Num());

        UTexture2D* Texture = UTexture2D::CreateTransient(
            Canvas.Width,
            Canvas.Height,
            PF_B8G8R8A8,
            NAME_None,
            Raw);

        if (!Texture)
        {
            return nullptr;
        }

        Texture->SRGB = true;
        Texture->NeverStream = true;
        Texture->CompressionSettings = TC_Default;
        Texture->MipGenSettings = TMGS_NoMipmaps;
        Texture->Filter = TF_Nearest;
        Texture->UpdateResource();

        UPaperSprite* Sprite = NewObject<UPaperSprite>(Outer, MakeUniqueObjectName(Outer, UPaperSprite::StaticClass(), FName(Name)));
        if (!Sprite)
        {
            return nullptr;
        }

        FSpriteAssetInitParameters Params;
        Params.SetTextureAndFill(Texture);
        Params.bOverridePixelsPerUnrealUnit = true;
        Params.PixelsPerUnrealUnit = PixelsPerUnit;
        Sprite->InitializeSprite(Params, true);
        return Sprite;
    }

    void AddLeafCluster(FArtCanvas& C, int32 X, int32 Y, int32 RX, int32 RY, const FColor& Dark, const FColor& Mid, const FColor& Light)
    {
        C.Ellipse(X + 5, Y + 8, RX + 5, RY + 3, Dark);
        C.Ellipse(X, Y, RX, RY, Mid);
        C.Ellipse(X - RX / 3, Y - RY / 3, FMath::Max(3, RX / 2), FMath::Max(3, RY / 2), Light);
    }
}

UPaperSprite* FAetherRuntime2DArt::CreateCharacterSprite(UObject* Outer)
{
    FArtCanvas C(192, 288);
    C.Ellipse(96, 268, 54, 13, FColor(18, 16, 20, 145));

    const FColor Boot(30, 24, 24, 255);
    const FColor Trouser(35, 48, 72, 255);
    const FColor CoatDark(48, 35, 68, 255);
    const FColor Coat(78, 48, 96, 255);
    const FColor Skin(214, 151, 112, 255);
    const FColor Hair(42, 28, 25, 255);
    const FColor Metal(190, 198, 214, 255);
    const FColor Gold(222, 169, 58, 255);

    C.Line(82, 218, 72, 258, 9, Trouser);
    C.Line(108, 218, 120, 258, 9, Trouser);
    C.Ellipse(70, 264, 15, 8, Boot);
    C.Ellipse(123, 264, 15, 8, Boot);

    C.Ellipse(96, 174, 48, 57, CoatDark);
    C.Triangle(FIntPoint(57, 195), FIntPoint(135, 195), FIntPoint(96, 245), Coat);
    C.Line(64, 171, 42, 208, 8, Skin);
    C.Line(128, 171, 150, 205, 8, Skin);
    C.Ellipse(41, 211, 9, 9, Skin);
    C.Ellipse(151, 208, 9, 9, Skin);

    C.Ellipse(96, 93, 42, 45, Skin);
    C.Ellipse(96, 69, 43, 27, Hair);
    C.Ellipse(73, 82, 17, 31, Hair);
    C.Ellipse(119, 82, 17, 31, Hair);
    C.Ellipse(83, 96, 5, 4, FColor::Black);
    C.Ellipse(109, 96, 5, 4, FColor::Black);

    C.Triangle(FIntPoint(48, 139), FIntPoint(144, 139), FIntPoint(96, 25), CoatDark);
    C.Ellipse(96, 36, 18, 8, Gold);

    C.Line(149, 200, 178, 133, 4, Metal);
    C.Line(144, 208, 173, 140, 2, Gold);

    return BuildSprite(Outer, TEXT("AOA_Runtime_Player_Mage"), C, 2.0f);
}

UPaperSprite* FAetherRuntime2DArt::CreateTreeSprite(UObject* Outer, int32 Variant)
{
    FArtCanvas C(256, 320);
    C.Ellipse(128, 303, 64, 14, FColor(20, 24, 15, 120));

    const FColor TrunkDark(70, 43, 28, 255);
    const FColor Trunk(105, 61, 35, 255);
    const FColor TrunkLight(151, 91, 49, 255);
    const FColor LeafDark(25, 76, 36, 255);
    const FColor Leaf(45, 118, 50, 255);
    const FColor LeafLight(104, 163, 66, 255);

    C.Line(128, 292, 122, 174, 19, TrunkDark);
    C.Line(127, 286, 130, 177, 12, Trunk);
    C.Line(124, 230, 82, 173, 8, TrunkDark);
    C.Line(132, 225, 177, 165, 8, TrunkDark);
    C.Line(128, 225, 91, 183, 5, TrunkLight);
    C.Line(132, 221, 168, 176, 5, TrunkLight);

    AddLeafCluster(C, 74, 115, 47, 42, LeafDark, Leaf, LeafLight);
    AddLeafCluster(C, 129, 83, 55, 48, LeafDark, Leaf, LeafLight);
    AddLeafCluster(C, 184, 119, 47, 43, LeafDark, Leaf, LeafLight);
    AddLeafCluster(C, 91, 153, 49, 39, LeafDark, Leaf, LeafLight);
    AddLeafCluster(C, 153, 150, 54, 43, LeafDark, Leaf, LeafLight);
    AddLeafCluster(C, 127, 123, 58, 51, LeafDark, Leaf, LeafLight);

    for (int32 I = 0; I < 14; ++I)
    {
        const int32 X = 45 + ((I * 47 + Variant * 19) % 165);
        const int32 Y = 68 + ((I * 31 + Variant * 11) % 105);
        C.Ellipse(X, Y, 5, 4, (I % 2) ? LeafLight : FColor(72, 138, 57, 255));
    }

    return BuildSprite(Outer, *FString::Printf(TEXT("AOA_Runtime_Tree_%d"), Variant), C, 1.6f);
}

UPaperSprite* FAetherRuntime2DArt::CreateRockSprite(UObject* Outer, int32 Variant)
{
    FArtCanvas C(192, 144);
    C.Ellipse(96, 126, 61, 13, FColor(22, 23, 22, 120));

    const FColor Dark(57, 61, 64, 255);
    const FColor Mid(99, 103, 104, 255);
    const FColor Light(153, 153, 145, 255);
    const FColor Moss(74, 102, 57, 255);

    C.Triangle(FIntPoint(24, 113), FIntPoint(65, 35), FIntPoint(122, 21), Dark);
    C.Triangle(FIntPoint(122, 21), FIntPoint(167, 77), FIntPoint(165, 113), Mid);
    C.Triangle(FIntPoint(24, 113), FIntPoint(122, 21), FIntPoint(165, 113), Mid);
    C.Triangle(FIntPoint(65, 35), FIntPoint(122, 21), FIntPoint(101, 81), Light);
    C.Ellipse(63 + Variant * 3, 53, 18, 7, Moss);
    C.Ellipse(126, 86, 22, 8, FColor(120, 126, 118, 255));

    return BuildSprite(Outer, *FString::Printf(TEXT("AOA_Runtime_Rock_%d"), Variant), C, 1.5f);
}

UPaperSprite* FAetherRuntime2DArt::CreateHouseSprite(UObject* Outer, int32 Variant)
{
    FArtCanvas C(320, 256);
    C.Ellipse(160, 239, 122, 14, FColor(20, 20, 18, 105));

    const FColor Wall(156, 119, 78, 255);
    const FColor WallLight(193, 157, 103, 255);
    const FColor Timber(76, 53, 39, 255);
    const FColor RoofDark(53, 43, 59, 255);
    const FColor Roof(78, 55, 79, 255);
    const FColor RoofLight(116, 77, 91, 255);
    const FColor Window(77, 129, 151, 255);
    const FColor Door(75, 48, 34, 255);

    C.Rect(55, 105, 265, 222, Wall);
    C.Rect(66, 117, 254, 222, WallLight);
    C.Triangle(FIntPoint(31, 111), FIntPoint(160, 22), FIntPoint(289, 111), RoofDark);
    C.Triangle(FIntPoint(46, 107), FIntPoint(160, 39), FIntPoint(274, 107), Roof);
    C.Triangle(FIntPoint(87, 93), FIntPoint(160, 49), FIntPoint(233, 93), RoofLight);

    C.Rect(146, 155, 177, 222, Door);
    C.Ellipse(170, 188, 3, 3, FColor(225, 182, 76, 255));

    C.Rect(82, 139, 122, 177, Timber);
    C.Rect(198, 139, 238, 177, Timber);
    C.Rect(88, 145, 116, 171, Window);
    C.Rect(204, 145, 232, 171, Window);

    C.Line(56, 110, 264, 110, 6, Timber);
    C.Line(160, 25, 160, 105, 7, Timber);
    C.Line(67, 220, 253, 220, 6, Timber);

    C.Rect(225, 65, 245, 103, Timber);
    C.Rect(228, 60, 242, 72, RoofDark);

    if (Variant % 2 == 1)
    {
        C.Rect(45, 184, 76, 214, Timber);
        C.Rect(48, 187, 73, 211, FColor(110, 75, 49, 255));
    }

    return BuildSprite(Outer, *FString::Printf(TEXT("AOA_Runtime_House_%d"), Variant), C, 1.4f);
}
