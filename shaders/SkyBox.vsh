varying vec3 vTexCoord;

void main(void)
{
   	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
   	vTexCoord = normalize(gl_Vertex.xyz);
}
