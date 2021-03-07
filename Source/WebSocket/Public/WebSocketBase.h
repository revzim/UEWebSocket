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

#include "Components/ActorComponent.h"
#include "UObject/NoExportTypes.h"
#include "Delegates/DelegateCombinations.h"
#include "WebSocketBase.generated.h"


DEFINE_LOG_CATEGORY_STATIC(WebSocket, Log, All);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWebSocketConnectError, const FString&, error);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FWebSocketClosed);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FWebSocketConnected);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWebSocketRecieve, const FString&, data);


#if PLATFORM_UWP
#include <collection.h>
#include <ppltasks.h>

class UWebSocketBase;

ref class FUWPSocketHelper sealed
{
public:
	FUWPSocketHelper();
	virtual ~FUWPSocketHelper();

	void MessageReceived(Windows::Networking::Sockets::MessageWebSocket^ sender, Windows::Networking::Sockets::MessageWebSocketMessageReceivedEventArgs^ args);
	void OnUWPClosed(Windows::Networking::Sockets::IWebSocket^ sender, Windows::Networking::Sockets::WebSocketClosedEventArgs^ args);
	void SetParent(int64 p);

private:
	int64 Parent;
};


#elif PLATFORM_HTML5
#include "Tickable.h"

extern "C" 
{
	int SocketCreate(const char* url);
	int SocketState(int socketInstance);
	void SocketSend(int socketInstance, const char* ptr, int length);
	void SocketRecv(int socketInstance, char* ptr, int length);
	int SocketRecvLength(int socketInstance);
	void SocketClose(int socketInstance);
	int SocketError(int socketInstance, char* ptr, int length);
}

class UWebSocketBase;

class FHtml5SocketHelper :public FTickableGameObject
{
public:

	FHtml5SocketHelper()
	{
		mHostWebSocket = nullptr;
	}

	void Bind(UWebSocketBase* host)
	{
		mHostWebSocket = host;
	}

	void UnBind()
	{
		mHostWebSocket = nullptr;
	}

	~FHtml5SocketHelper()
	{
		mHostWebSocket = nullptr;
	}

	virtual void Tick(float DeltaTime) override;
	virtual bool IsTickable() const override;
	virtual TStatId GetStatId() const override;

private:

	UWebSocketBase* mHostWebSocket;
};

#else
struct lws_context;
struct lws;
#endif

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class WEBSOCKET_API UWebSocketBase:public UObject
{
	GENERATED_BODY()
public:

	UWebSocketBase();

	virtual void BeginDestroy() override;
	
	UFUNCTION(BlueprintCallable, Category = WebSocket)
	void SendText(const FString& data);

	UFUNCTION(BlueprintCallable, Category = WebSocket)
	void Close();

	bool Connect(const FString& uri, const TMap<FString, FString>& header);

#if PLATFORM_UWP
	Concurrency::task<void> ConnectAsync(Platform::String^ uriString);
	void MessageReceived(Windows::Networking::Sockets::MessageWebSocket^ sender, Windows::Networking::Sockets::MessageWebSocketMessageReceivedEventArgs^ args);
	void OnUWPClosed(Windows::Networking::Sockets::IWebSocket^ sender, Windows::Networking::Sockets::WebSocketClosedEventArgs^ args);
	Concurrency::task<void> SendAsync(Platform::String^ message);
#endif

	UPROPERTY(BlueprintAssignable, Category = WebSocket)
	FWebSocketConnectError OnConnectError;

	UPROPERTY(BlueprintAssignable, Category = WebSocket)
	FWebSocketClosed OnClosed;

	UPROPERTY(BlueprintAssignable, Category = WebSocket)
	FWebSocketConnected OnConnectComplete;

	UPROPERTY(BlueprintAssignable, Category = WebSocket)
	FWebSocketRecieve OnReceiveData;

	void Cleanlws();
	void ProcessWriteable();
	void ProcessRead(const char* in, int len);
	bool ProcessHeader(unsigned char** p, unsigned char* end);

#if PLATFORM_UWP
	Windows::Networking::Sockets::MessageWebSocket^ messageWebSocket;
	Windows::Storage::Streams::DataWriter^ messageWriter;
	FUWPSocketHelper^ uwpSocketHelper;
#elif PLATFORM_HTML5
	int mWebSocketRef;
	bool mConnectSuccess;
	bool mIsError;
	FHtml5SocketHelper mHtml5SocketHelper;
#else
	struct lws_context* mlwsContext;
	struct lws* mlws;
#endif
	
	TArray<FString> mSendQueue;
	TMap<FString, FString> mHeaderMap;
};
