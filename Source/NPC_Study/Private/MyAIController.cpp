// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC_Study/Public/MyAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackBoardComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"


AMyAIController::AMyAIController()
{
	BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("Behavior Tree Component"));
	BlackboardComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("Blackboard Component"));
	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception Component"));

	Sight = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
	Sight->SightRadius = 2000.0f;
	Sight->LoseSightRadius = Sight->SightRadius + 500.0f;
	Sight->PeripheralVisionAngleDegrees = 90.0f; //좌우 시야 반경
	Sight->DetectionByAffiliation.bDetectNeutrals = true;
	//DetectionByAffiliation 는 적 ,중립 ,아군이 이 이 감각을 작동시킬 수 있는지?
	//중립이 이 감각에 감지된다.

	AIPerceptionComponent->ConfigureSense(*Sight); 
	AIPerceptionComponent->SetDominantSense(Sight->GetSenseImplementation());
}

//Stumuls 가 자극제,자극 이란 뜻이다. 감각에 자극을 준 것을 말한게 아닐까?
void AMyAIController::OnPerception(AActor* Actor, FAIStimulus Stimuls)
{
	//매개변수 Actor를 AMyCharacater로 형변환을 한 뒤 Player에 넣는다. AMyCharacter가 아니라면 nullptr이 들어갈 것이다.
	AMyCharacter* Player = Cast<AMyCharacter>(Actor);
	if(Player == nullptr)
	{
		return; //인지된 액터가 플레이어가 아니라면 취소
	}
	
}

//헤더파일에 선언이 되어 있어야 사용가능하다. 따라서 헤더파일에 BeginPlay() 가상함수 선언
void AMyAIController::BeginPlay()
{
	Super::BeginPlay();
	AMyCharacter* Player = Cast<AMyCharacter>(GetPawn()); //이렇게 하면 플레이어는 한명만 지정되지 않나?
	if (IsValid(BehaviorTree.Get()))
	{
		RunBehaviorTree(BehaviorTree.Get());
		BehaviorTreeComponent->StartTree(*BehaviorTree.Get());
	}
	if(Player)
	{
		Agent = Player; //한명의 플레이어만 타겟으로 삼지 않나?!
	}
}

void AMyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	if (IsValid(Blackboard.Get()) && IsValid(BehaviorTree.Get()))
	{
		Blackboard->InitializeBlackboard(*BehaviorTree.Get()->BlackboardAsset.Get());
	}
	AIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this,&AMyAIController::OnPerception);
}
