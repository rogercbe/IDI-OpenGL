#version 330 core
in vec4 Color;
out vec4 FragColor;

void main()
{
  FragColor = vec4(Color);
  /*
  if(int(gl_FragCoord.y) % 25 > 0 && int(gl_FragCoord.y) % 25 < 10) discard;
  
  //blau
  if (gl_FragCoord.x > 400 && gl_FragCoord.y > 400)
    FragColor = vec4(0.,0.,1.,1.);
  //vermell
  else if (gl_FragCoord.x < 400 && gl_FragCoord.y > 400)
    FragColor = vec4(1.,0.,0.,1.);
  //verd
  else if (gl_FragCoord.x > 400 && gl_FragCoord.y < 400)
    FragColor = vec4(0.,1.,0.,1.);
  //groc
  else
    FragColor = vec4(1.,1.,0.,1.);
  */
}