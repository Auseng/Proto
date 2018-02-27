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

	// Set up "look" bindings.
	PlayerInputComponent->BindAxis("Turn", this, &AProtoPlayer::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &AProtoPlayer::AddControllerPitchInput);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AProtoPlayer::StartJump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &AProtoPlayer::StopJump);

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
		// Find out which way is "right" and record that the player wants to move that way.
		FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::Y);
		AddMovementInput(Direction, Value);
	}

	void AProtoPlayer::StartJump()
	{
		bPressedJump = true;
	}

	void AProtoPlayer::StopJump()
	{
		bPressedJump = false;
	}

	void AProtoPlayer::Fire()
	{
		// Attempt to fire a projectile.
		if (ProjectileClass)
		{
			// Get the camera transform.
			FVector CameraLocation;
			FRotator CameraRotation;
			GetActorEyesViewPoint(CameraLocation, CameraRotation);

			// Transform MuzzleOffset from camera space to world space.
			FVector MuzzleLocation = CameraLocation + FTransform(CameraRotation).TransformVector(MuzzleOffset);
			FRotator MuzzleRotation = CameraRotation;
			// Skew the aim to be slightly upwards.
			MuzzleRotation.Pitch += 10.0f;
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