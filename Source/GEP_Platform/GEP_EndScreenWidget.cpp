// GEP_EndScreenWidget.cpp
#include "GEP_EndScreenWidget.h"
#include "GEP_PlatformGameMode.h"
#include "Components/Button.h"

void UGEP_EndScreenWidget::NativeConstruct()
{
	Super::NativeConstruct();
	Button_Restart->OnClicked.AddDynamic(this, &UGEP_EndScreenWidget::OnRestartClicked);
}

void UGEP_EndScreenWidget::OnRestartClicked()
{
	if (AGEP_PlatformGameMode* GM = GetWorld()->GetAuthGameMode<AGEP_PlatformGameMode>())
	{
		GM->RestartCurrentLevel();
	}
}