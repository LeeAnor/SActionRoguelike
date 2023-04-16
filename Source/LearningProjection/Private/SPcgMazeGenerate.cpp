// Fill out your copyright notice in the Description page of Project Settings.


#include "SPcgMazeGenerate.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ASPcgMazeGenerate::ASPcgMazeGenerate()
{
	MeshFloor = CreateDefaultSubobject<UStaticMeshComponent>("MeshFloor");
	RootComponent = MeshFloor;
}

// Called when the game starts or when spawned
void ASPcgMazeGenerate::BeginPlay()
{
	Super::BeginPlay();

	FVector Scalar = UKismetMathLibrary::MakeVector(rows, cols, 1.0f);
	FTransform Transform = UKismetMathLibrary::MakeTransform(GetActorLocation(), GetActorRotation(), Scalar);

	MeshFloor->SetWorldTransform(Transform);	//根据rows和cols来设置地板缩放

	//初始化TArray
	for (int i = 0; i < rows; ++i)
	{
		MazeTypes.SetNum(rows);
		MazeDoors.SetNum(rows);
		MazeTypes[i].SetNum(cols);
		MazeDoors[i].SetNum(cols);
		for (int j = 0; j < cols; ++j)
		{
			MazeTypes[i][j] = i * cols + j;
			MazeDoors[i][j].SetNum(4);
			for (int k = 0; k < 4; ++k)
			{
				MazeDoors[i][j][k] = false;
			}
		}
	}

	GenerateMaze();
}

void ASPcgMazeGenerate::GenerateMaze()
{
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			TArray<int> candidateDoorDir;	//0上，1右，2下，3左
			//是否能向上开门
			if (i > 0 && MazeTypes[i][j] != MazeTypes[i - 1][j]) {
				candidateDoorDir.Add(0);
			}
			//是否能向右开门
			if (j < cols - 1 && MazeTypes[i][j] != MazeTypes[i][j + 1]) {
				candidateDoorDir.Add(1);
			}
			//是否能向下开门
			if (i < rows - 1 && MazeTypes[i][j] != MazeTypes[i + 1][j]) {
				candidateDoorDir.Add(2);
			}
			//是否能向左开门
			if (j > 0 && MazeTypes[i][j] != MazeTypes[i][j - 1]) {
				candidateDoorDir.Add(3);
			}

			if (candidateDoorDir.Num() == 0) {
				break;
			}

			//在可以开门的备选方向里随机选一个方向开门
			int openDoorDir = candidateDoorDir[std::rand() % candidateDoorDir.Num()];
			int newRoomID = -1;
			//开门
			MazeDoors[i][j][openDoorDir] = true; 
			if (openDoorDir == 0) {
				MazeDoors[i - 1][j][2] = true;
				newRoomID = MazeTypes[i - 1][j];
			}
			else if (openDoorDir == 1) {
				MazeDoors[i][j + 1][3] = true; 
				newRoomID = MazeTypes[i][j + 1];
			}
			else if (openDoorDir == 2) {
				MazeDoors[i + 1][j][0] = true;
				newRoomID = MazeTypes[i + 1][j];
			}
			else if (openDoorDir == 3) {
				MazeDoors[i][j - 1][1] = true;
				newRoomID = MazeTypes[i][j - 1];
			}

			//将所有与当前房间编号相同的房间都置为新的房间号,此处可用并查集进一步优化算法
			int oldRoomID = MazeTypes[i][j];
			for (int ii = 0; ii < rows; ii++)
			{
				for (int jj = 0; jj < cols; jj++) 
				{
					if (MazeTypes[ii][jj] == oldRoomID) {
						MazeTypes[ii][jj] = newRoomID;
					}
				}
			}
		}
	}



	//根据MazeDoors生成相应的Wall

	FTransform SpawnTM;		//声明Wall生成矩阵

	FActorSpawnParameters SpawnParameters;	//声明SpawnActor()函数的生成参数
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;//设置SpawnActor()函数的生成参数

	for (int i_wall = 0; i_wall < rows; i_wall++)
	{
		for (int j_wall = 0; j_wall < cols; j_wall++)
		{
			//生成上边的墙壁
			if (!MazeDoors[i_wall][j_wall][0])
			{
				float xPos = j_wall * RoomExtent + GetActorLocation().X;
				float yPos = i_wall * RoomExtent + GetActorLocation().Y;

				SpawnTM = FTransform(FRotator::ZeroRotator, FVector(FIntPoint(xPos, yPos)));
				GetWorld()->SpawnActor<AActor>(MeshWall, SpawnTM, SpawnParameters);
			}
			//生成左边的墙壁
			if (!MazeDoors[i_wall][j_wall][3])
			{
				float xPos = j_wall * RoomExtent + GetActorLocation().X;
				float yPos = i_wall * RoomExtent + GetActorLocation().Y;

				SpawnTM = FTransform(FRotator(0, 90, 0), FVector(FIntPoint(xPos, yPos)));
				GetWorld()->SpawnActor<AActor>(MeshWall, SpawnTM, SpawnParameters);
			}
			//如果是最右的房间, 生成右边的墙壁
			if (j_wall == cols - 1)
			{
				float xPos = cols * RoomExtent + GetActorLocation().X;
				float yPos = i_wall * RoomExtent + GetActorLocation().Y;

				SpawnTM = FTransform(FRotator(0, 90, 0), FVector(FIntPoint(xPos, yPos)));
				GetWorld()->SpawnActor<AActor>(MeshWall, SpawnTM, SpawnParameters);
			}
			//如果是最下面的房间,生成下边的墙壁
			if (i_wall == rows - 1)
			{
				float xPos = j_wall * RoomExtent + GetActorLocation().X;
				float yPos = rows * RoomExtent + GetActorLocation().Y;

				SpawnTM = FTransform(FRotator::ZeroRotator, FVector(FIntPoint(xPos, yPos)));
				GetWorld()->SpawnActor<AActor>(MeshWall, SpawnTM, SpawnParameters);
			}
		}
	}

}
