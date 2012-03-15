uniform samplerCube cubeMap;
varying vec3 vTexCoord;

void main (void)
{
   	gl_FragColor = texture(cubeMap, vTexCoord) * 1;
}
