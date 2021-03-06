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

#include "Kismet/BlueprintFunctionLibrary.h"
#include "WebSocketBase.h"
#include "Runtime/Json/Public/Dom/JsonObject.h"
#include "Runtime/JsonUtilities/Public/JsonObjectConverter.h"
#include "Runtime/JsonUtilities/Public/JsonObjectWrapper.h"
#include "Internationalization/Culture.h"
#include "UObject/TextProperty.h"
#include "UObject/PropertyPortFlags.h"
#include "WebSocketBlueprintLibrary.generated.h"


UCLASS(BlueprintType, Blueprintable)
class UTest :public UObject
{
public:

	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Test)
	FString mName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Test)
	int mAge;
};


UCLASS(BlueprintType, Blueprintable)
class UTest2 :public UObject
{
public:

	GENERATED_BODY()

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Test)
		FString mName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Test)
		int mAge;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Test)
	UTest* mTest;
};


USTRUCT(BlueprintType)
struct FWebSocketHeaderPair
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(Category = WebSocket, EditAnywhere, BlueprintReadWrite)
	FString key;

	UPROPERTY(Category = WebSocket, EditAnywhere, BlueprintReadWrite)
	FString value;
};


/**
 * 
 */
UCLASS()
class WEBSOCKET_API UWebSocketBlueprintLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:

	UFUNCTION(BlueprintCallable, Category = "WebSocket")
	static UWebSocketBase* Connect(const FString& url, bool& connectFail);

	UFUNCTION(BlueprintCallable, Category = "WebSocket")
	static UWebSocketBase* ConnectWithHeader(const FString& url, const TArray<FWebSocketHeaderPair>& header, bool& connectFail);

	UFUNCTION(BlueprintCallable, Category = "WebSocket")
	static UObject* JsonToObject(const FString& data, UClass * StructDefinition, bool checkAll);
	
	UFUNCTION(BlueprintCallable, Category = "WebSocket")
	static bool GetJsonIntField(const FString& data, const FString& key, int& iValue);

	UFUNCTION(BlueprintCallable, Category = "WebSocket")
	static bool ObjectToJson(UObject* Object, FString& data);

	static bool JsonValueToUProperty(TSharedPtr<FJsonValue> JsonValue, UProperty* Property, void* OutValue, int64 CheckFlags, int64 SkipFlags);
	static bool ConvertScalarJsonValueToUProperty(TSharedPtr<FJsonValue> JsonValue, UProperty* Property, void* OutValue, int64 CheckFlags, int64 SkipFlags);
	static bool JsonObjectToUStruct(const TSharedRef<FJsonObject>& JsonObject, const UStruct* StructDefinition, void* OutStruct, int64 CheckFlags, int64 SkipFlags);
	static bool JsonAttributesToUStruct(const TMap< FString, TSharedPtr<FJsonValue> >& JsonAttributes, const UStruct* StructDefinition, void* OutStruct, int64 CheckFlags, int64 SkipFlags);
	static bool UObjectToJsonObject(const UStruct* StructDefinition, const void* Struct, TSharedRef<FJsonObject> OutJsonObject, int64 CheckFlags, int64 SkipFlags);
	static bool UObjectToJsonAttributes(const UStruct* StructDefinition, const void* Struct, TMap< FString, TSharedPtr<FJsonValue> >& OutJsonAttributes, int64 CheckFlags, int64 SkipFlags);
	static TSharedPtr<FJsonValue> UPropertyToJsonValue(UProperty* Property, const void* Value, int64 CheckFlags, int64 SkipFlags);
	static TSharedPtr<FJsonValue> ConvertScalarUPropertyToJsonValue(UProperty* Property, const void* Value, int64 CheckFlags, int64 SkipFlags);
};
