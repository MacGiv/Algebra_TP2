#include "raylib.h"
#include "math.h"
#include <iostream>

float perimeter;
float area;

struct Corners {
	Vector3 c1;
	Vector3 c2;
	Vector3 c3;
	Vector3 c4;
};

Vector3 Vector3CrossProduct(Vector3 v1, Vector3 v2);
Vector3 CalculateAuxVector(Vector3 vector);
Vector3 AddVectors(Vector3 v1, Vector3 v2);
Vector3 AddVectors(Vector3 v1, Vector3 v2, Vector3 v3);
Vector3 AddVectors(Vector3 v1, Vector3 v2, Vector3 v3, Vector3 v4);
Vector3 VectorDivision(Vector3 vector, float divisor);
Vector3 Negative(Vector3 vector);

bool IsVectorNull(Vector3 vector);
float Vector3Magnitude(Vector3 vector);
void SetVectorMagnitude(Vector3& vector, float desiredLength);

void DrawNextSegment(Corners cornersFloor, Vector3 vectorAMini, Vector3 vectorBMini, Vector3 vectorC);
void DrawSurface(Corners corners);
void DrawWall(Corners cornersFloor, Corners cornersCeiling);
void DrawLine(Vector3 startPos, Vector3 endPos, Color color);

Corners GetNextFloorCorners(Corners previousFloorCorners, Vector3 vectorAMini, Vector3 vectorBMini, Vector3 vectorC);

float RoundN(float number, int numberOfDecimals);

void main()
{
	srand(time(0));

	const int screenWidth = 1600;
	const int screenHeight = 800;

	Vector3 vectorA;
	Vector3 vectorB;
	Vector3 vectorC;
	Vector3 vectorAux;

	Camera camera = { 0 };
	Camera* auxCamera = &camera;

	float inputParameter = 10.0f;

	// Definición de vectores.
	vectorAux.x = 5.0f;
	vectorAux.y = 5.0f;
	vectorAux.z = 5.0f;

	vectorA = CalculateAuxVector(vectorAux);
	vectorB = Vector3CrossProduct(vectorA, vectorAux);
	vectorC = Vector3CrossProduct(vectorA, vectorB);

	// Seteo de magnitudes.
	SetVectorMagnitude(vectorA, 5.0f); // Hay que borrarla para la version final, pero para eso hay que arreglar la camara
	SetVectorMagnitude(vectorB, Vector3Magnitude(vectorA));
	SetVectorMagnitude(vectorC, Vector3Magnitude(vectorA) / inputParameter);

	// Vectores auxiliares
	Vector3 vectorAMini = VectorDivision(vectorA, inputParameter);
	Vector3 vectorBMini = VectorDivision(vectorB, inputParameter);

	// Cálculo de esquinas de la base
	Corners cornersBaseFloor;
	cornersBaseFloor.c1 = AddVectors(Negative(VectorDivision(vectorA, 2)), Negative(VectorDivision(vectorB, 2))); // -A/2 - B/2
	cornersBaseFloor.c2 = AddVectors(cornersBaseFloor.c1, vectorA); // C1 + A
	cornersBaseFloor.c3 = AddVectors(cornersBaseFloor.c1, vectorB); // C1 + B
	cornersBaseFloor.c4 = AddVectors(cornersBaseFloor.c1, vectorA, vectorB); // C1 + A + B

	// Configuración de cámara.
	{
		int cameraMode = CAMERA_FIRST_PERSON;

		Camera position;
		camera.position.x = 10.0f;
		camera.position.y = 10.0f;
		camera.position.z = 10.0f;

		// Camera looking at point
		camera.target.x = 0.0f;
		camera.target.y = 0.0f;
		camera.target.z = 0.0f;

		// Camera up vector (rotation towards target)
		camera.up.x = 0.0f;
		camera.up.y = 1.0f;
		camera.up.z = 0.0f;

		camera.fovy = 45.0f;
		camera.projection = CAMERA_PERSPECTIVE;
	}
	

	InitWindow(screenWidth, screenHeight, "Piramide Escalonada");
	while (!WindowShouldClose())
	{
		perimeter = 0.0f;
		area = 2.0f * pow(Vector3Magnitude(vectorA), 2.0f);

		BeginDrawing();
		ClearBackground(BLACK);
		BeginMode3D(camera);

		DrawNextSegment(cornersBaseFloor, vectorAMini, vectorBMini, vectorC);

		EndMode3D();

		EndDrawing();

		std::cout << "Perímetro: " << perimeter << std::endl;
		std::cout << "Área: " << area << std::endl;
	}

}

Vector3 Vector3CrossProduct(Vector3 v1, Vector3 v2)
{
	Vector3 result;

	result.x = v1.y * v2.z - v1.z * v2.y;
	result.y = v1.z * v2.x - v1.x * v2.z;
	result.z = v1.x * v2.y - v1.y * v2.x;

	return result;
}

Vector3 CalculateAuxVector(Vector3 vector)
{
	Vector3 vectorAux;
	
	do
	{
		vectorAux.x = rand();
		vectorAux.y = rand();
		vectorAux.z = rand();

	} while (IsVectorNull(Vector3CrossProduct(vector, vectorAux)));


	return vectorAux;
}

