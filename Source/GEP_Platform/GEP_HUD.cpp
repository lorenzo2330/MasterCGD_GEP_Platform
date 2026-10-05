#include "GEP_HUD.h"
#include "GEP_UserWidget.h"

void AGEP_HUD::BeginPlay()
{
	Super::BeginPlay();

	if (WidgetClass && GetOwningPlayerController())
	{
		Widget = CreateWidget<UGEP_UserWidget>(GetOwningPlayerController(), WidgetClass);
		if (Widget)
		{
			Widget->AddToViewport();
		}
	}
}