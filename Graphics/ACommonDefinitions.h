#pragma once

#include <cmath>

namespace  
{
	#define BITMASK_SWITCH(x)  for (uint64_t bit = 1; x >= bit; bit *= 2) if (x & bit) switch (bit)

	#define STRINGIFY(s) #s 
	#define AHRASSERT( p ) { assert(SUCCEEDED(hr)); }
	#define ADXASSERT( p ) { if(!p) MessageBoxA( nullptr, STRINGIFY(p), "Err", MB_OK); }

	#define SAFE_DELETE(a) if( (a) != nullptr ) delete (a); (a) = nullptr;
	#define DXASSERT(a) ADXASSERT(SUCCEEDED(a))

	#define BIG_EPSILON				0.00001f    
	#define SMALLL_EPSILON			0.000001f    
	#define PI						3.141592654    
	#define HALFPI					1.570796327    
	#define TWOPI					6.283185307    
	#define DEG_TO_RAD(x)			((1.74532952e-2)*x)    
	#define RAD_TO_DEG(x)			(57.29577951*x)   
	#define ROOT_2					1.414213562    
	#define SIN_45					0.707106781
	
	template <typename T>
	T Clamp(T d, T min, T max)
	{
		const T t = d < min ? min : d;

		return t > max ? max : t;
	}

	template <typename T>
	T Lerp(T min, T max, float alpha)
	{
		return min + (max - min) * alpha;
	}

	template <typename T>
	float InvLerp(T min, T max, T val)
	{
		return (val - min) / (max - min);
	}

	template <typename T>
	T Remap(T iMin, T iMax, T oMin, T oMax, T value)
	{
		float alpha = InvLerp(iMin, iMax, value);

		return Lerp(oMin, oMax, alpha);
	}

	Vector3 RLerp(Vector3 v1, Vector3 v2, float t)
	{
		v1.Normalize();
		v2.Normalize();

		float dot = v1.Dot(v2);
		
		if (dot < 0.0f) 
		{
			v2 = -v2;
			dot = -dot;
		}

		const float DOT_THRESHOLD = 0.9995f;

		if ( dot > DOT_THRESHOLD )
		{
			Vector3 result = Lerp(v1, v2, t);
			
			result.Normalize();

			return result;
		}
		 
		// Since dot is in range [0, DOT_THRESHOLD], acos is safe
		float theta_0 = acosf(dot);        // theta_0 = angle between input vectors
		float theta = theta_0 * t;          // theta = angle between v0 and result
		float sin_theta = sinf(theta);     // compute this value only once
		float sin_theta_0 = sinf(theta_0); // compute this value only once
 
		float s0 = cosf(theta) - dot * sin_theta / sin_theta_0;  // == sin(theta_0 - theta) / sin(theta_0)
		float s1 = sin_theta / sin_theta_0;

		Vector3 result = (s0 * v1) + (s1 * v2);

		result.Normalize();

		return result;
	}

	Vector3 Slerp(Vector3 v0, Vector3 v1, float t)
	{
		//; Inputs are : unit vectors v0 and v1, scalar t
		//	; v0 and v1 are linearly independent
		//
		//	Let θ0 = acos(v0 · v1)
		//	Let θ = tθ0
		//	Let v2 = Normalize(v1 - (v0 · v1)v0)
		//	return v0cosθ + v2sinθ

		v0.Normalize();
		v1.Normalize();

		float dot = v0.Dot(v1);

		float theta0 = acosf(dot);

		float theta = theta0 * t;

		Vector3 v2 = v1 - dot * v0;

		v2.Normalize();

		return v0 * cosf(theta) + v2 * sinf(theta);

	}

	Vector3 Slerp3(Vector3 v0, Vector3 v1, double t)
	{
		Quaternion q0 = Quaternion(v0);
		Quaternion q1 = Quaternion(v1);

		Quaternion q2 = Quaternion::Slerp(q0, q1, t);

		Vector3 result = Vector3(q2.x, q2.y, q2.z);
		result.Normalize();
		return result;
	}
		
	Vector3 Slerp2(Vector3 v0, Vector3 v1, double t)
	{
		v0.Normalize();
		v1.Normalize();

		// v0 and v1 should be unit length or else
		// something broken will happen.

		// Compute the cosine of the angle between the two vectors.
		double dot = v0.Dot(v1);

		const double DOT_THRESHOLD = 0.9995;

		if (dot > DOT_THRESHOLD) 
		{
			// If the inputs are too close for comfort, linearly interpolate
			// and normalize the result.

			Vector3 result = v0 + t * (v1 - v0);

			result.Normalize();

			return result;
		}

		Clamp<double>(dot, -1, 1);           // Robustness: Stay within domain of acos()

		double theta_0 = acos(dot);  // theta_0 = angle between input vectors

		double theta = theta_0 * t;    // theta = angle between v0 and result 

		Vector3 v2 = v1 - v0 * dot;

		v2.Normalize();              // { v0, v2 } is now an orthonormal basis

		return v0 * cos(theta) + v2 * sin(theta);
	}


