/*
   FileName:      direct.cpp

   Created Time:  2011-08-12 12:52:41

   Auther:        Cao Jiayin

   Email:         soraytrace@hotmail.com

   Location:      China, Shanghai

   Description:   SORT is short for Simple Open-source Ray Tracing. Anyone could checkout the source code from
                'sourceforge', https://soraytrace.svn.sourceforge.net/svnroot/soraytrace. And anyone is free to
                modify or publish the source code. It's cross platform. You could compile the source code in 
                linux and windows , g++ or visual studio 2008 is required.
*/

// include header file
#include "direct.h"
#include "integratormethod.h"
#include "geometry/intersection.h"
#include "geometry/scene.h"
#include "bsdf/bsdf.h"
#include "light/light.h"

// radiance along a specific ray direction
Spectrum DirectLight::Li( const Scene& scene , const Ray& r ) const
{
	if( r.m_Depth > 6 )
		return 0.0f;

	// get the intersection between the ray and the scene
	Intersection ip;
	if( false == scene.GetIntersect( r , &ip ) )
		return scene.EvaluateSky( r );

	Spectrum t;

	// get bsdf
	Bsdf* bsdf = ip.primitive->GetMaterial()->GetBsdf( &ip );

	// lights
	Visibility visibility(scene);
	const vector<Light*>& lights = scene.GetLights();
	vector<Light*>::const_iterator it = lights.begin();
	while( it != lights.end() )
	{
		Vector	lightDir;
		float	pdf;
		Spectrum ld = (*it)->sample_f( ip , lightDir , &pdf , visibility );
		if( ld.IsBlack() )
		{
			it++;
			continue;
		}
		Spectrum f = bsdf->f( -r.m_Dir , lightDir );
		if( f.IsBlack() )
		{
			it++;
			continue;
		}
		bool visible = visibility.IsVisible();
		if( visible )
			t += (ld * f * SatDot( lightDir , ip.normal ) / pdf);
		it++;
	}

	// add reflection
	if( bsdf->NumComponents( BXDF_REFLECTION ) > 0 )
		t += SpecularReflection( scene , r , &ip , bsdf , this );
	if( bsdf->NumComponents( BXDF_TRANSMISSION ) > 0 )
		t += SpecularRefraction( scene , r , &ip , bsdf , this );

	return t;
}

// output log information
void DirectLight::OutputLog() const
{
	LOG_HEADER( "Integrator" );
	LOG<<"Integrator algorithm : Direct Light Integrator."<<ENDL;
	LOG<<"It supports direct lighting , specular reflection and specular refraction."<<ENDL;
	LOG<<"Soft shadow and area light are also supported in the algorithm."<<ENDL;
	LOG<<"Indirect lighting , like color bleeding , caustics , is not supported."<<ENDL<<ENDL;
}
