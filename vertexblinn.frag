varying vec3 R;
uniform samplerCube texMap;

void main()
{
    gl_FragColor = textureCube(texMap, R);
}
