#include "Shader.h"
#include "Matrix3.h"
#include "MathHelpers.h"

Shader::Shader()
{
    //projectionMat = Matrix4::Perspective(90, (float)width / (float)height, 0.01f, 1000);
    modelViewMat = Matrix4::GetIdentity();
}

Shader::~Shader()
{

}

void Shader::viewport(int x, int y, int w, int h)
{
    viewPort = Matrix4::GetIdentity();
    viewPort.m03 = x+w/2.f;
    viewPort.m13 = y+h/2.f;
    viewPort.m23 = 1.f;
    viewPort.m00 = w/2.f;
    viewPort.m11 = h/2.f;
    viewPort.m22 = 0;
}

Matrix4 CreatePerspective(float fieldOfViewInRadians, float aspect, float near, float far)
{
    float f = std::tan(PI * 0.5 - 0.5 * fieldOfViewInRadians);
    float rangeInv = 1.0 / (near - far);

    return Matrix4(
      f / aspect, 0, 0, 0,
      0, f, 0, 0,
      0, 0, (near + far) * rangeInv, -1,
      0, 0, near * far * rangeInv * 2, 0
    );
}

void Shader::projection(float coeff)
{
    // Equivalent code:
    projectionMat = Matrix4::GetIdentity();
    projectionMat.m32 = coeff;

    //projectionMat = CreatePerspective( 90, (float)width / (float)height, 0.1f, 1000);

    //projectionMat = ToMatrix4(Projection);
    //projectionMat = Matrix4::Perspective(90, (float)width / (float)height, 0.1f, 1000);
    //projectionMat = projectionMat.Transpose();

    //Projection = ToMatrix(projectionMat);
    //Projection = Projection.transpose();
}

void Shader::lookat(Vector3 eye, Vector3 center, Vector3 up)
{
    modelview = Matrix4::LookAt(eye, center, up);
}

Vector4 Shader::TransformNDCToScreenSpace(Vector4 coord)
{
	Vector4 normalisedDeviceCoord = coord;

	normalisedDeviceCoord.x = (coord.x * width) + (width / 2);
	normalisedDeviceCoord.y = ((coord.y * height) + (height / 2));

	Vector4 screenPoint = normalisedDeviceCoord;

	// HACK Clip within screen:
	//screenPoint = MathHelpers.ClampVector(screenPoint.xyz, 0, Camera.Rasteriser.RenderingSize); // Clip against screen bounds

	return screenPoint; // technically we only need a 2 float vector though we will use z later for depth testing
}

Vector4 Shader::PerspectiveDivide(Vector4 vertexClip)
{
	Vector4 vertexNdc(vertexClip.x / vertexClip.w,
                      vertexClip.y / vertexClip.w,
                      vertexClip.z / vertexClip.w,
                      vertexClip.w);
	return vertexNdc;
}

Vector4 Shader::ScreenSpace(Vector4 clipCoord)
{
    Vector4 screenPoint;
    Vector4 normalisedDeviceCoord;

	float z = clipCoord.z;
	// Transform to Normalised Device Coordinates.
	// PERSPECTIVE DIVIDE
	normalisedDeviceCoord = PerspectiveDivide(clipCoord);

    //std::cout << "x: " << normalisedDeviceCoord.x << " y: " << normalisedDeviceCoord.y << " z: " << normalisedDeviceCoord.z << " w: " << normalisedDeviceCoord.w << std::endl;
	// NDC-SPACE
	screenPoint = TransformNDCToScreenSpace(normalisedDeviceCoord);
	screenPoint.w = z;

	return screenPoint;
}

Vector4 Shader::ProjectVector(Vector3 worldPosition, Matrix4 modelViewProjection)
{
	Vector4 normalisedDeviceCoord;
	Vector4 screenPoint;
    //modelViewProjection = Matrix4::GetIdentity();
	Vector4 world(worldPosition.x, worldPosition.y, worldPosition.z, 1.0f);
	Vector4 clipCoord = Matrix4::TransformVector(modelViewProjection, world);
    //clipCoord.w = 1.0;
	// Preserve the value of what z was before the perspective divide to use later for depth testing
	float z = clipCoord.z;
    //std::cout << "x: " << clipCoord.x << " y: " << clipCoord.y << " z: " << clipCoord.z << " w: " << clipCoord.w << std::endl;
	// Transform to Normalised Device Coordinates.
	// PERSPECTIVE DIVIDE
	normalisedDeviceCoord = PerspectiveDivide(clipCoord);

    //std::cout << "x: " << normalisedDeviceCoord.x << " y: " << normalisedDeviceCoord.y << " z: " << normalisedDeviceCoord.z << " w: " << normalisedDeviceCoord.w << std::endl;
	// NDC-SPACE
	screenPoint = TransformNDCToScreenSpace(normalisedDeviceCoord);
	screenPoint.w = z;

    //std::cout << "x: " << screenPoint.x << " y: " << screenPoint.y << " z: " << screenPoint.z << " w: " << screenPoint.w << std::endl;
	// SCREEN-SPACE
	return screenPoint;
}

