// Fill out your copyright notice in the Description page of Project Settings.


#include "LokiCompileTest.h"

// Sets default values for this component's properties
ULokiCompileTest::ULokiCompileTest()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


int32 ULokiCompileTest::GetTestValue() const
{
	return 2;
}

// Called when the game starts
void ULokiCompileTest::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void ULokiCompileTest::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

