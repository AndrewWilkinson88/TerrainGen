#version 130

// Make geometry solid
in vec4 color;

// Output fragment color
out vec4 vFragColor;

void main(void)
{ 
   vFragColor = color;
}