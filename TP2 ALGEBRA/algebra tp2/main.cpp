#include "raylib.h"
#include "math.h"
#include <iostream>

// Variables globales para almacenar per�metro y �rea
float perimeter;
float area;

// Estructura que representa las esquinas de una figura 3D
struct Corners {
    Vector3 c1; // Primera esquina
    Vector3 c2; // Segunda esquina
    Vector3 c3; // Tercera esquina
    Vector3 c4; // Cuarta esquina
};

// Declaraci�n de funciones utilizadas m�s adelante en el c�digo
Vector3 Vector3CrossProduct(Vector3 v1, Vector3 v2);                    // Producto cruzado de dos vectores
Vector3 CalculateAuxVector(Vector3 vector);                             // C�lculo de vector auxiliar
Vector3 AddVectors(Vector3 v1, Vector3 v2);                             // Suma de dos vectores
Vector3 AddVectors(Vector3 v1, Vector3 v2, Vector3 v3);                 // Suma de tres vectores
Vector3 AddVectors(Vector3 v1, Vector3 v2, Vector3 v3, Vector3 v4);     // Suma de cuatro vectores
Vector3 VectorDivision(Vector3 vector, float divisor);                  // Divisi�n de un vector por un escalar
Vector3 Negative(Vector3 vector);                                       // Negaci�n de un vector (multiplicaci�n por -1)

bool IsVectorNull(Vector3 vector);                                      // Verifica si un vector es nulo (todos los componentes son 0)
float Vector3Magnitude(Vector3 vector);                                 // Calcula la magnitud de un vector
void SetVectorMagnitude(Vector3& vector, float desiredLength);          // Ajusta la magnitud de un vector

void DrawNextSegment(Corners cornersFloor, Vector3 vectorAMini, Vector3 vectorBMini, Vector3 vectorC); // Dibuja el siguiente segmento de la pir�mide
void DrawSurface(Corners corners);                                      // Dibuja una superficie plana
void DrawWall(Corners cornersFloor, Corners cornersCeiling);            // Dibuja las paredes entre el piso y el techo de un segmento
void DrawLine(Vector3 startPos, Vector3 endPos, Color color);           // Dibuja una l�nea 3D entre dos puntos

// Calcula las esquinas del siguiente piso
Corners GetNextFloorCorners(Corners previousFloorCorners, Vector3 vectorAMini, Vector3 vectorBMini, Vector3 vectorC); 

float RoundN(float number, int numberOfDecimals);                       // Redondea un n�mero a un n�mero espec�fico de decimales