Vector3 AddVectors(Vector3 v1, Vector3 v2)
{
	Vector3 result;

	result.x = v1.x + v2.x;
	result.y = v1.y + v2.y;
	result.z = v1.z + v2.z;

	return result;
}

Vector3 AddVectors(Vector3 v1, Vector3 v2, Vector3 v3)
{
	return AddVectors(AddVectors(v1, v2), v3);
}

Vector3 AddVectors(Vector3 v1, Vector3 v2, Vector3 v3, Vector3 v4)
{
	return AddVectors(AddVectors(v1, v2, v3), v4);
}

Vector3 VectorDivision(Vector3 vector, float divisor)
{
	Vector3 result;

	result.x = vector.x / divisor;
	result.y = vector.y / divisor;
	result.z = vector.z / divisor;

	return result;
}

Vector3 Negative(Vector3 vector)
{
	Vector3 result;

	result.x = -vector.x;
	result.y = -vector.y;
	result.z = -vector.z;

	return result;
}

bool IsVectorNull(Vector3 vector)
{
	return vector.x == 0 && vector.y == 0 && vector.z == 0;
}

float Vector3Magnitude(Vector3 vector)
{
	float vectorMagnitude;

	vectorMagnitude = abs(sqrt(pow(vector.x, 2.0f) + pow(vector.y, 2.0f) + pow(vector.z, 2.0f)));

	return vectorMagnitude;
}

void SetVectorMagnitude(Vector3& vector, float desiredLength)
{
	float originalLength = Vector3Magnitude(vector);

	vector.x *= desiredLength / originalLength;

	vector.y *= desiredLength / originalLength;

	vector.z *= desiredLength / originalLength;
}

void DrawNextSegment(Corners cornersFloor, Vector3 vectorAMini, Vector3 vectorBMini, Vector3 vectorC)
{
	Corners cornersCeiling;
	cornersCeiling.c1 = AddVectors(cornersFloor.c1, vectorC);
	cornersCeiling.c2 = AddVectors(cornersFloor.c2, vectorC);
	cornersCeiling.c3 = AddVectors(cornersFloor.c3, vectorC);
	cornersCeiling.c4 = AddVectors(cornersFloor.c4, vectorC);

	DrawSurface(cornersFloor); // Piso
	DrawWall(cornersFloor, cornersCeiling); // Pared
	DrawSurface(cornersCeiling); // Techo

	if (RoundN(Vector3Magnitude(AddVectors(cornersCeiling.c1, Negative(cornersCeiling.c2))), 2) > 2.0f * RoundN(Vector3Magnitude(vectorC), 2)) 
	{
		// Cálculo de esquinas (p1)
		Corners cornersFloorNextSegment = GetNextFloorCorners(cornersFloor, vectorAMini, vectorBMini, vectorC);

		DrawNextSegment(cornersFloorNextSegment, vectorAMini, vectorBMini, vectorC);
	}
}

void DrawSurface(Corners corners)
{
	DrawLine(corners.c1, corners.c2, RED);
	DrawLine(corners.c1, corners.c3, BLUE);
	DrawLine(corners.c3, corners.c4, RED);
	DrawLine(corners.c2, corners.c4, BLUE);
}

void DrawWall(Corners cornersFloor, Corners cornersCeiling)
{
	DrawLine(cornersFloor.c1, cornersCeiling.c1, GREEN);
	DrawLine(cornersFloor.c2, cornersCeiling.c2, GREEN);
	DrawLine(cornersFloor.c3, cornersCeiling.c3, GREEN);
	DrawLine(cornersFloor.c4, cornersCeiling.c4, GREEN);
	float base = Vector3Magnitude(AddVectors(cornersFloor.c2, Negative(cornersFloor.c1))); // | C2 - C1 |
	float height = Vector3Magnitude(AddVectors(cornersCeiling.c1, Negative(cornersFloor.c1)));
	area += 4.0f * base * height; // Las cuatro paredes
}

void DrawLine(Vector3 startPos, Vector3 endPos, Color color)
{
	DrawLine3D(startPos, endPos, color);
	perimeter += Vector3Magnitude(AddVectors(endPos, Negative(startPos))); // | endPos - startPos |
}

Corners GetNextFloorCorners(Corners previousFloorCorners, Vector3 vectorAMini, Vector3 vectorBMini, Vector3 vectorC)
{
	Corners nextFloorCorners;
	nextFloorCorners.c1 = AddVectors(previousFloorCorners.c1, vectorAMini, vectorBMini, vectorC); // C1 + A + B + C
	nextFloorCorners.c2 = AddVectors(previousFloorCorners.c2, Negative(vectorAMini), vectorBMini, vectorC); // C2 - A + B + C
	nextFloorCorners.c3 = AddVectors(previousFloorCorners.c3, vectorAMini, Negative(vectorBMini), vectorC); // C3 + A - B + C
	nextFloorCorners.c4 = AddVectors(previousFloorCorners.c4, Negative(vectorAMini), Negative(vectorBMini), vectorC); // C4 - A - B + C
	return nextFloorCorners;
}

float RoundN(float number, int numberOfDecimals)
{
	return round(number * pow(10.0f, numberOfDecimals)) / pow(10.0f, numberOfDecimals);
}
