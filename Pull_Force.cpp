// Fill out your copyright notice in the Description page of Project Settings.


#include "Pull_Force.h"
#include "Components/SphereComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
APull_Force::APull_Force()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereCollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent")); 

	SphereCollisionComponent->SetupAttachment(RootComponent);
	SphereCollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &APull_Force::OnBeginOverlap);
	SphereCollisionComponent->OnComponentEndOverlap.AddDynamic(this, &APull_Force::OnEndOverlap);


}

// Called when the game starts or when spawned
void APull_Force::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APull_Force::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	for (AActor* Actor : ActorArray)
	{
	
		if (!Actor)
		{
			return;
		}

		UPrimitiveComponent* PrimitiveComp = Cast<UPrimitiveComponent>(Actor->GetRootComponent());
		
		FVector SceneRootLocation = RootComponent->GetComponentLocation();

		if (Push)
		{
			FVector UnitDirectionalVector = UKismetMathLibrary::GetDirectionUnitVector(SceneRootLocation, Actor->GetActorLocation());
			PushOrPullForce(UnitDirectionalVector, PrimitiveComp);
		}
		else {
			FVector UnitDirectionalVector = UKismetMathLibrary::GetDirectionUnitVector(Actor->GetActorLocation(), SceneRootLocation);
			PushOrPullForce(UnitDirectionalVector, PrimitiveComp);
		}

	}

}

void APull_Force::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool BFromSweep, const FHitResult& HitResult)
{

	if (OtherActor && OtherActor->ActorHasTag("Pull"))
	{
		ActorArray.Add(OtherActor);
	}
}

void APull_Force::OnEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && ActorArray.Contains(OtherActor))
	{
		ActorArray.Remove(OtherActor);
	}
}

void APull_Force::PushOrPullForce(FVector UnitDirection, UPrimitiveComponent* InPrimitiveComp)  
{
	FVector PullForceVector = UnitDirection * PullForce;
	InPrimitiveComp->AddForce(PullForceVector, BoneName, true);

	return;

}


