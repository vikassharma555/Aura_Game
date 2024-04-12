// Copyright (Vikas)


#include "UI/HUD/AuraHUD.h"

#include "Blueprint/UserWidget.h"
#include "UI/Widget/AuraUserWidget.h"
#include "UI/WidgetController/OverlayWidgetController.h"

UOverlayWidgetController* AAuraHUD::GetOverlayWidgetController(const FWidgetControllerParams& WCParams)
{
	if (OverlayWidgetController == nullptr)
	{
		OverlayWidgetController = NewObject<UOverlayWidgetController>(this, OverlayWidgetControllerClass);
		OverlayWidgetController->SetWidgetControllerParams(WCParams);
		OverlayWidgetController->BindCallbacksToDependencies();
		return OverlayWidgetController;
	}
	return OverlayWidgetController;
}

void AAuraHUD::InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS)
{
	checkf(OverlayWidgetClass, TEXT("Overlay widget class uniniatializedd, please fill out BP_AUra"));
	checkf(OverlayWidgetControllerClass, TEXT("Overlay widget controller class uniniatializedd, please fill out BP_AUra"));

	UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), OverlayWidgetClass);
	OverlayWidget = Cast<UAuraUserWidget>(Widget);

	const FWidgetControllerParams WidgetContoollerParams (PC, PS, ASC,AS);
	UOverlayWidgetController* WidgetController = GetOverlayWidgetController(WidgetContoollerParams);

	OverlayWidget->SetWidgetController(WidgetController);
	WidgetController->BroadCastInitialValues();
	Widget->AddToViewport();
}


