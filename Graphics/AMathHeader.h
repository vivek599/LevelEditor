#pragma once

using namespace DirectX;
using namespace DirectX::SimpleMath;

namespace 
{
	namespace Math
	{
	#define BIG_EPSILON				0.00001f    
	#define SMALLL_EPSILON			0.000001f    
	#define PI						3.141592654    
	#define HALFPI					1.570796327    
	#define TWOPI					6.283185307    
	#define DEG_TO_RAD(x)			((1.74532952e-2)*(x))    
	#define RAD_TO_DEG(x)			(57.29577951*(x))   
	#define ROOT_2					1.414213562    
	#define SIN_45					0.707106781

		Vector3 Rotation( const Vector3& vector)
		{
			Vector3 d = vector;
			d.Normalize();
			return Vector3(asin(-d.y), atan2(d.x, d.z), 0.0f);
		}

		template<class T>
		constexpr const T& Clamp(const T& d, const T& min, const T& max)
		{
			const T t = d < min ? min : d;
	
			return t > max ? max : t;
		}
	
		template<class T>
		constexpr const T& Lerp(const T& min, const T& max, const float alpha)
		{
			return min + (max - min) * alpha;
		}
	
		template<class T>
		constexpr const float InvLerp(const T& min, const T& max, const T val)
		{
			return (val - min) / (max - min);
		}
	
		template<class T>
		constexpr const T& Remap(const T& iMin, const T& iMax, const T& oMin, const T& oMax, const T& value)
		{
			float alpha = InvLerp(iMin, iMax, value);
	
			return Lerp(oMin, oMax, alpha);
		}
	
		float SmotherStep(const float& minVal, const float& maxVal, float& x)
		{
			// Scale, and clamp x to 0..1 range
			const float& val = (x - minVal) / (maxVal - minVal);
			x = Clamp(val, 0.0f, 1.0f);
	
			// Evaluate polynomial
			return x * x * x * (x * (x * 6 - 15) + 10);
		};
	
		Vector3& RLerp(Vector3& v1, Vector3& v2, float t)
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
	
			if (dot > DOT_THRESHOLD)
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
	
		Vector3& Slerp(Vector3& v0, Vector3& v1, float t)
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
	
		Vector3& Slerp3(Vector3& v0, Vector3& v1, double t)
		{
			Quaternion q0 = Quaternion(v0);
			Quaternion q1 = Quaternion(v1);
	
			Quaternion q2 = Quaternion::Slerp(q0, q1, t);
	
			Vector3 result = Vector3(q2.x, q2.y, q2.z);
			result.Normalize();
			return result;
		}
	
		Vector3& Slerp2(Vector3& v0, Vector3& v1, double t)
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
	
		Vector3& Nlerp(Vector3& start, Vector3& end, float percent)
		{
			Vector3 result = Lerp(start, end, percent);
	
			result.Normalize();
	
			return result;
		}
	 
	}
}
