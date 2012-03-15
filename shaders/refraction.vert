varying vec3 RF;

void main(void)
{
    // Normal in Eye Space
    vec3 vEyeNormal = gl_NormalMatrix * gl_Normal;

    // Vertex position in Eye Space
    vec4 vVert4 = gl_ModelViewMatrix * gl_Vertex;
    vec3 vEyeVertex = normalize(vVert4.xyz / vVert4.w);

    vec4 vCoords = vec4(refract(vEyeVertex, vEyeNormal, 1.5), 0.0);

    // Rotate by flipped camera
    vCoords = gl_ModelViewMatrixInverse * vCoords;
    RF.xyz = normalize(vCoords.xyz);

    // Don't forget to transform the geometry!
    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
}

