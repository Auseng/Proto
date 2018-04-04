// Fill out your copyright notice in the Description page of Project Settings.

#include "ProtoPlayer.h"
#include "Proto.h"
#include "Bullet.h"


// Sets default values
AProtoPlayer::AProtoPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AProtoPlayer::BeginPlay()
{
Super::BeginPlay();


	if (GEngine)
	{
		// Put up a debug message for five seconds. The -1 "Key" value (first argument) indicates that we will never need to update or refresh this message.
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("We are using ProtoPlayer."));
	
	
}
}

// Called every frame
void AProtoPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AProtoPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Set up "movement" bindings.
	PlayerInputComponent->BindAxis("MoveForward", this, &AProtoPlayer::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AProtoPlayer::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &AProtoPlayer::AddControllerYawInput);

	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AProtoPlayer::Fire);
	}

	void AProtoPlayer::MoveForward(float Value)
	{
		// Find out which way is "forward" and record that the player wants to move that way.
		FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}

	void AProtoPlayer::MoveRight(float Value)
	{
		const FRotator YawRotation(0.f, 0.f, 0.f);

		// Find out which way is "right" and record that the player wants to move that way.
		FVector Direction = FRotationMatrix(YawRotation).GetScaledAxis(EAxis::Y); //FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::Y);
		AddMovementInput(Direction, Value);
	}

	void AProtoPlayer::Fire()
	{
		// Attempt to fire a projectile.
		if (ProjectileClass)
		{
			// Get the camera transform.
			FVector MouseLocation;
			FRotator MouseRotation;
			GetActorEyesViewPoint(MouseLocation, MouseRotation);

			// Transform MuzzleOffset from camera space to world space.
			FVector MuzzleLocation = MouseLocation + FTransform(MouseRotation).TransformVector(MuzzleOffset);
			FRotator MuzzleRotation = MouseRotation;
			// Skew the aim to be slightly upwards.
			MuzzleRotation.Pitch += 0.0f;
			UWorld* World = GetWorld();
			if (World)
			{
				FActorSpawnParameters SpawnParams;
				SpawnParams.Owner = this;
				SpawnParams.Instigator = Instigator;
				// Spawn the projectile at the muzzle.
				ABullet* Projectile = World->SpawnActor<ABullet>(ProjectileClass, MuzzleLocation, MuzzleRotation, SpawnParams);
				if (Projectile)
				{
					// Set the projectile's initial trajectory.
					FVector LaunchDirection = MuzzleRotation.Vector();
					Projectile->FireInDirection(LaunchDirection);
				}
			}
		}
	}