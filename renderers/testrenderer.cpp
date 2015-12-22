#include "testrenderer.h"

namespace Raytracer {

void TestRenderer::rayTracing(Ray ray, Color& res, int depth, real aRIndex, real &aDist){
	Intersection isect;
	if (mScene->intersect(ray, isect) == 0){
		res = BACKGROUND;
		return;
	}
	//real Li = mScene->getLi(isect);
	//printf("%lf\n",isect.getDist());
	//Vec3f pi = ray.o + ray.d * isect.getDist();
	//res = WHITE / (1 + (isect.getDist()));
	res = mScene->getLi(ray, isect);
	aDist = isect.getDist();

	Material* matter = isect.getPrim()->getMaterial();
	Vec3f pi = isect.getPos();
	Vec3f norm = isect.getNorm();
	Color color = isect.getColor();
	// calculate reflection
	real refl = matter->getReflection();
	if ((refl > 0.0f) && (depth < TRACEDEPTH))
	{
		// real drefl = prim->getMaterial()->getDiffuseRefl();
		// if ((drefl > 0) && (depth < 3))
		// {
		// 	// calculate diffuse reflection
		// 	Vec3f RP = ray.d - 2.0f * dot(ray.d, N) * N;
		// 	Vec3f RN1 = Vec3f(RP.z, RP.y, -RP.x);
		// 	Vec3f RN2 = cross(RP, RN1);
		// 	refl *= a_SScale;
		// 	for ( int i = 0; i < SAMPLES; i++ )
		// 	{
		// 		real xoffs, yoffs;
		// 		do
		// 		{
		// 			xoffs = (m_Twister.Rand() - 0.5f) * drefl;
		// 			yoffs = (m_Twister.Rand() - 0.5f) * drefl;
		// 		}
		// 		while ((xoffs * xoffs + yoffs * yoffs) > (drefl * drefl));
		// 		Vec3f R = RP + RN1 * xoffs + RN2 * yoffs * drefl;
		// 		NORMALIZE( R );
		// 		real dist;
		// 		Color rcol( 0, 0, 0 );
		// 		rayTracing( Ray( pi + R * EPS, R ), rcol, depth + 1, a_RIndex, dist, a_Samples * 0.25f, a_SScale * 4 );
		// 		mRaysCast++;
		// 		res += refl * rcol * color;
		// 	}
		// }
		// else
		{
			// calculate perfect reflection
			Vec3f N = isect.getNorm();
			Vec3f R = ray.d - 2.0f * dot(ray.d, N) * N;
			Color rcol(0, 0, 0);
			real dist;
			rayTracing( Ray( pi + R * EPS, R ), rcol, depth + 1, aRIndex, dist); //, a_Samples * 0.5f, a_SScale * 2 );
			mRaysCast++;
			res += refl * rcol * color;
		}
	}

	// calculate refraction
	real refr = matter->getRefraction();
	if ((refr > 0.0f) && (depth < TRACEDEPTH))
	{
		real rindex = matter->getRefrIndex();
		real n = aRIndex / rindex;
		Vec3f N = norm;
		real cosI = -dot(N, ray.d);
		real cosT2 = 1.0f - n * n * (1.0f - cosI * cosI);
		if (cosT2 > 0.0f)
		{
			Vec3f T = (n * ray.d) + (n * cosI - sqrt( cosT2 )) * N;
			//printf("%lf %lf %lf\n", n, cosI, cosT2); T.prt();
			Color rcol( 0, 0, 0 );
			real dist;
			rayTracing( Ray(pi + T * EPS, T), rcol, depth + 1, rindex, dist); //, a_Samples * 0.5f, a_SScale * 2 );
			mRaysCast++;
			// apply Beer's law
			Color absorbance = matter->getColor() * 0.15f * -dist;
			Color transparency = Color( exp( absorbance.r ), exp( absorbance.g ), exp( absorbance.b ) );
			res += refr * rcol * transparency;
		}
	}

	return;
}

void TestRenderer::render(){
	//assert(mCamera != NULL);
	vector<Ray> rays = mCamera->generateRays();
	int nRays = rays.size(), cnt = 0, tot = 100;
	int lastShow = -1;
	cout << nRays << endl;
	for (Ray ray : rays){
		++cnt;
		int percent = cnt * tot / nRays;
		if (percent != lastShow){
			lastShow = percent;
			printf("%d/%d\n", percent,tot);
		}

		int x = ray.mFilmX, y = ray.mFilmY;
		//if (x>=45 && x<=88)
		//	printf("%d %d\n", x, y);
		//if (x % 30 == 0 && y % 40 == 0)
		//ray.prt();
		Color res;
		real dist;
		rayTracing(ray, res, 0, 1, dist);
		//printf("%d %d\n",x,y);
		mCamera->getFilm()->setColor(x, y, res);
	}
}

void TestRenderer::show(){
	mCamera->getFilm()->show();
}

}; // namespace Raytracer
