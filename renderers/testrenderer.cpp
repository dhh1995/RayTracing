#include "testrenderer.h"

namespace Raytracer {

void TestRenderer::rayTracing(Ray ray, Color& res, int depth){
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

	Primitive* prim = isect.getPrim();
	Vec3f pi = isect.getPos();
	Color color = prim->getColor(pi);
	// calculate reflection
	real refl = prim->getMaterial()->getReflection();
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
			Vec3f N = prim->getNorm(pi);
			Vec3f R = ray.d - 2.0f * dot(ray.d, N) * N;
			Color rcol(0, 0, 0);
			real dist;
			rayTracing( Ray( pi + R * EPS, R ), rcol, depth + 1); //, a_RIndex, dist, a_Samples * 0.5f, a_SScale * 2 );
			mRaysCast++;
			res += refl * rcol * color;
		}
	}

	return;
}

void TestRenderer::render(){
	//assert(mCamera != NULL);
	vector<Ray> rays = mCamera->generateRays();
	cout << rays.size() << endl;
	for (Ray ray : rays){
		int x = ray.mFilmX, y = ray.mFilmY;
		//if (x % 30 == 0 && y % 40 == 0)
		//ray.prt();
		Color res;
		rayTracing(ray, res, 0);
		//printf("%d %d\n",x,y);
		mCamera->getFilm()->setColor(x, y, res);
	}
}

void TestRenderer::show(){
	mCamera->getFilm()->show();
}

}; // namespace Raytracer
