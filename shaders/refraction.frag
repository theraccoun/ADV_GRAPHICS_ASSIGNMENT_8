uniform samplerCube cubeMap;
varying vec3 RF;

void main (void)
{
        gl_FragColor = texture(cubeMap, RF);
}
