// Korepanov Kirill, 2017

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameViewportClient.h"
#include "LocalMPViewportClient.generated.h"

/**
 * 
 */
UCLASS()
class UNREALBOMBER_API ULocalMPViewportClient : public UGameViewportClient
{
	GENERATED_BODY()
	

	virtual bool InputKey(FViewport* Viewport, int32 ControllerId, FKey Key, EInputEvent EventType, float AmountDepressed = 1.f, bool bGamepad = false) override;
};
