#include "Data/AetherContentRegistry.h"

bool FAetherContentRegistry::RegisterDefinition(const FAetherContentDefinition& Definition, FString& OutError)
{
    OutError.Reset();

    if (!Definition.IsStructurallyValid(OutError))
    {
        return false;
    }

    const FString ID = Definition.Metadata.DefinitionID.TrimStartAndEnd();
    if (Definitions.Contains(ID))
    {
        OutError = FString::Printf(TEXT("Duplicate DefinitionID '%s'."), *ID);
        return false;
    }

    FAetherContentDefinition Copy = Definition;
    Copy.Metadata.DefinitionID = ID;
    Definitions.Add(ID, MoveTemp(Copy));
    return true;
}

bool FAetherContentRegistry::RegisterDefinitions(
    const TArray<FAetherContentDefinition>& InDefinitions,
    TArray<FAetherContentValidationIssue>& OutIssues)
{
    OutIssues.Reset();
    bool bSuccess = true;

    for (const FAetherContentDefinition& Definition : InDefinitions)
    {
        FString Error;
        if (!RegisterDefinition(Definition, Error))
        {
            bSuccess = false;
            AddIssue(OutIssues, Definition.Metadata.DefinitionID, TEXT("RegisterFailed"), Error);
        }
    }

    if (!Validate(OutIssues))
    {
        bSuccess = false;
    }

    return bSuccess;
}

bool FAetherContentRegistry::Resolve(const FString& DefinitionID, FAetherContentDefinition& OutDefinition) const
{
    const FString NormalizedID = DefinitionID.TrimStartAndEnd();
    const FAetherContentDefinition* Found = Definitions.Find(NormalizedID);
    if (!Found)
    {
        return false;
    }

    OutDefinition = *Found;
    return true;
}

bool FAetherContentRegistry::Contains(const FString& DefinitionID) const
{
    return Definitions.Contains(DefinitionID.TrimStartAndEnd());
}

bool FAetherContentRegistry::Remove(const FString& DefinitionID)
{
    return Definitions.Remove(DefinitionID.TrimStartAndEnd()) > 0;
}

void FAetherContentRegistry::Reset()
{
    Definitions.Reset();
}

bool FAetherContentRegistry::Validate(TArray<FAetherContentValidationIssue>& OutIssues) const
{
    OutIssues.Reset();
    bool bValid = true;

    TArray<FString> IDs;
    Definitions.GetKeys(IDs);
    IDs.Sort();

    for (const FString& ID : IDs)
    {
        const FAetherContentDefinition& Definition = Definitions.FindChecked(ID);

        FString Error;
        if (!Definition.IsStructurallyValid(Error))
        {
            AddIssue(OutIssues, ID, TEXT("InvalidDefinition"), Error);
            bValid = false;
        }

        for (const FString& Reference : Definition.References)
        {
            const FString NormalizedReference = Reference.TrimStartAndEnd();
            if (NormalizedReference.IsEmpty())
            {
                AddIssue(OutIssues, ID, TEXT("EmptyReference"), TEXT("A dependency reference is empty."));
                bValid = false;
                continue;
            }

            if (!Definitions.Contains(NormalizedReference))
            {
                AddIssue(OutIssues, ID, TEXT("MissingReference"),
                    FString::Printf(TEXT("Referenced definition '%s' does not exist."), *NormalizedReference));
                bValid = false;
            }
        }
    }

    TMap<FString, uint8> VisitState;
    for (const FString& ID : IDs)
    {
        VisitState.Add(ID, 0);
    }

    TFunction<bool(const FString&)> Visit = [&](const FString& ID)
    {
        uint8& State = VisitState.FindChecked(ID);
        if (State == 1)
        {
            AddIssue(OutIssues, ID, TEXT("CircularDependency"), TEXT("Circular content dependency detected."));
            return false;
        }
        if (State == 2)
        {
            return true;
        }

        State = 1;
        const FAetherContentDefinition& Definition = Definitions.FindChecked(ID);
        bool bOK = true;

        TArray<FString> References = Definition.References;
        References.Sort();
        for (const FString& Reference : References)
        {
            const FString NormalizedReference = Reference.TrimStartAndEnd();
            if (!Definitions.Contains(NormalizedReference))
            {
                bOK = false;
                continue;
            }
            if (!Visit(NormalizedReference))
            {
                bOK = false;
            }
        }

        State = 2;
        return bOK;
    };

    for (const FString& ID : IDs)
    {
        if (!Visit(ID))
        {
            bValid = false;
        }
    }

    return bValid && OutIssues.Num() == 0;
}

void FAetherContentRegistry::GetDefinitionIDs(TArray<FString>& OutIDs) const
{
    Definitions.GetKeys(OutIDs);
    OutIDs.Sort();
}

void FAetherContentRegistry::AddIssue(
    TArray<FAetherContentValidationIssue>& OutIssues,
    const FString& DefinitionID,
    const FString& Code,
    const FString& Message) const
{
    FAetherContentValidationIssue& Issue = OutIssues.AddDefaulted_GetRef();
    Issue.DefinitionID = DefinitionID;
    Issue.Code = Code;
    Issue.Message = Message;
}
