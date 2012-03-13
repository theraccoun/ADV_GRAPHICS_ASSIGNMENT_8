//  Per-pixel Blinn-Phong lighting
//  Fragment shader

varying vec3 Half;
varying vec3 Light;
varying vec3 Normal;
varying vec4 Ambient;
uniform sampler2D tex;

vec4 phong()
{
   //  N is the object normal
   vec3 N = normalize(Normal);
   //  L is the light vector
   vec3 L = normalize(Light);

   //  Emission and ambient color
   vec4 color = Ambient;

   //  Diffuse light is cosine of light and normal vectors
   float Id = dot(L,N);
   if (Id>0.0)
   {
      //  Add diffuse
      color += Id*gl_FrontLightProduct[0].diffuse;
      //  V is the view vector (eye vector)
      vec3 H = normalize(Half);
      //  Specular is cosine of reflected and view vectors
      float Is = dot(H,L);
      if (Is>0.0) color += pow(Is,gl_FrontMaterial.shininess)*gl_FrontLightProduct[0].specular;
   }

   //  Return sum of color components
   return color;
}

void main()
{
   gl_FragColor = phong() * texture2D(tex,gl_TexCoord[0].xy);
}