	Vector3 Nlerp(Vector3 start, Vector3 end, float percent)
	{
		Vector3 result = Lerp(start, end, percent);

		result.Normalize();

		return result;
	}

	Vector3 PitchYawRollFromVector( Vector3 v )
	{
		Vector3 pyr;
		Vector3 up = Vector3(0.0f, 1.0f, 0.0f);

		v.Normalize(); 

		// Yaw is the bearing of the forward vector's shadow in the xz plane.
		float yaw = atan2f(v.x, v.z);

		// Pitch is the altitude of the forward vector off the xz plane, toward the down direction.
		float pitch = -asinf(v.y);

		//// Find the vector in the xz plane 90 degrees to the right of our bearing.
		//float planeRightX = sinf(yaw);
		//float planeRightZ = -cosf(yaw);

		// Roll is the rightward lean of our up vector, computed here using a dot product.
		float roll = 0.0f;// asinf(up.x * planeRightX + up.z * planeRightZ);

		//// If we're twisted upside-down, return a roll in the range +-(pi/2, pi)
		//if (up.z < 0)
		//{
		//	roll = roll > 0 ? PI - roll : -PI - roll;
		//}

		// Convert radians to degrees.
		pyr.x = RAD_TO_DEG(pitch);
		pyr.y = RAD_TO_DEG(yaw);
		pyr.z = RAD_TO_DEG( roll );

		return pyr;

	}

