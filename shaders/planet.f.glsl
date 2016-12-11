/*
 *   Fragment shader that implements Phong Shading with texturing.
 *
 *   Jeffrey Paone, Fall 2015
 *       for CSCI 441, Computer Graphics, Colorado School of Mines
 */

#version 120

varying vec3 normalVec;
varying vec3 lightVec;
varying vec3 halfwayVec;
varying float attenuation;

uniform sampler2D tex;

void main(void) 
{

    /*****************************************/
    /********* Texture Calculations  *********/
    /*****************************************/

    // get the texel corresponding to the interpolated texture coordinates
    vec4 texel = texture2D( tex, gl_TexCoord[0].st );

    /*****************************************/
    /********* Lighting Calculations *********/
    /*****************************************/

    // compute the ambient component of lighting
    vec4 ambientComponent;
    if( gl_FrontFacing )
        ambientComponent = gl_FrontMaterial.ambient * gl_LightSource[0].ambient;
    else
        ambientComponent = gl_BackMaterial.ambient * gl_LightSource[0].ambient;

    // compute the diffuse component of lighting
    vec4 diffuseComponent;
    if( gl_FrontFacing )
        diffuseComponent = gl_FrontMaterial.diffuse * gl_LightSource[0].diffuse * max( 0.0, dot( normalVec, lightVec ) ) * attenuation;
    else
        diffuseComponent = gl_BackMaterial.diffuse * gl_LightSource[0].diffuse * max( 0.0, dot( normalVec, lightVec ) ) * attenuation;

    // compute the specular component of lighting
    vec4 specularComponent;
    if( gl_FrontFacing )
        specularComponent = gl_FrontMaterial.specular * gl_LightSource[0].specular * pow( max( 0.0, dot( normalVec, halfwayVec ) ), gl_FrontMaterial.shininess ) * attenuation;
    else
        specularComponent = gl_BackMaterial.specular * gl_LightSource[0].specular * pow( max( 0.0, dot( normalVec, halfwayVec ) ), gl_FrontMaterial.shininess ) * attenuation;

    // compute the emission component of lighting
    vec4 emissionComponent;
    if( gl_FrontFacing )
        emissionComponent = gl_FrontMaterial.emission;
    else
        emissionComponent = gl_BackMaterial.emission;

    // set the lighting color
    vec4 theColor = ambientComponent + diffuseComponent + specularComponent + emissionComponent;

    /*****************************************/
    /******* Final Color Calculations ********/
    /*****************************************/

    // break out the texture colors and lighting colors
    vec3 ct = texel.rgb;
    vec3 cf = theColor.rgb;

    float at = texel.a;
    float af = theColor.a;

    // compute the modulated resulting color
    vec4 originalColor = vec4( ct * cf, at * af );
    float intensity;
    vec4 color;
    vec3 n = normalize(normalVec);
    intensity = dot(vec3(gl_LightSource[0].position), n);

    if (intensity > 0.95)
        color = vec4(1.0*originalColor.r,1*originalColor.g,1*originalColor.b,1.0);
    else if (intensity > 0.85)
        color = vec4(0.8*originalColor.r,0.8*originalColor.g,0.8*originalColor.b,1.0);
    else if (intensity > 0.75)
        color = vec4(0.7*originalColor.r,0.7*originalColor.g,0.7*originalColor.b,1.0);
    else if (intensity > 0.65)
        color = vec4(0.6*originalColor.r,0.6*originalColor.g,0.6*originalColor.b,1.0);
    else if (intensity > 0.55)
        color = vec4(0.5*originalColor.r,0.5*originalColor.g,0.5*originalColor.b,1.0);
    else if (intensity > 0.45)
        color = vec4(0.4*originalColor.r,0.4*originalColor.g,0.4*originalColor.b,1.0);
    else if (intensity > 0.35)
        color = vec4(0.3*originalColor.r,0.3*originalColor.g,0.3*originalColor.b,1.0);
    else if (intensity > 0.25)
        color = vec4(0.2*originalColor.r,0.2*originalColor.g,0.2*originalColor.b,1.0);
    else if (intensity > 0.15)
        color = vec4(0.1*originalColor.r,0.1*originalColor.g,0.1*originalColor.b,1.0);
    else
        color = vec4(0*originalColor.r,0*originalColor.g,0*originalColor.b,1.0);
    gl_FragColor = color;
}