Vector4 Shader::TransformWorldToScreenspace(Vector3 worldPosition, Transform cameraTransform, Transform objectTransform, Vector4& clipCoord)
{
	Vector4 result;
    //modelViewMat = modelview;
    //modelViewMat = Matrix4::Multiply(objectTransform.localMatrix, modelview);
	modelViewMat = Matrix4::Multiply(cameraTransform.localMatrix, modelview);
	//modelViewMat = cameraTransform.localMatrix;
	modelViewMat = Matrix4::Multiply(objectTransform.localMatrix, modelViewMat);
	MVP = Matrix4::Multiply(projectionMat, modelViewMat);
    //MVP = projectionMat;

	Vector4 world = Vector4(worldPosition, 1.0);
	clipCoord = Matrix4::TransformVector(MVP, world);

	result = ProjectVector(worldPosition, MVP);

	return result;
}

Vector4 Shader::vertex(Model* model, int iface, int nthvert, Triangle& t, Transform cameraTransform, Transform objectTransform)
{
    varying_uv.set_col(nthvert, model->uv(iface, nthvert));

    Matrix4 MVP = (projectionMat * modelview);
    Vector3 projNormal = (MVP.Inverse().Transpose()
        * Vector4(model->normal(iface, nthvert), 0.f)
        ).xyz();
    varying_nrm.set_col(nthvert, projNormal);

    Vector4 gl_Vertex;
    Vector4 clipCoord;

    gl_Vertex = TransformWorldToScreenspace(model->vert(iface, nthvert), cameraTransform, objectTransform, clipCoord);

    varying_tri.set_col(nthvert, clipCoord);

    Vector4 ndcCoord = PerspectiveDivide(clipCoord);
    ndc_tri.set_col(nthvert, Vector3(ndcCoord.x, ndcCoord.y, ndcCoord.z));

    return gl_Vertex;
}

bool Shader::fragment(Vector3 eyeWorldPos, Model* model, Vector3 bar, ColourRGBA &colour)
{
    Vector3 FragPosition = (varying_tri * bar).xyz();
    Vector3 bn = (varying_nrm*bar).Normalise();
    Vector2 uv = varying_uv*bar;

    Matrix3 A;
    A.SetColumn0(ToVec3(ndc_tri.col(1) - ndc_tri.col(0)));
    A.SetColumn1(ToVec3(ndc_tri.col(2) - ndc_tri.col(0)));
    A.SetColumn2(bn);

    Matrix3 AI = A.Inverse();

    Vector3 i = (AI * Vector3(varying_uv[0][1] - varying_uv[0][0], varying_uv[0][2] - varying_uv[0][0], 0));
    Vector3 j = (AI * Vector3(varying_uv[1][1] - varying_uv[1][0], varying_uv[1][2] - varying_uv[1][0], 0));

    Matrix3 B;
    B.SetColumn0(i.Normalise());
    B.SetColumn1(j.Normalise());
    B.SetColumn2(bn);

    Vector3 n = (B* model->normal(uv)).Normalise();

    float diff = std::max(0.f, Vector3::Dot(n, light_dir));
    float spec = model->specular(uv);

    //Vector3 r = Vector3(0,0,0);
    //Vector3 v = Vector3(0,0,0);

    //colour = model->diffuse(uv) * diff * 0.3f;

    //float specularValue = (spec * std::pow(std::max(Vector3::Dot(r, v), 0.0f), shininess));

    //colour = model->diffuse(uv) * diff;
    ColourRGBA diffuseCol = model->diffuse(uv);
    ColourRGBA glowCol = model->emissiveness(uv);
    Vector3 emissive = Vector3(glowCol.r, glowCol.g, glowCol.b);
    Vector3 specular = Vector3(1, 1, 1);
    Vector3 ambient = Vector3(1, 1, 1);
    Vector3 diffuse = Vector3(diffuseCol.r, diffuseCol.g, diffuseCol.b);
    float light_ambient = 0.0f;
    float diffuseValue = diff;
    float emissiveness = 1.0f;
    float specularValue = spec;
    float shininess = 0.004f;


 ///*
    Vector3 gDirectionalLight_Color = specular;
    Vector3 gDirectionalLight_Direction = light_dir;
    Vector3 Normal = n;
    Vector3 VertexToEye = (eyeWorldPos - FragPosition).Normalise();
    Vector3 LightReflect = Vector3::Reflect(gDirectionalLight_Direction, Normal).Normalise();
    float SpecularFactor = Vector3::Dot(VertexToEye, LightReflect);
    Vector3 SpecularColor;
    if (SpecularFactor > 0)
    {
        float gMatSpecularIntensity = 0.001f;
        float gSpecularPower = shininess;
        SpecularFactor = std::pow(std::max(SpecularFactor, 0.0f), gSpecularPower);

        SpecularColor = gDirectionalLight_Color;

        specularValue = (spec * gMatSpecularIntensity * SpecularFactor);
    }
    // */

    Vector3 fragColour = (emissive * emissiveness) + (ambient * light_ambient)
      + (diffuse * diffuseValue) + (SpecularColor * specularValue);


    /*
    // Reinhard Tone Mapping
    float gamma = 2.2f;
    Vector3 hdrColor = fragColour;
    Vector3 mapped = hdrColor / (hdrColor + Vector3(1.0f, 1.0f, 1.0f));
    Vector3 gammaFactor = Vector3(1.0f / gamma, 1.0f / gamma, 1.0f / gamma);
    mapped = Vector3(std::pow(mapped.x, gammaFactor.x),
                     std::pow(mapped.y, gammaFactor.y),
                     std::pow(mapped.z, gammaFactor.z)); // gamma correction
    fragColour = mapped; // */



    colour = ColourRGBA(1.0f, fragColour.x, fragColour.y, fragColour.z);

    // colour = Colour::red;

    return false;
}
