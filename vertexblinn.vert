void main()
{
   //  Texture coordinate
   gl_TexCoord[0] = gl_MultiTexCoord0;

   //  Return fixed transform coordinates for this vertex
   gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
}
