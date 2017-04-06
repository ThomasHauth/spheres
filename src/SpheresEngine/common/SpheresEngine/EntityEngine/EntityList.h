#pragma once

#include <SpheresEngine/Util.h>
#include <vector>

#include "Entity.h"

/**
 * typedef for transferring a non-owning list of pointers to entities
 */
typedef std::vector<Entity*> EntityList;

/**
 * typedef for transferring an owning list of pointers to entities
 */
typedef std::vector<uniq<Entity>> EntityListUniq;
