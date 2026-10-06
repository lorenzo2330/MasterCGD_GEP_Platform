#pragma once

#include "CoreMinimal.h"
#include "GEP_MenuWidgetBase.h"
#include "GEP_ConfirmWidget.generated.h"

class UButton;
class UTextBlock;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnConfirmResult);

UCLASS(Abstract)
class GEP_PLATFORM_API UGEP_ConfirmWidget : public UGEP_MenuWidgetBase
{
	GENERATED_BODY()

public:
	/** Fired when the player confirms. */
	UPROPERTY(BlueprintAssignable, Category = "Confirm")
	FOnConfirmResult OnConfirmed;

	/** Fired when the player cancels. */
	UPROPERTY(BlueprintAssignable, Category = "Confirm")
	FOnConfirmResult OnCancelled;

protected:
	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget)) TObjectPtr<UButton> ButtonConfirm;
	UPROPERTY(meta = (BindWidget)) TObjectPtr<UButton> ButtonCancel;

	UFUNCTION() void OnConfirmClicked();
	UFUNCTION() void OnCancelClicked();
};