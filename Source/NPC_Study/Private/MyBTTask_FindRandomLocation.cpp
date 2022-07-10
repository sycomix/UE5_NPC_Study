// Fill out your copyright notice in the Description page of Project Settings.


#include "MyBTTask_FindRandomLocation.h"
#include "NavigationSystem.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

//생성자 함수
UMyBTTask_FindRandomLocation::UMyBTTask_FindRandomLocation()
{
	NodeName = TEXT("Find Random Location");

	//accept only vectors 벡터 값만 이 태스크의 블랙보드 값에 채워질 수 있다.
	BlackboardKey.AddVectorFilter(this,GET_MEMBER_NAME_CHECKED(UMyBTTask_FindRandomLocation,BlackboardKey));
}

EBTNodeResult::Type UMyBTTask_FindRandomLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	FNavLocation Location{};

	//Get ai pawn
	AAIController* AIController = OwnerComp.GetAIOwner();
	const APawn* AIPawn{AIController->GetPawn()};

	//get pawn origin
	const FVector Origin = AIPawn->GetActorLocation();

	//get nav system and find random location
	const UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(GetWorld());
	if(IsValid(NavSystem)&& NavSystem->GetRandomPointInNavigableRadius(Origin,SearchRadius,Location))
	{
		AIController->GetBlackboardComponent()->SetValueAsVector(BlackboardKey.SelectedKeyName,Location.Location);
	}

	//signal the behaviortreecomponent the task finished with a success!
	FinishLatentTask(OwnerComp,EBTNodeResult::Succeeded); //태스크를 종료시킨다.
	return EBTNodeResult::Succeeded;
}

FString UMyBTTask_FindRandomLocation::GetStaticDescription() const
{
	return FString::Printf(TEXT("Vector: %s"),*BlackboardKey.SelectedKeyName.ToString());
}
