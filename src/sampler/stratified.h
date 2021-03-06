/*
   FileName:      stratified.h

   Created Time:  2011-08-11 13:25:56

   Auther:        Cao Jiayin

   Email:         soraytrace@hotmail.com

   Location:      China, Shanghai

   Description:   SORT is short for Simple Open-source Ray Tracing. Anyone could checkout the source code from
                'sourceforge', https://soraytrace.svn.sourceforge.net/svnroot/soraytrace. And anyone is free to
                modify or publish the source code. It's cross platform. You could compile the source code in 
                linux and windows , g++ or visual studio 2008 is required.
*/

#ifndef	SORT_STRATIFIED
#define	SORT_STRATIFIED

#include "sampler.h"

/////////////////////////////////////////////////////////////////////////////////
// definitation of the sampler
class StratifiedSampler : public Sampler
{
// public method
public:
	DEFINE_CREATOR( StratifiedSampler , "stratified" );

	// default constructor
	StratifiedSampler(){}
	// destructor
	~StratifiedSampler(){}

	// round the size for sampler
	// para 'size' : number of samples to be generated
	// result      : the modified number
	// note : samples like jittered sampling needs to overwrite the method
	virtual unsigned RoundSize( unsigned size ) const;

	// generate sample in one dimension
	// para 'sample' : the memory to save the sampled data
	// para 'num'    : the number of samples to be generated
	virtual void Generate1D( float* sample , unsigned num ) const;

	// generate sample in two dimension
	// para 'sample' : the memory to save the sampled data
	// para 'num'    : the number of samples to be generated
	virtual void Generate2D( float* sample , unsigned num ) const;
};

#endif