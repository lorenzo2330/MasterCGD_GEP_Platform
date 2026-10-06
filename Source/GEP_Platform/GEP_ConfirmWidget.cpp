// Fill out your copyright notice in the Description page of Project Settings.


#include "GEP_ConfirmWidget.h"

#include "Components/Button.h"

void UGEP_ConfirmWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	ButtonConfirm->OnClicked.AddDynamic(this, &UGEP_ConfirmWidget::OnConfirmClicked);
	ButtonCancel->OnClicked.AddDynamic(this, &UGEP_ConfirmWidget::OnCancelClicked);
	
	EnterUIMode();
}

void UGEP_ConfirmWidget::OnConfirmClicked()
{
	OnConfirmed.Broadcast();
}

void UGEP_ConfirmWidget::OnCancelClicked()
{
	OnCancelled.Broadcast();
}