void main()
{
    srand(time(0));

    // Dimensiones de la ventana
    const int screenWidth = 1600;
    const int screenHeight = 800;

    // Declaraci�n de vectores para definir las direcciones de los escalones
    Vector3 vectorA;
    Vector3 vectorB;
    Vector3 vectorC;
    Vector3 vectorAux;

    // Configuraci�n inicial de la c�mara
    Camera camera = { 0 };
    Camera* auxCamera = &camera;

    float inputParameter = 10.0f; // Par�metro para escalar los vectores

    // Definici�n de un vector auxiliar aleatorio
    vectorAux.x = rand();
    vectorAux.y = rand();
    vectorAux.z = rand();

    // C�lculo de los vectores A, B y C con base en el vector auxiliar
    vectorA = CalculateAuxVector(vectorAux);
    vectorB = Vector3CrossProduct(vectorA, vectorAux); // B es perpendicular a A
    vectorC = Vector3CrossProduct(vectorA, vectorB); // C es perpendicular a A y B

    // Ajuste de las magnitudes de los vectores
    SetVectorMagnitude(vectorA, 5.0f); // Establece la longitud de A en 5
    SetVectorMagnitude(vectorB, Vector3Magnitude(vectorA)); // B tiene la misma longitud que A
    SetVectorMagnitude(vectorC, Vector3Magnitude(vectorA) / inputParameter); // C es una fracci�n de A

    // Vectores A y B en versi�n reducida (mini)
    Vector3 vectorAMini = VectorDivision(vectorA, inputParameter); // Divide A por inputParameter
    Vector3 vectorBMini = VectorDivision(vectorB, inputParameter); // Divide B por inputParameter

    // C�lculo de las esquinas de la base de la pir�mide
    Corners cornersBaseFloor;
    cornersBaseFloor.c1 = AddVectors(Negative(VectorDivision(vectorA, 2)), Negative(VectorDivision(vectorB, 2))); // Esquina inferior izquierda
    cornersBaseFloor.c2 = AddVectors(cornersBaseFloor.c1, vectorA); // Esquina inferior derecha
    cornersBaseFloor.c3 = AddVectors(cornersBaseFloor.c1, vectorB); // Esquina superior izquierda
    cornersBaseFloor.c4 = AddVectors(cornersBaseFloor.c1, vectorA, vectorB); // Esquina superior derecha

    // Configuraci�n de la c�mara en primera persona
    int cameraMode = CAMERA_FIRST_PERSON;

    // Posici�n y orientaci�n inicial de la c�mara
    camera.position.x = 10.0f;
    camera.position.y = 10.0f;
    camera.position.z = 10.0f;
    camera.target.x = 0.0f;
    camera.target.y = 0.0f;
    camera.target.z = 0.0f;
    camera.up.x = 0.0f;
    camera.up.y = 1.0f;
    camera.up.z = 0.0f;
    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE; // Modo de proyecci�n en perspectiva

    // Inicializaci�n de la ventana de Raylib
    InitWindow(screenWidth, screenHeight, "Piramide Escalonada");
    while (!WindowShouldClose()) // Bucle principal del programa
    {
        // Reseteo de per�metro y �rea en cada iteraci�n
        perimeter = 0.0f;
        area = 2.0f * pow(Vector3Magnitude(vectorA), 2.0f); // C�lculo del �rea base (A^2)

        BeginDrawing(); // Comienza el proceso de dibujo
        ClearBackground(BLACK); // Fondo negro
        BeginMode3D(camera); // Comienza el modo 3D

        // Dibuja el siguiente segmento de la pir�mide
        DrawNextSegment(cornersBaseFloor, vectorAMini, vectorBMini, vectorC);

        EndMode3D(); // Finaliza el modo 3D
        EndDrawing(); // Finaliza el proceso de dibujo

        // Imprime el per�metro y �rea en la consola
        std::cout << "Per�metro: " << perimeter << std::endl;
        std::cout << "�rea: " << area << std::endl;
    }
}

// Funci�n que calcula el producto cruzado de dos vectores
Vector3 Vector3CrossProduct(Vector3 v1, Vector3 v2)
{
    Vector3 result;
    result.x = v1.y * v2.z - v1.z * v2.y;
    result.y = v1.z * v2.x - v1.x * v2.z;
    result.z = v1.x * v2.y - v1.y * v2.x;
    return result;
}

// Crea un vector auxiliar que no es paralelo al vector dado
Vector3 CalculateAuxVector(Vector3 vector)
{
    Vector3 vectorAux;
    do
    {
        vectorAux.x = rand();
        vectorAux.y = rand();
        vectorAux.z = rand();
    } while (IsVectorNull(Vector3CrossProduct(vector, vectorAux))); // Asegura que el vector no sea paralelo
    return vectorAux;
}

// Funci�n para sumar dos vectores
Vector3 AddVectors(Vector3 v1, Vector3 v2)
{
    Vector3 result;
    result.x = v1.x + v2.x;
    result.y = v1.y + v2.y;
    result.z = v1.z + v2.z;
    return result;
}

// Sobrecarga para sumar tres vectores
Vector3 AddVectors(Vector3 v1, Vector3 v2, Vector3 v3)
{
    return AddVectors(AddVectors(v1, v2), v3);
}

// Sobrecarga para sumar cuatro vectores
Vector3 AddVectors(Vector3 v1, Vector3 v2, Vector3 v3, Vector3 v4)
{
    return AddVectors(AddVectors(v1, v2, v3), v4);
}

// Divide un vector por un escalar
Vector3 VectorDivision(Vector3 vector, float divisor)
{
    Vector3 result;
    result.x = vector.x / divisor;
    result.y = vector.y / divisor;
    result.z = vector.z / divisor;
    return result;
}

