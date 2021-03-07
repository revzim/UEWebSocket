/*
* UEWebSocket - Unreal Engine 4 WebSocket Plugin (modified original source from 2017 https://github.com/feixuwu/uewebsocket)
* Copyright (c) 2021 revzim 
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
* 
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
* 
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*/

#pragma once

#include "UObject/NoExportTypes.h"
#include "Tickable.h"

#if PLATFORM_UWP
#elif PLATFORM_HTML5
#elif PLATFORM_WINDOWS  
#include "Windows/PreWindowsApi.h"
#include "libwebsockets.h"
#include "Windows/PostWindowsApi.h" 
#else
#include "libwebsockets.h"
#endif

#include <iostream>

#include "WebSocketContext.generated.h"


class UWebSocketBase;
/**
 * 
 */
UCLASS()
class UWebSocketContext : public UObject, public FTickableGameObject
{
	GENERATED_BODY()
public:

	UWebSocketContext();

	~UWebSocketContext();

	void CreateCtx();

	virtual void BeginDestroy() override;

	virtual void Tick(float DeltaTime) override;
	virtual bool IsTickable() const override;
	virtual TStatId GetStatId() const override;

	UWebSocketBase* Connect(const FString& uri, bool& connectFail);
	UWebSocketBase* Connect(const FString& uri, const TMap<FString, FString>& header, bool& connectFail);
#if PLATFORM_UWP
#elif PLATFORM_HTML5
#else
	static int callback_echo(struct lws *wsi, enum lws_callback_reasons reason, void *user, void *in, size_t len);
#endif
	
private:

#if PLATFORM_UWP
#elif PLATFORM_HTML5
#else
	struct lws_context* mlwsContext;
	std::string mstrCAPath;
#endif
};
