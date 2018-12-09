#version 400

layout (location = 0) in vec4 vPosition;
layout (location = 1) in vec4 vColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec4 v2fcolor;

void main() {
    vec4 VMvPosition = view * model * vPosition;

    gl_Position = projection * VMvPosition;

    //float gray = (gl_Position.z + 1) * 0.5;

    //v2fcolor = vec4 (gl_Position.x*0.5, gl_Position.y*0.5, gl_Position.z*0.5, 1);
    v2fcolor = vec4 (0, (abs(vPosition.x) + abs(vPosition.z))*0.25, 0, 1);
    //v2fcolor = vColor;
}
