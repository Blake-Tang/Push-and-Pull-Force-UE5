// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Pull_Force.generated.h"

UCLASS()
class SPRAYER_API APull_Force : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APull_Force();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere)
	class USphereComponent* SphereCollisionComponent;

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool BFromSweep, const FHitResult& HitResult);

	UFUNCTION()
	void OnEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(VisibleAnywhere)
	TArray <AActor*> ActorArray;

	UPROPERTY(EditAnywhere)
	float PullForce = 4000.0f;

	UPROPERTY(EditAnywhere)
	bool Push = false;

	void PushOrPullForce(FVector UnitDirection, UPrimitiveComponent* InPrimitiveComp);


	FName BoneName;

	TSubclassOf <UPrimitiveComponent> PrimitiveCompClass; 

private: 

	
	

};
