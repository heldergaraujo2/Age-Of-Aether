#include "UI/AetherUIPresentationTypes.h"
bool FAetherUIScreenPresentation::IsValid() const { return Screen != EAetherUIScreen::None && !WidgetClass.IsNull(); }
bool UAetherUIPresentationCatalog::IsValid(TArray<FString>& OutErrors) const
{
    OutErrors.Reset(); TSet<uint8> Seen;
    for (const FAetherUIScreenPresentation& Entry : Screens)
    {
        if (!Entry.IsValid()) { OutErrors.Add(TEXT("Invalid UI screen presentation entry.")); continue; }
        const uint8 Key = static_cast<uint8>(Entry.Screen);
        if (Seen.Contains(Key)) OutErrors.Add(FString::Printf(TEXT("Duplicate UI screen: %d"), Key));
        Seen.Add(Key);
    }
    return OutErrors.IsEmpty();
}
const FAetherUIScreenPresentation* UAetherUIPresentationCatalog::Find(EAetherUIScreen Screen) const
{
    for (const FAetherUIScreenPresentation& Entry : Screens) if (Entry.Screen == Screen) return &Entry;
    return nullptr;
}