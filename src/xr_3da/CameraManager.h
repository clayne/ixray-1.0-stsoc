#pragma once

#include "CameraDefs.h"

struct ENGINE_API SPPInfo {
	struct SColor{
		float r, g, b;
		SColor					(){}
		SColor					(float _r, float _g, float _b):r(_r),g(_g),b(_b){}
		IC operator u32()										{
			int		_r	= clampr	(iFloor(r*255.f+.5f),0,255);
			int		_g	= clampr	(iFloor(g*255.f+.5f),0,255);
			int		_b	= clampr	(iFloor(b*255.f+.5f),0,255);
			return color_rgba		(_r,_g,_b,0);
		}
		IC SColor& operator +=	(const SColor &ppi)				{
			r += ppi.r; g += ppi.g; b += ppi.b; 
			return *this;
		}
		IC SColor& operator -=	(const SColor &ppi)				{
			r -= ppi.r; g -= ppi.g; b -= ppi.b; 
			return *this;
		}
		IC SColor& set			(float _r, float _g, float _b)	{
			r=_r;g=_g;b=_b;
			return *this;
		}
	};
	float		blur, gray;
	struct SDuality { 
		float h, v; 
		SDuality				(){}
		SDuality				(float _h, float _v):h(_h),v(_v){}
		IC SDuality& set		(float _h, float _v)			{
			h=_h;v=_v;
			return *this;
		}
	} duality;
	struct SNoise	{
		float		intensity, grain;
		float		fps;
		SNoise					(){}
		SNoise					(float _i, float _g, float _f):intensity(_i),grain(_g),fps(_f){}
		IC SNoise& set			(float _i, float _g, float _f){
			intensity=_i;grain=_g;fps=_f;
			return *this;
		}
	} noise;

	SColor		color_base;
	SColor		color_gray;
	SColor		color_add;
	float		cm_influence;
	float		cm_interpolate;
	shared_str			cm_tex1;
	shared_str			cm_tex2;

	SPPInfo& add		(const SPPInfo &ppi);
	SPPInfo& sub		(const SPPInfo &ppi);
	void normalize		();
	SPPInfo				();
	SPPInfo&	lerp	(const SPPInfo& def, const SPPInfo& to, float factor);
	void		validate(LPCSTR str);
};

using EffectorCamVec = xr_vector<CEffectorCam*>;
using EffectorCamIt = EffectorCamVec::iterator;

using EffectorPPVec = xr_vector<CEffectorPP*>;
using EffectorPPIt = EffectorPPVec::iterator;

class ENGINE_API CCameraManager
{
	Fvector					vPosition;
	Fvector					vDirection;
	Fvector					vNormal;
	Fvector					vRight;

	EffectorCamVec			m_EffectorsCam;
	EffectorCamVec			m_EffectorsCam_added_deffered;
	EffectorCamVec			m_EffectorsCam_removed_deffered;
	EffectorPPVec			m_EffectorsPP;

	float					fFov;
	float					fFar;
	float					fAspect;
	bool					m_bAutoApply;
	SPPInfo					pp_affected;
	void					UpdateDeffered();
public:
#ifdef DEBUG	
	u32						dbg_upd_frame;
#endif

	void					Dump					(void);
	CEffectorCam*			AddCamEffector			(CEffectorCam*			ef);
	CEffectorCam*			GetCamEffector			(ECamEffectorType		type);
	void					RemoveCamEffector		(ECamEffectorType		type);

	CEffectorPP*			GetPPEffector			(EEffectorPPType	type);
	CEffectorPP*			AddPPEffector			(CEffectorPP*		ef);
	void					RemovePPEffector		(EEffectorPPType	type);

	IC Fvector				Pos					()	const { return vPosition;	}
	IC Fvector				Dir					()	const { return vDirection;}
	IC Fvector				Up					()	const { return vNormal;	}
	IC Fvector				Right				()	const { return vRight;	}
	
	IC void					camera_Matrix		(Fmatrix& M){M.set(vRight,vNormal,vDirection,vPosition);}
	void					Update				(const Fvector& P, const Fvector& D, const Fvector& N, float fFOV_Dest, float fASPECT_Dest, float fFAR_Dest, u32 flags=0);
	void					Update				(const CCameraBase* C);
	void					ApplyDevice			(float _viewport_near);
	static void				ResetPP				();

							CCameraManager		(bool bApplyOnUpdate);
							~CCameraManager		();
};
ENGINE_API extern SPPInfo					pp_identity;
ENGINE_API extern SPPInfo					pp_zero;

ENGINE_API extern float psCamInert;
ENGINE_API extern float psCamSlideInert;
