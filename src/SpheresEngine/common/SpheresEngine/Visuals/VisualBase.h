#pragma once

#include <SpheresEngine/VectorTypes.h>
#include <SpheresEngine/RenderEngine/RendererVisualChange.h>

#include <algorithm>
#include <utility>
#include <tuple>
#include <vector>
#include <string>
#include <boost/noncopyable.hpp>

struct VisualDataExtractContainer;

/** Id used by the RenderEngine to reference Visuals
 * which still need to be prepared.
 * This Id is used by the RenderEngine to identify the
 * visuals once they have been prepared
 */
typedef unsigned long PrepareVisualId;

/**
 * This struct transfer position and rotation offset
 * which need to be honored by visuals which are attached to a moved
 * or rotated entity
 */
struct ExtractOffset {
	/**
	 * Position offset in global coordinates
	 */
	Vector3 Position = Vector3::zero();

	/**
	 * Rotation offset as quaternion
	 */
	Quaternion Rotation = Quaternion(glm::vec3(0.0f, 0.0f, 0.0f));
};

/**
 * Abstract class where all concrete visuals can derive from. This class
 * is used to pack all Visuals into one list via pointers
 */
class VisualAbstract {
public:

	/**
	 * Virtual dtor to support inheritance
	 */
	virtual ~VisualAbstract() = default;

	/**
	 * Return the type of the visual as string
	 */
	virtual std::string getType() const = 0;

	/**
	 * Extract the render data contained in a visual
	 */
	virtual void extractData(VisualDataExtractContainer & cont,
			ExtractOffset const& eo) = 0;

	/**
	 * Update visual internal information if some new content (for example shader)
	 * was loaded during the last call to the render thread
	 */
	virtual void update(RendererVisualChange const&) {
		// ignore in this generic version, can be implemented by concrete visual classes
	}
};

/**
 * Templated Visual base class which provides convenience functions for accessing the
 * Visual data which is extracted for the render queue
 */
template<class TData>
class VisualBase: public VisualAbstract {
public:

	/**
	 * Typedef to the visual data type used by an inherited visual
	 */
	typedef TData ExtractData;

	/**
	 * Return the Visual data hold by this visual
	 */
	TData & getData() {
		return m_data;
	}

	/**
	 * Return a const-version of the visual data
	 */
	TData const& getData() const {
		return m_data;
	}

	/**
	 * Returns true if the Visual has been prepared, aka all assets loaded
	 * by the render thread
	 */
	bool isPrepared() const {
		return m_isPrepared;
	}

	/**
	 * Sets the Visual to be prepared
	 */
	void setPrepared() {
		m_isPrepared = true;
	}

private:

	/**
	 * Visual data which is passed to the Render thread
	 */
	TData m_data;

	/**
	 *  will be set to true, once the Render thread has loadad all necessary ingredients
	 */
	bool m_isPrepared = false;
};
