
//’á‘¬
matrix Invert(matrix mat)
{
    matrix ret;
    float det = determinant(mat);
    ret._11 = mat._22 * mat._33 * mat._44 + mat._23 * mat._34 * mat._42 + mat._24 * mat._32 * mat._43 - mat._22 * mat._34 * mat._43 - mat._23 * mat._32 * mat._44 - mat._24 * mat._33 * mat._42;
    ret._12 = mat._12 * mat._34 * mat._43 + mat._13 * mat._32 * mat._44 + mat._14 * mat._33 * mat._42 - mat._12 * mat._33 * mat._44 - mat._13 * mat._34 * mat._42 - mat._14 * mat._32 * mat._43;
    ret._13 = mat._12 * mat._23 * mat._44 + mat._13 * mat._24 * mat._42 + mat._14 * mat._22 * mat._43 - mat._12 * mat._24 * mat._43 - mat._13 * mat._22 * mat._44 - mat._14 * mat._23 * mat._42;
    ret._14 = mat._12 * mat._24 * mat._33 + mat._13 * mat._22 * mat._34 + mat._14 * mat._23 * mat._32 - mat._12 * mat._23 * mat._34 - mat._13 * mat._24 * mat._32 - mat._14 * mat._22 * mat._33;

    ret._21 = mat._21 * mat._34 * mat._43 + mat._23 * mat._31 * mat._44 + mat._24 * mat._33 * mat._41 - mat._21 * mat._33 * mat._44 - mat._23 * mat._34 * mat._41 - mat._24 * mat._31 * mat._43;
    ret._22 = mat._11 * mat._33 * mat._44 + mat._13 * mat._34 * mat._41 + mat._14 * mat._31 * mat._43 - mat._11 * mat._34 * mat._43 - mat._13 * mat._31 * mat._44 - mat._14 * mat._33 * mat._41;
    ret._23 = mat._11 * mat._24 * mat._43 + mat._13 * mat._21 * mat._44 + mat._14 * mat._23 * mat._41 - mat._11 * mat._23 * mat._44 - mat._13 * mat._24 * mat._41 - mat._14 * mat._21 * mat._43;
    ret._24 = mat._11 * mat._23 * mat._34 + mat._13 * mat._24 * mat._31 + mat._14 * mat._21 * mat._33 - mat._11 * mat._24 * mat._33 - mat._13 * mat._21 * mat._34 - mat._14 * mat._23 * mat._31;

    ret._31 = mat._21 * mat._32 * mat._44 + mat._22 * mat._34 * mat._41 + mat._24 * mat._31 * mat._42 - mat._21 * mat._34 * mat._42 - mat._22 * mat._31 * mat._44 - mat._24 * mat._32 * mat._41;
    ret._32 = mat._11 * mat._34 * mat._42 + mat._12 * mat._31 * mat._44 + mat._14 * mat._32 * mat._41 - mat._11 * mat._32 * mat._44 - mat._12 * mat._34 * mat._41 - mat._14 * mat._31 * mat._42;
    ret._33 = mat._11 * mat._22 * mat._44 + mat._12 * mat._24 * mat._41 + mat._14 * mat._21 * mat._42 - mat._11 * mat._24 * mat._42 - mat._12 * mat._21 * mat._44 - mat._14 * mat._22 * mat._41;
    ret._34 = mat._11 * mat._24 * mat._32 + mat._12 * mat._21 * mat._34 + mat._14 * mat._22 * mat._31 - mat._11 * mat._22 * mat._34 - mat._12 * mat._24 * mat._31 - mat._14 * mat._21 * mat._32;

    ret._41 = mat._21 * mat._33 * mat._42 + mat._22 * mat._31 * mat._43 + mat._23 * mat._32 * mat._41 - mat._21 * mat._32 * mat._43 - mat._22 * mat._33 * mat._41 - mat._23 * mat._31 * mat._42;
    ret._42 = mat._11 * mat._32 * mat._43 + mat._12 * mat._33 * mat._41 + mat._13 * mat._31 * mat._42 - mat._11 * mat._33 * mat._42 - mat._12 * mat._31 * mat._43 - mat._13 * mat._32 * mat._41;
    ret._43 = mat._11 * mat._23 * mat._42 + mat._12 * mat._21 * mat._43 + mat._13 * mat._22 * mat._41 - mat._11 * mat._22 * mat._43 - mat._12 * mat._23 * mat._41 - mat._13 * mat._21 * mat._42;
    ret._44 = mat._11 * mat._22 * mat._33 + mat._12 * mat._23 * mat._31 + mat._13 * mat._21 * mat._32 - mat._11 * mat._23 * mat._32 - mat._12 * mat._21 * mat._33 - mat._13 * mat._22 * mat._31;

    ret._11 /= det;
    ret._12 /= det;
    ret._13 /= det;
    ret._14 /= det;

    ret._21 /= det;
    ret._22 /= det;
    ret._23 /= det;
    ret._24 /= det;

    ret._31 /= det;
    ret._32 /= det;
    ret._33 /= det;
    ret._34 /= det;

    ret._41 /= det;
    ret._42 /= det;
    ret._43 /= det;
    ret._44 /= det;
    return ret;
}

matrix CreateLookAt(float3 position, float3 target, float3 upward)
{
    float3 zaxis = target - position;
    normalize(zaxis);

    float3 xaxis = cross(upward, zaxis);
    normalize(xaxis);

    float3 yaxis = cross(zaxis, xaxis);
    normalize(yaxis);
    return Matrix(
		xaxis.x, yaxis.x, zaxis.x, 0.0f,
		xaxis.y, yaxis.y, zaxis.y, 0.0f,
		xaxis.z, yaxis.z, zaxis.z, 0.0f,
		-dot(xaxis, position),
		-dot(yaxis, position),
		-dot(zaxis, position),
		1.0f);
}

float3 SafeCross(float3 a, float3 b)
{

}

matrix Transpose(matrix mat)
{
    return transpose(mat);
;
}

float Determinant(matrix mat)
{
    float det = determinant(mat);

    return det;
}

float Lambert(float3 normal, float3 lightDir)
{
    return max(dot(normalize(normal), normalize(lightDir)), 0.0f);
}

float HalfLambert(float3 normal, float3 lightDir)
{
    return pow(max(0.0f, dot(normalize(normal), normalize(lightDir))) * 0.5f + 0.5f, 2.0f);
}

float NormalizeLambert(float3 normal, float3 lightDir)
{
    return dot(normalize(normal), normalize(lightDir)) * rcp(3.141592f);
}
