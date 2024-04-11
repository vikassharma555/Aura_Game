// Copyright (Vikas)

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AuraUserWidget.generated.h"


/**
 * 
 */
UCLASS()
class AURA_API UAuraUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void SetWidgetController(UObject* InWidgetController);
	
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UObject> WidgetController;

protected:
	// make a function like evenBegin to be called on blueprint
	UFUNCTION(BlueprintImplementableEvent)
	void WidgetControllerSet();
	
};
