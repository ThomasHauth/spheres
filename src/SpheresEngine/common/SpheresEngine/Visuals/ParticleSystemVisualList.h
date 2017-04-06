#pragma once

#include <list>
#include <vector>
#include <SpheresEngine/Util.h>
#include "ParticleSystemVisual.h"


typedef std::list<ParticleSystemVisual *> ParticeSystemVisualList;
typedef std::list<uniq<ParticleSystemVisual>> ParticeSystemVisualListUniq;

typedef std::vector<ParticleSystemVisual *> ParticeSystemVisualVectorAdd;
typedef std::vector<ParticleSystemVisual *> ParticeSystemVisualVector;