// Devuelve el vector negativo
Vector3 Negative(Vector3 vector)
{
    Vector3 result;
    result.x = -vector.x;
    result.y = -vector.y;
    result.z = -vector.z;
    return result;
}

// Verifica si un vector es nulo
bool IsVectorNull(Vector3 vector)
{
    return vector.x == 0 && vector.y == 0 && vector.z == 0;
}

// Calcula la magnitud de un vector
float Vector3Magnitude(Vector3 vector)
{
    return abs(sqrt(pow(vector.x, 2.0f) + pow(vector.y, 2.0f) + pow(vector.z, 2.0f)));
}

// Ajusta la magnitud de un vector a una longitud deseada
void SetVectorMagnitude(Vector3& vector, float desiredLength)
{
    vector = VectorDivision(vector, Vector3Magnitude(vector)); // Normaliza el vector
    vector = VectorDivision(vector, desiredLength); // Escala el vector a la longitud deseada
}

// Dibuja el siguiente segmento de la pir�mide
void DrawNextSegment(Corners cornersFloor, Vector3 vectorAMini, Vector3 vectorBMini, Vector3 vectorC)
{
    // Si el tama�o del escal�n es suficientemente peque�o, detiene la recursi�n
    if (Vector3Magnitude(vectorAMini) < 0.2f) return;

    Corners cornersCeiling = GetNextFloorCorners(cornersFloor, vectorAMini, vectorBMini, vectorC); // Calcula las esquinas del techo
    DrawSurface(cornersFloor); // Dibuja la superficie del piso
    DrawWall(cornersFloor, cornersCeiling); // Dibuja las paredes entre el piso y el techo
    DrawNextSegment(cornersCeiling, vectorAMini, vectorBMini, vectorC); // Llamada recursiva para el siguiente escal�n
}

// Dibuja una superficie plana
void DrawSurface(Corners corners)
{
    DrawLine(corners.c1, corners.c2, RED);
    DrawLine(corners.c1, corners.c3, RED);
    DrawLine(corners.c2, corners.c4, RED);
    DrawLine(corners.c3, corners.c4, RED);
}

// Dibuja las paredes entre el piso y el techo
void DrawWall(Corners cornersFloor, Corners cornersCeiling)
{
    DrawLine(cornersFloor.c1, cornersCeiling.c1, GREEN);
    DrawLine(cornersFloor.c2, cornersCeiling.c2, GREEN);
    DrawLine(cornersFloor.c3, cornersCeiling.c3, GREEN);
    DrawLine(cornersFloor.c4, cornersCeiling.c4, GREEN);
    perimeter += 4 * Vector3Magnitude(AddVectors(cornersCeiling.c4, Negative(cornersFloor.c4))); // Suma al per�metro la longitud de las paredes
    area += perimeter; // El �rea se calcula sumando el per�metro de las paredes
}

// Dibuja una l�nea 3D entre dos puntos
void DrawLine(Vector3 startPos, Vector3 endPos, Color color)
{
    DrawLine3D(startPos, endPos, color);
}

// Calcula las esquinas del siguiente piso de la pir�mide
Corners GetNextFloorCorners(Corners previousFloorCorners, Vector3 vectorAMini, Vector3 vectorBMini, Vector3 vectorC)
{
    Corners corners;
    corners.c1 = AddVectors(previousFloorCorners.c1, vectorAMini, vectorBMini, vectorC);
    corners.c2 = AddVectors(previousFloorCorners.c2, Negative(vectorAMini), vectorBMini, vectorC);
    corners.c3 = AddVectors(previousFloorCorners.c3, vectorAMini, Negative(vectorBMini), vectorC);
    corners.c4 = AddVectors(previousFloorCorners.c4, Negative(vectorAMini), Negative(vectorBMini), vectorC);
    return corners;
}

// Redondea un n�mero flotante a un n�mero espec�fico de decimales
float RoundN(float number, int numberOfDecimals)
{
    return round(number * pow(10, numberOfDecimals)) / pow(10, numberOfDecimals);
}
