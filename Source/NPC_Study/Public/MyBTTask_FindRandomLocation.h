// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "MyBTTask_FindRandomLocation.generated.h"

/**
 * 
 */
UCLASS()
class NPC_STUDY_API UMyBTTask_FindRandomLocation : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	UMyBTTask_FindRandomLocation();

private:
	//EBTNodeResult::type은 노드의 성공 여부를 나타내는 타입들이 들어있다.
	//EBTNodeResult::Type ExecuteTask 를 통해 노드의 성공여부를 받아올 수 있다.
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual FString GetStaticDescription() const override;

	//탐색 범위
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Search",meta=(AllowPrivateAccess=true))
	float SearchRadius = {500.0f};
	
		
};


