#pragma once

#include "../Util/Math.h"

struct SpawnData{
	Vector3 forward;
	float  colorSamplingV;
	Vector4 position;
	float  initialSpeedFactor;
	float  reductionRate;
	float  speedMag;
	float  lengthMag;
};

struct InstanceData {
	Vector4 position;
	Vector3 forward;
	float  initialSpeedFactor;
	float  reductionRate;
	float  speedMag;
	float  lengthMag;
	float  spawnTime;
	Vector3 reserved;
	float  colorSamplingV;
};

struct InstanceDrawData {
	Vector4 position[2];
	Vector2 colorSampleCoord;
	Vector2 pad;
};