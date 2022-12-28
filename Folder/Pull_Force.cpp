// Fill out your copyright notice in the Description page of Project Settings.


#include "Pull_Force.h"
#include "Components/SphereComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Kismet/KismetMathLibrary.h"


APull_Force::APull_Force()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	//Create SphereComponent and attach to root body
	SphereCollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent")); 
	SphereCollisionComponent->SetupAttachment(RootComponent);
	
	//Create delegate
	SphereCollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &APull_Force::OnBeginOverlap);
	SphereCollisionComponent->OnComponentEndOverlap.AddDynamic(this, &APull_Force::OnEndOverlap);


}

void APull_Force::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	//for each Actor in ActorArray, get root component and apply force
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

//When Begin Overlap Happens
void APull_Force::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool BFromSweep, const FHitResult& HitResult)
{

	if (OtherActor && OtherActor->ActorHasTag("Pull"))
	{
		ActorArray.Add(OtherActor);
	}
}

//When End Overlap Happens
void APull_Force::OnEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && ActorArray.Contains(OtherActor))
	{
		ActorArray.Remove(OtherActor);
	}
}

//Add Force Base On Unit Direction
void APull_Force::PushOrPullForce(FVector UnitDirection, UPrimitiveComponent* InPrimitiveComp)  
{
	FVector PullForceVector = UnitDirection * PullForce;
	InPrimitiveComp->AddForce(PullForceVector, BoneName, true);

	return;

}