	enum ColorARGB : uint32_t
	{
		AliceBlue				= 0xFFF0F8FF,
		AntiqueWhite			= 0xFFFAEBD7,
		Aqua					= 0xFF00FFFF,
		Aquamarine				= 0xFF7FFFD4,
		Azure					= 0xFFF0FFFF,
		Beige					= 0xFFF5F5DC,
		Bisque					= 0xFFFFE4C4,
		Black					= 0xFF000000,
		BlanchedAlmond			= 0xFFFFEBCD,
		Blue					= 0xFF0000FF,
		BlueViolet				= 0xFF8A2BE2,
		Brown					= 0xFFA52A2A,
		BurlyWood				= 0xFFDEB887,
		CadetBlue				= 0xFF5F9EA0,
		Chartreuse				= 0xFF7FFF00,
		Chocolate				= 0xFFD2691E,
		Coral					= 0xFFFF7F50,
		CornflowerBlue			= 0xFF6495ED,
		Cornsilk				= 0xFFFFF8DC,
		Crimson					= 0xFFDC143C,
		Cyan					= 0xFF00FFFF,
		DarkBlue				= 0xFF00008B,
		DarkCyan				= 0xFF008B8B,
		DarkGoldenrod			= 0xFFB8860B,
		DarkGray				= 0xFFA9A9A9,
		DarkGreen				= 0xFF006400,
		DarkKhaki				= 0xFFBDB76B,
		DarkMagenta				= 0xFF8B008B,
		DarkOliveGreen			= 0xFF556B2F,
		DarkOrange				= 0xFFFF8C00,
		DarkOrchid				= 0xFF9932CC,
		DarkRed					= 0xFF8B0000,
		DarkSalmon				= 0xFFE9967A,
		DarkSeaGreen			= 0xFF8FBC8B,
		DarkSlateBlue			= 0xFF483D8B,
		DarkSlateGray			= 0xFF2F4F4F,
		DarkTurquoise			= 0xFF00CED1,
		DarkViolet				= 0xFF9400D3,
		DeepPink				= 0xFFFF1493,
		DeepSkyBlue				= 0xFF00BFFF,
		DimGray					= 0xFF696969,
		DodgerBlue				= 0xFF1E90FF,
		Firebrick				= 0xFFB22222,
		FloralWhite				= 0xFFFFFAF0,
		ForestGreen				= 0xFF228B22,
		Fuchsia					= 0xFFFF00FF,
		Gainsboro				= 0xFFDCDCDC,
		GhostWhite				= 0xFFF8F8FF,
		Gold					= 0xFFFFD700,
		Goldenrod				= 0xFFDAA520,
		Gray					= 0xFF808080,
		Green					= 0xFF008000,
		GreenYellow				= 0xFFADFF2F,
		Honeydew				= 0xFFF0FFF0,
		HotPink					= 0xFFFF69B4,
		IndianRed				= 0xFFCD5C5C,
		Indigo					= 0xFF4B0082,
		Ivory					= 0xFFFFFFF0,
		Khaki					= 0xFFF0E68C,
		Lavender				= 0xFFE6E6FA,
		LavenderBlush			= 0xFFFFF0F5,
		LawnGreen				= 0xFF7CFC00,
		LemonChiffon			= 0xFFFFFACD,
		LightBlue				= 0xFFADD8E6,
		LightCoral				= 0xFFF08080,
		LightCyan				= 0xFFE0FFFF,
		LightGoldenrodYellow	= 0xFFFAFAD2,
		LightGray				= 0xFFD3D3D3,
		LightGreen				= 0xFF90EE90,
		LightPink				= 0xFFFFB6C1,
		LightSalmon				= 0xFFFFA07A,
		LightSeaGreen			= 0xFF20B2AA,
		LightSkyBlue			= 0xFF87CEFA,
		LightSlateGray			= 0xFF778899,
		LightSteelBlue			= 0xFFB0C4DE,
		LightYellow				= 0xFFFFFFE0,
		Lime					= 0xFF00FF00,
		LimeGreen				= 0xFF32CD32,
		Linen					= 0xFFFAF0E6,
		Magenta					= 0xFFFF00FF,
		Maroon					= 0xFF800000,
		MediumAquamarine		= 0xFF66CDAA,
		MediumBlue				= 0xFF0000CD,
		MediumOrchid			= 0xFFBA55D3,
		MediumPurple			= 0xFF9370DB,
		MediumSeaGreen			= 0xFF3CB371,
		MediumSlateBlue			= 0xFF7B68EE,
		MediumSpringGreen		= 0xFF00FA9A,
		MediumTurquoise			= 0xFF48D1CC,
		MediumVioletRed			= 0xFFC71585,
		MidnightBlue			= 0xFF191970,
		MintCream				= 0xFFF5FFFA,
		MistyRose				= 0xFFFFE4E1,
		Moccasin				= 0xFFFFE4B5,
		NavajoWhite				= 0xFFFFDEAD,
		Navy					= 0xFF000080,
		OldLace					= 0xFFFDF5E6,
		Olive					= 0xFF808000,
		OliveDrab				= 0xFF6B8E23,
		Orange					= 0xFFFFA500,
		OrangeRed				= 0xFFFF4500,
		Orchid					= 0xFFDA70D6,
		PaleGoldenrod			= 0xFFEEE8AA,
		PaleGreen				= 0xFF98FB98,
		PaleTurquoise			= 0xFFAFEEEE,
		PaleVioletRed			= 0xFFDB7093,
		PapayaWhip				= 0xFFFFEFD5,
		PeachPuff				= 0xFFFFDAB9,
		Peru					= 0xFFCD853F,
		Pink					= 0xFFFFC0CB,
		Plum					= 0xFFDDA0DD,
		PowderBlue				= 0xFFB0E0E6,
		Purple					= 0xFF800080,
		Red						= 0xFFFF0000,
		RosyBrown				= 0xFFBC8F8F,
		RoyalBlue				= 0xFF4169E1,
		SaddleBrown				= 0xFF8B4513,
		Salmon					= 0xFFFA8072,
		SandyBrown				= 0xFFF4A460,
		SeaGreen				= 0xFF2E8B57,
		SeaShell				= 0xFFFFF5EE,
		Sienna					= 0xFFA0522D,
		Silver					= 0xFFC0C0C0,
		SkyBlue					= 0xFF87CEEB,
		SlateBlue				= 0xFF6A5ACD,
		SlateGray				= 0xFF708090,
		Snow					= 0xFFFFFAFA,
		SpringGreen				= 0xFF00FF7F,
		SteelBlue				= 0xFF4682B4,
		Tan						= 0xFFD2B48C,
		Teal					= 0xFF008080,
		Thistle					= 0xFFD8BFD8,
		Tomato					= 0xFFFF6347,
		Transparent				= 0x00FFFFFF,
		Turquoise				= 0xFF40E0D0,
		Violet					= 0xFFEE82EE,
		Wheat					= 0xFFF5DEB3,
		White					= 0xFFFFFFFF,
		WhiteSmoke				= 0xFFF5F5F5,
		Yellow					= 0xFFFFFF00,
		YellowGreen				= 0xFF9ACD32
	};

	Color ArgbToColor(ColorARGB color)
	{
		Color c;
		uint32_t alpha = (color & 0xFF000000) >> 24;
		c.w = alpha / float(255);

		uint32_t red = (color & 0x00FF0000) >> 16;
		c.x = red / float(255);

		uint32_t green = (color & 0x0000FF00) >> 8;
		c.y = green / float(255);

		uint32_t blue = color & 0x000000FF;
		c.z = blue / float(255);

		return c;
	}
}
