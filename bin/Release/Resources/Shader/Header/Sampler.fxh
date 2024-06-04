#ifndef SAMPLER_FXH
#define SAMPLER_FXH

SamplerState anisotropicSampler : register(s0); //D3D11_FILTER_ANISOTROPIC
SamplerState pointSampler : register(s1); //D3D11_FILTER_MIN_MAG_MIP_POINT

#endif